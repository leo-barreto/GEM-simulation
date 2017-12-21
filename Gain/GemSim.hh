#ifndef GEMSIM_H
#define GEMSIM_H


#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

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

void SetupInfo(double gem[9], std::string folder, double up, double low,
               double t_die, double t_pla, double i_field, double d_field) {

 // Dimensions in cm, Fields in V/cm and Potentials in V
 gem[0] = std::stod(folder.substr(3, 5)) / 10000;    // Hole Diameter
 gem[1] = std::stod(folder.substr(6, 9)) / 10000;    // Distance between Holes
 gem[2] = up;                                        // Distance to Electrode
 gem[3] = low;                                       // Distance to Pad
 gem[4] = t_die;                                     // Dieletric Thickness
 gem[5] = t_pla;                                     // Plates Thickness
 gem[6] = i_field;                                   // Induction Field
 gem[7] = d_field;                                   // Drift Field
 gem[8] = std::stod(folder.substr(10, 13));          // GEM Potential

 std::cout << "\nSetup information collected." << std::endl;
}


void GainOneElectron(std::string folder, double info[9],
                     double electron_pos, int n_events) {

  auto t_start = std::chrono::high_resolution_clock::now();
  TApplication app("app", &argc, argv);


  // GEM Dimensions in cm
  const double T_DIE = info[4];
  const double T_PLA = info[5];
  const double DIST = info [1];
  const double H = sqrt(3) * DIST / 2;
  const double D_E = info[2];
  const double D_P = info[3];

  const double Z_AXIS = -1 * (D_P + T_DIE / 2 + T_PLA);


  // Import
  ComponentElmer* elm = new ComponentElmer(folder + "/mesh.header",
                                           folder + "/mesh.elements",
                                           folder + "/mesh.nodes",
                                           folder + "/dielectrics.dat",
                                           folder + "/gem.result", "mm");
  elm -> EnablePeriodicityX();
  elm -> EnableMirrorPeriodicityY();


  // Medium
  MediumMagboltz* gas = new MediumMagboltz();
  gas -> SetComposition("ar", 70., "co2", 30.);
  gas -> SetTemperature(293.15);
  gas -> SetPressure(760.);
  gas -> SetMaxElectronEnergy(2000.);
  gas -> EnableDrift();
  gas -> EnableDebugging();
  gas -> Initialise();
  gas -> DisableDebugging();
  elm -> SetMedium(0, gas);


  // Sensor.
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(elm);
  sensor -> SetArea(-DIST, -H, Z_AXIS, DIST, H, electron_pos + 0.01);


  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> EnableAvalancheSizeLimit(10);


  // Histograms
  int nBins = 100;
  float hmin = 0.;
  float hmax = 50.;

  TH1F* hRGain = new TH1F("hRGain", "Real Gain", nBins, hmin, hmax);
  TH1F* hEGain = new TH1F("hEGain", "Effective Gain", nBins, hmin, hmax);
  TH1F* h0RGain = new TH1F("h0RGain", "Real Gain 0", nBins, hmin, hmax);
  TH1F* h0EGain = new TH1F("h0EGain", "Effective Gain 0", nBins, hmin, hmax);

  // Avalanches Calculations
  for (int i = N_AVAL; i--;) {
    // Random Initial Positions
    double x0 = (2 * RndmUniform() - 1) * DIST / 2;
    double y0 = (2 * RndmUniform() - 1) * DIST / 2;
    aval -> AvalancheElectron(x0, y0, Z0, 0, 0, 0., 0., 0.);
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
      if (ze2 <= Z_AXIS) {
        nf += 1;
      }
      else {
        n_other += 1;
      }
    }


    hRGain -> Fill(np);
    hEGain -> Fill(nf);
    if (nf > 0) {
      h0RGain -> Fill(np);
      h0EGain -> Fill(nf);
    }

    std::cout << "\n" << N_AVAL -  i << "/" << N_AVAL;
    std::cout << "\n... avalanche complete with " << np
              << " electron tracks. (number of electrons "
              << ne << ")" << std::endl;

    std::cout << "\n... nf: " << nf << ", n_other: " << n_other << std::endl;
  }


  // Plotting
  TCanvas* cH = new TCanvas("cH", "Histograms", 800, 400);
  cH -> Divide(2,2);
  cH -> cd(1);
  hRGain -> Draw();
  cH -> cd(2);
  hEGain -> Draw();
  cH -> cd(3);
  h0RGain -> Draw();
  cH -> cd(4);
  h0EGain -> Draw();


  auto t_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = t_end - t_start;
  std::cout << "\n... END OF SIMULATION: " << diff.count() << " s.\n"<< std::endl;

  app.Run(kTRUE);








}




#endif
