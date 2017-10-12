#include <iostream>
#include <fstream>
#include <chrono>

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>

#include "ComponentElmer.hh"
#include "MediumMagboltz.hh"
#include "Sensor.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"
#include "Plotting.hh"
#include "ViewFEMesh.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {

  auto t_start = std::chrono::high_resolution_clock::now();
  TApplication app("app", &argc, argv);

  // GEM Dimensions in cm
  const double T_DIE = 0.005;              // Dieletric Thickness
  const double T_PLA = 0.0005;             // Plates Thickness
  const double DIST = 0.014;              // Distance Between Holes
  const double H = sqrt(3) * DIST / 2;     // Height
  const double D_E = 0.1;                  // Distance to Electrode
  const double D_P = 0.1;                  // Distance to Pad

  // Primary Electron Initial Position
  double x0 = 0.;
  double y0 = 0.;
  double z0 = 0.02;

  // Import
  ComponentElmer* elm = new ComponentElmer("gem/mesh.header",
                                           "gem/mesh.elements",
                                           "gem/mesh.nodes",
                                           "gem/dielectrics.dat",
                                           "gem/gem.result", "mm");
  elm -> EnablePeriodicityX();
  elm -> EnableMirrorPeriodicityY();
  elm -> PrintRange();

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
  //const double rPenning = 0.51;
  //const double lambdaPenning = 0.;
  //gas -> EnablePenningTransfer(rPenning, lambdaPenning, "ar");
  elm -> SetMedium(0, gas);

  // Ion Mobility
  const std::string path = getenv("GARFIELD_HOME");
  gas -> LoadIonMobility(path + "/Data/IonMobility_Ar+_Ar.txt");

  // Sensor.
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(elm);
  sensor -> SetArea(-5 * DIST, -5 * DIST, -0.02, 5 * DIST, 5 * DIST, z0);

  // Avalanche and Drift Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);

  AvalancheMC* drift = new AvalancheMC();
  drift -> SetSensor(sensor);
  drift -> SetDistanceSteps(2.e-4);

  // View Drift
  ViewDrift* driftView = new ViewDrift();
  driftView -> SetArea(-2 * DIST, -2 * DIST, -D_P, 2 * DIST, 2 * DIST, z0);
  aval -> EnablePlotting(driftView);
  drift -> EnablePlotting(driftView);

  // Avalanche Calculation
  double xe1, ye1, ze1, te1, e1;
  double xe2, ye2, ze2, te2, e2;
  double xi1, yi1, zi1, ti1;
  double xi2, yi2, zi2, ti2;
  int status;
  aval -> AvalancheElectron(x0, y0, z0, 0, 0, 0., 0., 0.);
  const int np = aval -> GetNumberOfElectronEndpoints();

  for (int i = np; i--;) {
    aval -> GetElectronEndpoint(i, xe1, ye1, ze1, te1,
                                e1, xe2, ye2, ze2, te2,
                                e2, status);
    drift -> DriftIon(xe1, ye1, ze1, te1);
  }
  std::cout << "\n... avalanche complete with " <<
  np << " electron tracks." << std::endl;
/*
  // Plotting
  TCanvas* ue = new TCanvas();
  driftView -> SetCanvas(ue);
  driftView -> Plot();

  // Visualization of Geometry
  TCanvas *cGeo = new TCanvas("geo", "Geometry");
  ViewFEMesh* vFE = new ViewFEMesh();

  vFE -> SetCanvas(cGeo);
  vFE -> SetComponent(elm);
  vFE -> SetPlane(0, -1, 0, 0, H / 2, 0);
  vFE -> SetFillMesh(true);
  vFE -> SetColor(1, kCyan - 3);
  vFE -> SetColor(2, kOrange + 7);
  vFE -> SetColor(3, kOrange + 7);
  vFE -> EnableAxes();
  vFE -> SetXaxisTitle("x (cm)");
  vFE -> SetYaxisTitle("z (cm)");
  vFE -> SetArea(-1.5 * DIST, -3 * T_DIE, 0., 1.5 * DIST, z0, 0.);
  vFE -> SetViewDrift(driftView);
  vFE -> Plot();*/

  auto t_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = t_end - t_start;
  std::cout << "\nEND OF SIMULATION: " << diff.count() << " s.\n"<< std::endl;

  app.Run(kTRUE);
}
