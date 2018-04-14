#ifndef GEMSAMPA_H
#define GEMSAMPA_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

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

void SetupInfo(double gem[9], std::string folder, double diam, double dist,
               double up, double low, double t_die, double t_pla,
               double i_field, double d_field, double potential) {


 // Dimensions in cm, Fields in V/cm and Potentials in
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



void GainOneElectron(std::string folder, double info[9],
                     std::string txtfile, bool plot = true,
                     int sizelimit = 10, int n_events = 100,
                     double electron_pos = 0.05, bool penning = true) {


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


/*
void ReadTXTGain(std::string txtfolder, const char* rootname) {

  // ROOT File
  TFile* f = new TFile(rootname, "NEW");
  float r, e, t;

  for i in





  f -> Close();



}*/



#endif
