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

 std::cout << "\nSetup information collected:" << std::endl;
 std::cout << "    Hole Diameter: " << gem[0] << " cm" << std::endl;
 std::cout << "    Distance between Holes: " << gem[1] << " cm" << std::endl;
 std::cout << "    Distance to Electrode: " << gem[2] << " cm" << std::endl;
 std::cout << "    Distance to Pad: " << gem[3] << " cm" << std::endl;
 std::cout << "    Dieletric Thickness: " << gem[4] << " cm" << std::endl;
 std::cout << "    Plates Thickness: " << gem[5] << " cm" << std::endl;
 std::cout << "    Induction Field: " << gem[6] << " V/cm" << std::endl;
 std::cout << "    Drift Field: " << gem[7] << " V/cm" << std::endl;
 std::cout << "    GEM Potential: " << gem[8] << " V\n" << std::endl;
}


void GainOneElectron(std::string folder, double info[9], bool plot = true,
                     int sizelimit = 10, int n_events = 100,
                     double electron_pos = 0.05, bool penning = true) {

  auto t_start = std::chrono::high_resolution_clock::now();

  // Input Confirmation
  std::cout << "Inputs for Gain Calculation:" << std::endl;
  std::cout << "    Show Histograms: " << plot << std::endl;
  std::cout << "    Number of Avalanches: " << n_events << std::endl;
  std::cout << "    Avalanche Size Limit: " << sizelimit << std::endl;
  std::cout << "    Penning Transfer: " << penning << std::endl;
  std::cout << "    Initial Electron Position in Z: " << electron_pos
            << " cm\n" << std::endl;

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
  if (penning) {
    const double rPenning = 0.57;
    const double lambdaPenning = 0.;
    gas -> EnablePenningTransfer(rPenning, lambdaPenning, "ar");
  }
  else {
    gas -> DisablePenningTransfer();
  }
  elm -> SetMedium(0, gas);


  // Sensor.
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(elm);
  sensor -> SetArea(-DIST, -H, Z_AXIS, DIST, H, electron_pos + 0.01);


  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  if (sizelimit > 0) {
    aval -> EnableAvalancheSizeLimit(sizelimit);
  }
  else {
    aval -> DisableAvalancheSizeLimit();
  }


  // Histograms
  int nBins = 100;
  float hmin = 1.;
  float hmax = 100.;

  TH1F* hRGain = new TH1F("hRGain", "Real Gain", nBins, hmin, hmax);
  TH1F* hEGain = new TH1F("hEGain", "Effective Gain", nBins, hmin, hmax);

  // Avalanches Calculations
  for (int i = n_events; i--;) {
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
      else {
        n_other += 1;
      }
    }


    hRGain -> Fill(np);
    hEGain -> Fill(nf);

    std::cout << "\n" << n_events -  i << "/" << n_events;
    std::cout << "\n... avalanche complete with " << np
              << " electron tracks. (number of electrons "
              << ne << ")" << std::endl;

    std::cout << "\n... effective electrons: " << nf << ", n_other: "
              << n_other << std::endl;
  }


  // Saving Histograms
  std::string title = folder + "_hists.root";
  const char* f_title = title.c_str();
  TFile* f = new TFile(f_title, "RECREATE");
  hRGain -> Write();
  hEGain -> Write();
  f -> Close();


  // Plotting
  if (plot == true) {
     TCanvas* cH = new TCanvas("cH", "Histograms", 800, 400);
     cH -> Divide(2,1);
     cH -> cd(1);
     hRGain -> Draw();
     cH -> cd(2);
     hEGain -> Draw();
   }




  auto t_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = t_end - t_start;
  std::cout << "\n\n... END OF GAIN CALCULATION: "
            << diff.count() << " s.\n"<< std::endl;

}

#endif
