// Testing Triple-GEM detector properties
#include "../../GemSampa.hh"
#include "../../GemAnalysis.hh"
#include <string>
#include <vector>
#include <TCanvas.h>
#include "ViewFEMesh.hh"


//using namespace Garfield;

int main(int argc, char * argv[]) {
  std::string folder = "../triplegem";

  ComponentElmer *Elm = LoadGas(folder);

  double xmin = -0.034, xmax = 0.0034, zmin = -0.022, zmax = 0.022;
  double H = 0.012;


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(Elm);
  sensor -> SetArea(-10, -10, -0.309, 10, 10, 0.309);


  // Drift Visualization
  ViewDrift* vDrift = new ViewDrift();
  vDrift -> SetArea(-10, -10, -0.309, 10, 10, 0.309);

  // Avalanche Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(1000);
  aval -> EnablePlotting(vDrift);

  aval -> AvalancheElectron(0, 0, 0.3, 0, 0, 0., 0., 0.);
  std::cout << "Gain: " << aval -> GetNumberOfElectronEndpoints() << std::endl;

  // Visualization of Geometry
  TCanvas *cGeo = new TCanvas("geo", "Geometry");
  ViewFEMesh* vFE = new ViewFEMesh();

  vFE -> SetCanvas(cGeo);
  vFE -> SetComponent(Elm);
  vFE -> SetPlane(0, -1, 0, 0, 0.006, 0);
  vFE -> SetFillMesh(true);
  vFE -> SetColor(1, kCyan - 3);
  vFE -> SetColor(2, kOrange + 7);
  vFE -> SetColor(3, kOrange + 7);
  vFE -> EnableAxes();
  vFE -> SetXaxisTitle("x (cm)");
  vFE -> SetYaxisTitle("z (cm)");
  vFE -> SetArea(-0.05, -0.309, -1, 0.05, 0.309, 1);
  vFE -> SetViewDrift(vDrift);
  vFE -> Plot();

  cGeo -> SaveAs("aval.pdf");


  // Visualization of Fields
  TCanvas* cFie = new TCanvas("fie", "Field");
  ViewField* vF = new ViewField();

  vF -> SetCanvas(cFie);
  vF -> SetComponent(Elm);
  vF -> SetPlane(0, -1, 0, 0, H / 2, 0);
  vF -> SetArea(-0.05, -0.309, 0.05, 0.309);
  vF -> SetVoltageRange(0, -2100);
  vF -> PlotContour("e");

  cFie -> SaveAs("field.pdf");

  std::cout << "Gain: " << aval -> GetNumberOfElectronEndpoints() << std::endl;


}
