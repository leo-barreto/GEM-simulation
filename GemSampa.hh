#ifndef GEMSAMPA_H
#define GEMSAMPA_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <dirent.h>
#include <math.h>

#include <TApplication.h>
#include <TMath.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TFile.h>

#include "ComponentElmer.hh"
#include "MediumMagboltz.hh"
#include "Sensor.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"
#include "Random.hh"
#include "TrackHeed.hh"


using namespace Garfield;

// Old manual setup of detector's properties
void SetupInfoManual(double gem[9], double diam, double dist,
               double up, double low, double t_die, double t_pla,
               double i_field, double d_field, double potential) {


 // Dimensions in cm (except diam and dist), Fields in V/cm and Potentials in V
  gem[0] = diam / 10000;    // Hole Diameter
  gem[1] = dist / 10000;    // Distance between Holes
  gem[2] = up;              // Drift Distance
  gem[3] = low;             // Induction Distance
  gem[4] = t_die;           // Dieletric Thickness
  gem[5] = t_pla;           // Plates Thickness
  gem[6] = i_field;         // Induction Field
  gem[7] = d_field;         // Drift Field
  gem[8] = potential;       // GEM Potential

}



/*std::vector<float> SetupInfo(std::string folder) {


  std::vector<float>* f;
  std::string infoloc = folder + "/info.root";
  const char* il = infoloc.c_str();
  TFile* F = new TFile(il);
  F -> GetObject("info", f);
  std::vector<float> g = *f;
  return g;
}*/

std::vector<float> SetupInfo(std::string folder) {


  std::vector<float> g;
  std::string infoloc = folder + "/info.txt";
  std::ifstream File;
  File.open(infoloc.c_str());
  std::string line;

  if (File.fail()) {
    std::cerr << "\nFailed to open info.txt\n" << std::endl;
    exit(1);
  }

  while (!File.eof()) {
    File >> line;
    g.push_back(atof(line.c_str()));
  }

  // Popping last duplicated element
  g.pop_back();

  return g;
}


void WriteSetup(const std::string &name, std::vector<float> g) {
  const char *f = name.c_str();
  FILE* file;
  // Check if the file name doesn't exist
  if (!(file = fopen(f, "r"))) {
    file = fopen(f, "w");

    // Write detector info
    fprintf(file, "# GEM INFO\n");
    /*for (const float &p : g) {
      fprintf(file, "%f\n", p);
    }*/

    for (std::size_t i = 0; i < g.size(); i++){
        fprintf(file, "%f\n", g[i]);
    }

    // Write gas info



    fprintf(file, "#########\n");
    fclose(file);
  }
}



ComponentElmer* LoadGas(std::string folder, double percent = 70.,
                        double ppm = 0., double penning = 0.57,
                        const char* noble = "ar",
                        double temp = 293.15, double press = 760.) {
  // Temperature (temp) in Kelvin and Pressure (press) in Torr


  // Import
  ComponentElmer* elm = new ComponentElmer(folder + "/mesh.header",
                                          folder + "/mesh.elements",
                                          folder + "/mesh.nodes",
                                          folder + "/dielectrics.dat",
                                          folder + "/gem.result", "mm");
  elm -> SetWeightingField(folder + "/gemWT.result", "WT");
  elm -> EnableMirrorPeriodicityX();
  elm -> EnableMirrorPeriodicityY();
  elm -> PrintRange();


  double o2 = ppm * 1E-4;
  double co2 = 100. - percent - o2;


  // Medium
  MediumMagboltz* gas = new MediumMagboltz();
  gas -> SetComposition(noble, percent, "co2", co2, "o2", o2);
  gas -> SetTemperature(temp);
  gas -> SetPressure(press);
  gas -> SetMaxElectronEnergy(2000.);
  gas -> EnableDrift();
  gas -> Initialise();

  if (penning != 0.) {
   gas -> EnablePenningTransfer(penning, 0., noble);
  }
  else {
   gas -> DisablePenningTransfer();
  }

  int nMaterials = elm -> GetNumberOfMaterials();
  for (int i = 0; i < nMaterials; ++i) {
    double eps = elm -> GetPermittivity(i);
    if (eps == 1.) {
      elm -> SetMedium(i, gas);
    }
  }
  elm -> PrintMaterials();

  return elm;

}



void Gain(ComponentElmer* Elm, std::vector<float> g, std::string txtfile,
          int sizelimit = 10, int n_events = 100) {


  // GEM Dimensions
  const double H = sqrt(3) * g[1] / 2;
  const double CENTER = (g[2] - g[3]) / 2;
  const double Z_MIN = -1 * (g[3] + g[4] / 2 + g[5]) - CENTER;
  const double Z_MAX = g[2] + g[4] / 2 + g[5] - CENTER;


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(Elm);
  sensor -> SetArea(-5 * g[1], -5 * H, Z_MIN, 5 * g[1], 5 * H, Z_MAX);


  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(100);
  if (sizelimit > 0) {
    aval -> EnableAvalancheSizeLimit(sizelimit);
    std::cout << "Aval Limit: " << aval -> GetAvalancheSizeLimit() << std::endl;
  }


  // File
  FILE* file;
  const char* f_title = txtfile.c_str();
  WriteSetup(txtfile, g);


  // Avalanches Calculations
  for (int i = n_events; i--;) {
    clock_t begin_time = clock();

    // Random Initial Positions
    double z0 = g[4] / 2 + g[5] + RndmUniform() * g[2] - CENTER;
    double x0 = (2 * RndmUniform() - 1) * g[1] / 2;
    double y0 = (2 * RndmUniform() - 1) * g[1] / 2;
    aval -> AvalancheElectron(x0, y0, z0, 0, 0, 0., 0., 0.);
    int np = aval -> GetNumberOfElectronEndpoints();
    int ne = 0, ni = 0, nf = 0;
    aval -> GetAvalancheSize(ne, ni);

    // Final Positions Analysis
    double xe1, ye1, ze1, te1, e1;
    double xe2, ye2, ze2, te2, e2;
    int status;
    for (int j = np; j--;) {
      aval -> GetElectronEndpoint(j, xe1, ye1, ze1, te1, e1,
                                  xe2, ye2, ze2, te2, e2, status);

      if (ze2 <= Z_MIN + 0.001) {  // Added a delta to minimize border effects
        nf += 1;
      }
      if (status == -1) {
        std::cout << "Electron left the drift area!" << std::endl;
      }
    }

    // Saving Gains
    file = fopen(f_title, "a");
    float min = float(clock() - begin_time) / (CLOCKS_PER_SEC * 60);
    fprintf(file, "%d;%d;%.2f\n", np, nf, min);
    fclose(file);

    if (np == 1) {
      i += 1;         // No multiplication doesn't add to counter
    }

    std::cout << i << "/" << n_events << std::endl;

  }
}



void LaunchParticle(ComponentElmer* Elm, double info[9], std::string txtfile,
               double energy, int n_events = 1000,
               const char* particle = "alpha", bool plot = false) {


  // GEM Dimensions in cm
  const double T_DIE = info[4];
  const double T_PLA = info[5];
  const double DIST = info[1];
  const double H = sqrt(3) * DIST / 2;
  const double D_E = info[2];
  const double D_P = info[3];

  const double Center = (D_E - D_P) / 2;
  const double Z_AXIS = -1 * (D_P + T_DIE / 2 + T_PLA) - Center;


  // Initial Parameters for Track
  double x0 = (2 * RndmUniform() - 1) * DIST / 2;
  double y0 = (2 * RndmUniform() - 1) * H / 2;
  double z0 = T_DIE / 2 + T_PLA + D_E - 0.01 - Center;
  double t0 = 0.;
  double dx0 = 2 * RndmUniform() - 1;
  double dy0 = 2 * RndmUniform() - 1;
  double dz0 = -1;


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(Elm);
  sensor -> SetArea(-10 * DIST, -10 * H, Z_AXIS, 10 * DIST, 10 * H, z0);


  // Histograms
  TH1::StatOverflows(kTRUE);
  TH1F* hEle = new TH1F("hEle", "Number of Primary Electrons",
                        100, 0, 200);
  TH1F* hEne = new TH1F("hEne", "Energy Loss", 100, 0., 10.);


  // Setup Track
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);
  track -> SetParticle(particle);
  track -> SetEnergy(energy);


  // File
  FILE* file;
  const char* f_title = txtfile.c_str();


  for (int i = n_events; i--;) {
    track -> NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);

    // Cluster coordinates
    double xc, yc, zc, tc, extra;
    // Number of electrons produced in a collision and total
    int nc = 0, nsum = 0.;
    // Energy loss in a collision and total
    double ec = 0., esum = 0.;

    // Loop over the clusters
    while (track -> GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
      esum += ec;
      nsum += nc;
      for (int j = 0; j < nc; j++) {
        // Properties of electron (pos, time, energy, velocity vector)
        double xe1, ye1, ze1, te1, e1, dxe, dye, dze;
        // double xe2, ye2, ze2, te2, e2;
        // int status;
        track -> GetElectron(j, xe1, ye1, ze1, te1, e1, dxe, dye, dze);

        // Saving on txt file
        // Energy and velocity are not provided by Heed, so they are ignored
        file = fopen(f_title, "a");
        fprintf(file, "%d;%f;%f;%f;%f\n", i, xe1, ye1, ze1, te1);
        fclose(file);
      }
    }

    hEle -> Fill(nsum);
    hEne -> Fill(esum * 1.E-3);

  }

  if (plot == true) {
    TCanvas* c1 = new TCanvas();
    hEle -> GetXaxis() -> SetTitle("Number of Electrons");
    hEle -> Draw();

    TCanvas* c2 = new TCanvas();
    hEne -> GetXaxis() -> SetTitle("Energy Loss [keV]");
    hEne -> Draw();
  }
}



void EnergyResolution(ComponentElmer* Elm, std::vector<float> g,
                      std::string txtfile, double energy, int n_events = 1) {
  // Launching a photon, non-parallel calculation


  // Initial Parameters for Track
  const double Center = (g[2] - g[3]) / 2;
  const double Z_AXIS = -1 * (g[3] + g[4] / 2 + g[5]) - Center;
  const double H = sqrt(3) * g[1] / 2;

  // Position
  const double x0 = 0.;
  const double y0 = 0.;
  const double z0 = g[4] / 2 + g[5] + g[2] - 0.001 - Center;
  const double t0 = 0.;

  // Velocity vector (direction only)
  const double dx0 = 0;
  const double dy0 = 0;
  const double dz0 = -1;


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(Elm);
  sensor -> SetArea(-10 * g[1], -10 * H, Z_AXIS, 10 * g[1], 10 * H, z0);


  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);


  // Setup Track
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);


  // File
  FILE* file;
  const char* f_title = txtfile.c_str();
  WriteSetup(txtfile, g);


  for (int i = n_events; i--;) {
    int nf = 0, nel = 0;
    while (nel == 0) {
      track -> TransportPhoton(x0, y0, z0, t0, energy, dx0, dy0, dz0, nel);
    }
    //std::cout << "Number of Primary Electrons: " << nel << std::endl;

    for (int j = 0; j < nel; j++) {
      // Properties of electron (pos, time, energy, velocity vector)
      double xe1, ye1, ze1, te1, e1, dxe, dye, dze;
      track -> GetElectron(j, xe1, ye1, ze1, te1, e1, dxe, dye, dze);

      aval -> AvalancheElectron(xe1, ye1, ze1, 0, 0, 0., 0., 0.);
      int np = aval -> GetNumberOfElectronEndpoints();

      std::cout << i << ".Avalanche: " << j << "/" << nel << std::endl;

      // Final Positions Analysis
      double xe2, ye2, ze2, te2, e2;
      double xe3, ye3, ze3, te3, e3;
      int status;
      for (int k = np; k--;) {
        aval -> GetElectronEndpoint(k, xe2, ye2, ze2, te2, e2,
                                    xe3, ye3, ze3, te3, e3, status);

        if (ze3 <= Z_AXIS + 0.001) {  // Added a delta to minimize border effects
          nf += 1;
        }

        if (status == -1) {
          std::cout << "Electron left the drift area!" << std::endl;
        }

      }
    }

    // Saving on txt
    file = fopen(f_title, "a");
    fprintf(file, "%d\n", nf);
    fclose(file);
  }
}



void PositionResolution(ComponentElmer* Elm, double info[9], std::string txtfile,
                        double energy, int n_events = 1,
                        const char* particle = "alpha") {

  // GEM Dimensions in cm
  const double T_DIE = info[4];
  const double T_PLA = info[5];
  const double DIST = info[1];
  const double H = sqrt(3) * DIST / 2;
  const double D_E = info[2];
  const double D_P = info[3];

  const double Center = (D_E - D_P) / 2;
  const double Z_AXIS = -1 * (D_P + T_DIE / 2 + T_PLA) - Center;


  // Initial Parameters for Track
  double x0 = 0;
  double y0 = 0;
  double z0 = T_DIE / 2 + T_PLA + D_E - 0.01 - Center;
  double t0 = 0.;
  double dx0 = 0;
  double dy0 = 0;
  double dz0 = -1;


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(Elm);
  sensor -> SetArea(-10 * DIST, -10 * H, Z_AXIS, 10 * DIST, 10 * H, z0);


  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(100);


  // Setup Track
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);
  track -> SetParticle(particle);
  track -> SetEnergy(energy);


  // File
  FILE* file;
  const char* f_title = txtfile.c_str();


  for (int i = n_events; i--;) {
    track -> NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);

    // Cluster properties
    double xc, yc, zc, tc, extra, ec;
    int nc;
    // Sum of all positions in x and y and electrons detected
    double sumy = 0, sumx = 0;
    int nf = 0;

    // Loop over the clusters
    while (track -> GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
      for (int j = 0; j < nc; j++) {
        // Properties of electron (pos, time, energy, velocity vector)
        double xe1, ye1, ze1, te1, e1, dxe, dye, dze;
        double xe2, ye2, ze2, te2, e2;
        double xe3, ye3, ze3, te3, e3;
        int status;
        track -> GetElectron(j, xe1, ye1, ze1, te1, e1, dxe, dye, dze);

        aval -> AvalancheElectron(xe1, ye1, ze1, 0, 0, 0., 0., 0.);
        int np = aval -> GetNumberOfElectronEndpoints();

        // Final Positions Analysis
        for (int k = np; k--;) {
          aval -> GetElectronEndpoint(k, xe2, ye2, ze2, te2, e2,
                                      xe3, ye3, ze3, te3, e3, status);

          if (ze3 <= Z_AXIS + 0.001) {  // Added a delta to minimize border effects
            nf++;
            sumx += xe3;
            sumy += ye3;
          }
        }
      }
    }

    // Saving on txt
    file = fopen(f_title, "a");
    fprintf(file, "%f;%f\n", sumx / nf, sumy / nf);
    fclose(file);
  }
}



void LaunchPhoton(ComponentElmer* Elm, double info[9], std::string txtfile,
                      double energy, int n_events = 1) {
  // Launching a photon, non-parallel calculation


  // GEM Dimensions in cm
  const double T_DIE = info[4];
  const double T_PLA = info[5];
  const double DIST = info[1];
  const double H = sqrt(3) * DIST / 2;
  const double D_E = info[2];
  const double D_P = info[3];

  const double Center = (D_E - D_P) / 2;
  const double Z_AXIS = -1 * (D_P + T_DIE / 2 + T_PLA) - Center;


  // Initial Parameters for Track
  double x0 = (2 * RndmUniform() - 1) * DIST / 2;
  double y0 = (2 * RndmUniform() - 1) * H / 2;
  double z0 = T_DIE / 2 + T_PLA + D_E - 0.01 - Center;
  double t0 = 0.;
  double dx0 = 2 * RndmUniform() - 1;
  double dy0 = 2 * RndmUniform() - 1;
  double dz0 = -1;
  int nel = -1;   // Number of Electrons Produced


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(Elm);
  sensor -> SetArea(-10 * DIST, -10 * H, Z_AXIS, 10 * DIST, 10 * H, z0);


  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(100);


  // Setup Track
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);


  // File
  FILE* file;
  const char* f_title = txtfile.c_str();


  for (int i = n_events; i--;) {
    track -> TransportPhoton(x0, y0, z0, t0, energy, dx0, dy0, dz0, nel);
    //std::cout << "Number of Primary Electrons: " << nel << std::endl;

    for (int j = 0; j < nel; j++) {
      // Properties of electron (pos, time, energy, velocity vector)
      double xe1, ye1, ze1, te1, e1, dxe, dye, dze;
      track -> GetElectron(j, xe1, ye1, ze1, te1, e1, dxe, dye, dze);

      aval -> AvalancheElectron(xe1, ye1, ze1, 0, 0, 0., 0., 0.);
      int np = aval -> GetNumberOfElectronEndpoints();

      // Final Positions Analysis
      double xe2, ye2, ze2, te2, e2;
      double xe3, ye3, ze3, te3, e3;
      int status;
      for (int k = np; k--;) {
        aval -> GetElectronEndpoint(k, xe2, ye2, ze2, te2, e2,
                                    xe3, ye3, ze3, te3, e3, status);

        file = fopen(f_title, "a");
        fprintf(file, "%d;%f;%f;%f;%f;%f\n", i, xe3, ye3, ze3, te3, e3);
        fclose(file);

      }
    }
  }
}

#endif
