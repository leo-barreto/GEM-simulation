
#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <chrono>

#include <TCanvas.h>
#include <TApplication.h>
#include <TFile.h>

#include "MediumMagboltz.hh"
#include "ComponentElmer.hh"
#include "Sensor.hh"
#include "ViewField.hh"
#include "Plotting.hh"
#include "ViewFEMesh.hh"
#include "ViewSignal.hh"
#include "GarfieldConstants.hh"
#include "Random.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"


using namespace Garfield;


int main(int argc, char * argv[]) {

    auto t_start = std::chrono::high_resolution_clock::now();
    TApplication app("app", &argc, argv);

    // GEM Dimensions in cm
    const double T_DIE = 0.005;              // Dieletric Thickness
    const double T_PLA = 0.0005;             // Plates Thickness
    const double DIST = 0.0140;              // Distance Between Holes
    const double H = sqrt(3) * DIST / 2;     // Height
    const double D_E = 0.3;                  // Distance to Electrode
    const double D_P = 0.1;                  // Distance to Pad

    const double Eind = 4000;                // Induction Field [V/cm]
    const double Edrift = 100;               // Drift Field [V/cm]
    const double dV = 300;                   // GEM Potential Difference [V]


    // Electron Start Parameters
    const double zi = 0.;
    double xi = 0.;
    double yi = H / 2;


    // Import from Elmer
    ComponentElmer* elm = new ComponentElmer("gem70_140_400/mesh.header",
                                             "gem70_140_400/mesh.elements",
                                             "gem70_140_400/mesh.nodes",
                                             "gem70_140_400/dielectrics.dat",
                                             "gem70_140_400/gem.result", "mm");

    elm -> EnablePeriodicityX();
    elm -> EnableMirrorPeriodicityY();
    elm -> SetWeightingField("gem70_140_400/gemWT.result", "wtlel");
    elm -> PrintRange();


    // Visualization of Fields
    TCanvas* cFie = new TCanvas("fie", "Field");
    ViewField* vF = new ViewField();

    vF -> SetCanvas(cFie);
    vF -> SetComponent(elm);
    vF -> SetPlane(0, -1, 0, 0, H / 2, 0);
    vF -> SetArea(-0.5 * DIST, 0.19, 0.5 * DIST, 0.21);
    //vF -> SetNumberOfContours(80);
    //vF -> SetNumberOfSamples2d(60, 60);
    vF -> SetVoltageRange(-1500., -300.);
    vF -> PlotContour("v");


    // Medium
    const std::string path = getenv("GARFIELD_HOME");
    const double rPenning = 0.51;                   // ué?
    const double lambdaPenning = 0.;
    MediumMagboltz* gas = new MediumMagboltz();

    gas -> SetTemperature(293.15);                  // Temperature [K]
    gas -> SetPressure(740.);                       // Pressure [Torr]
    gas -> EnableDrift();
    gas -> SetComposition("ar", 70., "co2", 30.);   // Ar/CO2 70:30
    gas -> SetMaxElectronEnergy(200.);              // Energy [eV]
    gas -> EnableDebugging();
    gas -> Initialise();
    gas -> DisableDebugging();
    gas -> LoadIonMobility(path + "/Data/IonMobility_Ar+_Ar.txt");
    gas -> EnablePenningTransfer(rPenning, lambdaPenning, "ar");
    elm -> SetMedium(0, gas);
    elm -> PrintMaterials();


    // Sensor
    const double TIME = 500.;
    const int BINS = 500;
    Sensor* sensor = new Sensor();

    sensor -> AddComponent(elm);
    sensor -> SetArea(-DIST, -H / 2, -3 * T_DIE,
                      DIST, H / 2, zi + T_PLA);
    sensor -> AddElectrode(elm, "wtlel");
    sensor -> SetTimeWindow(0., TIME / BINS, BINS);


    // Avalanche Set Up
    AvalancheMicroscopic* aval = new AvalancheMicroscopic();

    aval -> SetSensor(sensor);
    aval -> SetCollisionSteps(100);
    aval -> EnableSignalCalculation();

    AvalancheMC* drift = new AvalancheMC();

    drift -> SetSensor(sensor);
    drift -> SetDistanceSteps(2.e-4);


    // Drift Visualization
    ViewDrift* vDrift = new ViewDrift();

    vDrift -> SetArea(-DIST, -D_P, -H / 2, DIST, zi, H / 2);
    aval -> EnablePlotting(vDrift);
    drift -> EnablePlotting(vDrift);

    TCanvas* dri = new TCanvas("dri", "Drift");
    vDrift -> SetCanvas(dri);
    vDrift -> Plot();


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
    vFE -> SetArea(-1.5 * DIST, -3 * T_DIE, 0., 1.5 * DIST, 3 * T_DIE, 0.);


    // Avalanche Calculation
    aval -> AvalancheElectron(xi, yi, zi, 0., 0., 0., 0., 0.);
    std::cout << "\n... avalanche complete with " <<
    aval -> GetNumberOfElectronEndpoints() << " electron tracks." << std::endl;


    // Plotting
    vFE -> SetViewDrift(vDrift);
    vFE -> Plot();




    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t_end - t_start;
    std::cout << "\nEND OF SIMULATION: " << diff.count() << " s.\n"<< std::endl;

    app.Run(kTRUE);

    return 0;
}
