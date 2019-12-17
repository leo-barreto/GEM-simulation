// Testing Triple-GEM detector properties
#include "../../GemSampa.hh"
#include "../../GemAnalysis.hh"
#include <string>
#include <vector>
#include <TCanvas.h>
#include "ViewFEMesh.hh"


using namespace Garfield;

int main(int argc, char * argv[]) {
  std::string folder = "3gem_example";

  ComponentElmer *Elm = LoadGas(folder);
  double ZMAX = 0.209;
  double delta = 0.001;


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(Elm);
  sensor -> SetArea(-10, -10, -ZMAX, 10, 10, ZMAX);


  // Drift Visualization
  ViewDrift* vDrift = new ViewDrift();
  vDrift -> SetArea(-10, -10, -ZMAX, 10, 10, ZMAX);

  // Avalanche Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(1000);
  aval -> EnablePlotting(vDrift);
  aval -> EnableAvalancheSizeLimit(200);

  aval -> AvalancheElectron(0, 0, ZMAX - delta, 0, 0, 0., 0., 0.);
  std::cout << "Gain: " << aval -> GetNumberOfElectronEndpoints() << std::endl;

  // Visualization of Geometry
  TCanvas *cGeo = new TCanvas("geo", "Geometry");
  ViewFEMesh* vFE = new ViewFEMesh();

  vFE -> SetCanvas(cGeo);
  vFE -> SetComponent(Elm);
  vFE -> SetPlane(0, -1, 0, 0, 0, 0);
  vFE -> SetFillMesh(true);
  vFE -> SetColor(0, kCyan - 3);
  vFE -> SetColor(1, kOrange + 7);
  vFE -> SetColor(2, kOrange + 7);
  vFE -> EnableAxes();
  vFE -> SetXaxisTitle("x (cm)");
  vFE -> SetYaxisTitle("z (cm)");
  vFE -> SetArea(-0.1, -ZMAX, 0, 0.1, ZMAX, 0);
  vFE -> SetViewDrift(vDrift);
  vFE -> Plot();

  cGeo -> SaveAs("aval.png");


  // Visualization of Fields
  TCanvas* cFie = new TCanvas("fie", "Field");
  ViewField* vF = new ViewField();

  vF -> SetCanvas(cFie);
  vF -> SetComponent(Elm);
  vF -> SetPlane(0, -1, 0, 0, 0, 0);
  vF -> SetArea(-0.02, -0.01, 0.02, 0.01);
  vF -> SetVoltageRange(0, -2100);
  vF -> PlotContour("e");

  cFie -> SaveAs("field.png");

  std::cout << "Gain: " << aval -> GetNumberOfElectronEndpoints() << std::endl;


}
