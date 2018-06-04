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


using namespace Garfield;

void SetupInfo(double gem[9], double diam, double dist,
               double up, double low, double t_die, double t_pla,
               double i_field, double d_field, double potential) {


 // Dimensions in cm, Fields in V/cm and Potentials in V
  gem[0] = diam / 10000;    // Hole Diameter
  gem[1] = dist / 10000;    // Distance between Holes
  gem[2] = up;              // Distance to Electrode
  gem[3] = low;             // Distance to Pad
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
  elm -> EnablePeriodicityX();
  elm -> EnableMirrorPeriodicityY();

  double o2 = ppm * 1E-4;
  double co2 = 100. - percent - o2;


  // Medium
  MediumMagboltz* gas = new MediumMagboltz();
  gas -> SetComposition(noble, percent, "co2", co2, "o2", o2);
  gas -> SetTemperature(temp);
  gas -> SetPressure(press);
  gas -> SetMaxElectronEnergy(2000.);
  gas -> EnableDrift();
  gas -> EnableDebugging();
  gas -> Initialise();
  gas -> DisableDebugging();

  if (penning != 0.) {
   gas -> EnablePenningTransfer(penning, 0., noble);
  }
  else {
   gas -> DisablePenningTransfer();
  }

  elm -> SetMedium(0, gas);

  return elm;

}



void GainOneElectron(std::string folder, double info[9],
                     std::string txtfile, bool plot = true,
                     int sizelimit = 10, int n_events = 100,
                     bool penning = true) {


  // GEM Dimensions in cm
  const double T_DIE = info[4];
  const double T_PLA = info[5];
  const double DIST = info[1];
  const double H = sqrt(3) * DIST / 2;
  const double D_E = info[2];
  const double D_P = info[3];

  const double Z_AXIS = -1 * (D_P + T_DIE / 2 + T_PLA);
  const double electron_pos = T_DIE / 2 + T_PLA + (0.1 + 0.8 * RndmUniform()) * D_E;


  // Import and Set Medium
  ComponentElmer* Elm = LoadGas(folder);


  // Sensor.
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
    int ne = 0, ni = 0, nf = 0, n_other = 0;
    aval -> GetAvalancheSize(ne, ni);

    // Final Positions Analysis
    double xe1, ye1, ze1, te1, e1;
    double xe2, ye2, ze2, te2, e2;
    int status;
    for (int j = np; j--;) {
      aval -> GetElectronEndpoint(j, xe1, ye1, ze1, te1, e1,
                                  xe2, ye2, ze2, te2, e2, status);

      if (ze2 <= Z_AXIS + 0.001) {
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



void ReadTXTGain(std::string txtfolder) {

  std::string folder = "./" + txtfolder;
  DIR* dir = opendir(folder.c_str());
  dirent* pdir;
  std::vector<std::string> txts;
  std::string line;
  int rg, eg;
  std::string rootname = folder + "_hists.root";


  // Getting all files of folder
  while (pdir = readdir(dir)) {
    std::string fn = pdir -> d_name;
    if (fn != "." and fn != "..") {
      txts.push_back(fn);
    }
  }

  TFile* f = new TFile(rootname.c_str(), "RECREATE");
  chdir(folder.c_str());

  for (int i = 0; i < txts.size(); i++) {
    std::ifstream File;
    File.open(txts[i].c_str());
    std::vector<int> RGain, EGain;

    if (File.fail()) {
      std::cerr << "\nFailed to open file\n" << std::endl;
      exit(1);
    }

    while (!File.eof()) {
      File >> line;
      size_t stop1 = line.find(";");
      size_t stop2 = line.find(";", stop1 + 1);
      rg = stod(line.substr(0, stop1));
      eg = stod(line.substr(stop1 + 1, stop2 - stop1 - 1));
      RGain.push_back(rg);
      EGain.push_back(eg);
    }

    // Popping last duplicated elements
    RGain.pop_back();
    EGain.pop_back();

    // Histograms
    int nBins = 100;
    float hmin = 0.;
    float hmaxR = *max_element(RGain.begin(), RGain.end());
    float hmaxE = *max_element(EGain.begin(), EGain.end());
    std::string name_r = "hRGain_" + txts[i].substr(0, txts[i].size() - 4);
    std::string name_e = "hEGain_" + txts[i].substr(0, txts[i].size() - 4);

    TH1I* hRGain = new TH1I(name_r.c_str(), "Real Gain", nBins, hmin, hmaxR);
    TH1I* hEGain = new TH1I(name_e.c_str(), "Effective Gain", nBins, hmin, hmaxE);

    for (int i = 0; i < RGain.size(); i++) {
      hRGain -> Fill(RGain[i]);
      hEGain -> Fill(EGain[i]);
    }

    std::cout << "\nFilled histograms for " << txts[i] << std::endl;
    hRGain -> Write();
    hEGain -> Write();
    std::cout << "Saved histograms for " << txts[i] << std::endl;

  }

  f -> Close();
  chdir("..");
  std::cout << "\nFinished saving all histograms!\n" << std::endl;

}




#endif
