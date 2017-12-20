#include <iostream>
#include <fstream>
#include <chrono>

#include <TApplication.h>
#include <TMath.h>

#include "ComponentElmer.hh"
#include "MediumMagboltz.hh"
#include "Sensor.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"
#include "Random.hh"


using namespace Garfield;

int main(int argc, char * argv[]) {

  auto t_start = std::chrono::high_resolution_clock::now();
  TApplication app("app", &argc, argv);

  // GEM Dimensions in cm
  const double T_DIE = 0.005;              // Dieletric Thickness
  const double T_PLA = 0.0005;             // Plates Thickness
  const double DIST = 0.014;               // Distance Between Holes
  const double H = sqrt(3) * DIST / 2;     // Height
  const double D_E = 0.1;                  // Distance to Electrode
  const double D_P = 0.1;                  // Distance to Pad

  const double Z_AXIS = -1 * (D_P + T_DIE / 2 + T_PLA);
  const double Z0 = 0.05;
  const int N_AVAL = 10;


  // Import
  ComponentElmer* elm = new ComponentElmer("gem70_140_500/mesh.header",
                                           "gem70_140_500/mesh.elements",
                                           "gem70_140_500/mesh.nodes",
                                           "gem70_140_500/dielectrics.dat",
                                           "gem70_140_500/gem.result", "mm");
  elm -> EnablePeriodicityX();
  elm -> EnableMirrorPeriodicityY();
  //elm -> SetWeightingField("gem70_140_500/gemWT.result", "wt");

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
  sensor -> SetArea(-DIST, -H, Z_AXIS, DIST, H, Z0 + 0.01);

  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> EnableAvalancheSizeLimit(10);

  // Avalanches Calculations
  std::vector<int> number_p = {};
  std::vector<int> number_e = {};
  std::vector<int> number_i = {};
  for (int i = N_AVAL; i--;) {
    // Random Initial Positions
    double x0 = (2 * RndmUniform() - 1) * DIST / 2;
    double y0 = (2 * RndmUniform() - 1) * DIST / 2;
    aval -> AvalancheElectron(x0, y0, Z0, 0, 0, 0., 0., 0.);
    int np = aval -> GetNumberOfElectronEndpoints();
    int ne = 0, ni = 0;
    aval -> GetAvalancheSize(ne, ni);

    std::cout << "\n" << N_AVAL -  i << "/" << N_AVAL;
    std::cout << "\n... avalanche complete with " <<
              np << " electron tracks. (number of electrons " << ne << ")" << std::endl;
    number_p.push_back(np);
    number_e.push_back(ne);
    number_i.push_back(ni);
  }

 double mean_p = TMath::Mean(number_p.begin(), number_p.end());
 std::cout << "\n\n... Mean End Points: " << mean_p << std::endl;

 double mean_e = TMath::Mean(number_e.begin(), number_e.end());
 std::cout << "\n... Mean Electron Number: " << mean_e << std::endl;

 double mean_i = TMath::Mean(number_i.begin(), number_i.end());
 std::cout << "\n... Mean Ion Number: " << mean_i << std::endl;

 auto t_end = std::chrono::high_resolution_clock::now();
 std::chrono::duration<double> diff = t_end - t_start;
 std::cout << "\n... END OF SIMULATION: " << diff.count() << " s.\n"<< std::endl;

 app.Run(kTRUE);
}
