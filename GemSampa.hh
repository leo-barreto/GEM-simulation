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

void SetupInfo(double gem[9], double diam, double dist,
               double up, double low, double t_die, double t_pla,
               double i_field, double d_field, double potential) {


 // Dimensions in cm, Fields in V/cm and Potentials in V
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

  //elm -> SetMedium(0, gas);
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


void Gain(ComponentElmer* Elm, double info[9], std::string txtfile,
          int sizelimit = 10, int n_events = 100) {


  // GEM Dimensions in cm
  const double T_DIE = info[4];
  const double T_PLA = info[5];
  const double DIST = info[1];
  const double H = sqrt(3) * DIST / 2;
  const double D_E = info[2];
  const double D_P = info[3];

  const double Center = (D_E - D_P) / 2;
  const double Z_AXIS = -1 * (D_P + T_DIE / 2 + T_PLA) - Center;
  const double electron_pos = T_DIE / 2 + T_PLA + (0.1 + 0.7 * RndmUniform()) * D_E - Center;


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(Elm);
  sensor -> SetArea(-DIST, -H, Z_AXIS, DIST, H, electron_pos + 0.01);


  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(100);
  if (sizelimit > 0) {
    aval -> EnableAvalancheSizeLimit(sizelimit);
  }


  // Avalanches Calculations
  for (int i = n_events; i--;) {
    const clock_t begin_time = clock();

    // Random Initial Positions
    double x0 = (2 * RndmUniform() - 1) * DIST / 2;
    double y0 = (2 * RndmUniform() - 1) * DIST / 2;
    aval -> AvalancheElectron(x0, y0, electron_pos, 0, 0, 0., 0., 0.);
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

      if (ze2 <= Z_AXIS + 0.001) {  // Added a delta to minimize border effects
        nf += 1;
      }
    }

    // Saving Gains
    FILE* file;
    const char* f_title = txtfile.c_str();
    file = fopen(f_title, "a");
    float min = float(clock () - begin_time) / (CLOCKS_PER_SEC * 60);
    fprintf(file, "%d;%d;%.2f\n", np, nf, min);
    fclose(file);

    if (np == 1) {
      i += 1;         // No multiplication doesn't add to counter
    }

  }
}


void LaunchParticle(ComponentElmer* Elm, double info[9], std::string txtfile,
               double energy, int n_events = 1000,
               const char* particle = "alpha", bool plot = true) {


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
  sensor -> SetArea(-2 * DIST, -2 * H, Z_AXIS, 2 * DIST, 2 * H, z0);


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
        double xe2, ye2, ze2, te2, e2;
        int status;
        track -> GetElectron(j, xe1, ye1, ze1, te1, e1, dxe, dye, dze);

        // Saving on txt file
        // Energy and velocity are not provided by Heed, so they are ignored
        FILE* file;
        const char* f_title = txtfile.c_str();
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
    hEne-> GetXaxis() -> SetTitle("Energy Loss [keV]");
    hEne -> Draw();
  }
}


void EnergyResolution(ComponentElmer* Elm, double info[9], std::string txtfile,
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
  sensor -> SetArea(-2 * DIST, -2 * H, Z_AXIS, 2 * DIST, 2 * H, z0);

  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(100);
  // aval -> EnableAvalancheSizeLimit(10);

  // Setup Track
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);

  for (int i = n_events; i--;) {
    int nf = 0;
    track -> TransportPhoton(x0, y0, z0, t0, energy, dx0, dy0, dz0, nel);

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

        if (ze3 <= Z_AXIS + 0.001) {  // Added a delta to minimize border effects
          nf += 1;
        }
      }
    }

    // Saving on txt file
    FILE* file;
    const char* f_title = txtfile.c_str();
    file = fopen(f_title, "a");
    fprintf(file, "%d\n", nf);
    fclose(file);
  }
}



#endif
