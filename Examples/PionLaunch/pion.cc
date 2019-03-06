// Example code for the analysis of a pion launched on a GEM detector
#include "../../GemSampa.hh"
#include <string>
#include <vector>
#include <fstream>
#include <TH2F.h>
#include <TCanvas.h>
#include "ViewFEMesh.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {
  //TApplication app("app", &argc, argv);
  clock_t begin_time = clock();

  double g[9];                  // GEM info list
  std::string GEM = "../gem70_140_420";   // GEM folder
  std::string TXTION = "ion.txt";    // Primary Ionization Output
  std::string TXTELE = "ele.txt";    // Readout Output

  // Files
  FILE* fion;     // Primary Ionization Output
  FILE* fele;     // Readout Output
  const char* f1 = TXTION.c_str();
  const char* f2 = TXTELE.c_str();
  fion = fopen(f1, "a");
  fele = fopen(f2, "a");


  // GEM Setup
  SetupInfo(g, 70., 140., 0.3, 0.1, 0.005, 0.0005, 4000., 1300., 420.);
  ComponentElmer* elm = LoadGas(GEM, 70.);    // 70% Ar, 30% CO2


  // Initial Parameters for Track
  const double Center = (g[2] - g[3]) / 2;
  const double Z_AXIS = -1 * (g[3] + g[4] / 2 + g[5]) - Center;
  const double H = sqrt(3) * g[1] / 2;
  // Position
  const double x0 = (2 * RndmUniform() - 1) * g[1] / 2;
  const double y0 = (2 * RndmUniform() - 1) * H / 2;
  const double z0 = g[4] / 2 + g[5] + g[2] - 0.001 - Center;
  // Time
  const double t0 = 0.;
  // Velocity vector (direction only)
  const double dx0 = RndmUniform() - 0.5;
  const double dy0 = RndmUniform() - 0.5;
  const double dz0 = -1;


  // Given the velocity vector and approximating the particle trajectory as a
  // line, we can reduce the volume in which tracks are calculated
  // (sensor area) for efficiency
  double delta = 3 * g[1];
  double tgx = fabs(dx0 / dz0);
  double tgy = fabs(dy0 / dz0);
  double xmax, xmin, ymax, ymin;
  if (dx0 >= 0.) {
    xmax = z0 * tgx + delta;
    xmin = -delta;
  }
  else {
    xmax = delta;
    xmin = -z0 * tgx - delta;
  }
  if (dy0 >= 0.) {
    ymax = z0 * tgy + delta;
    ymin = -delta;
  }
  else {
    ymax = delta;
    ymin = -z0 * tgy - delta;
  }


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(elm);
  sensor -> SetArea(xmin, ymin, Z_AXIS, xmax, ymax, z0);


  // Drift Visualization
  ViewDrift* vDrift = new ViewDrift();
  vDrift -> SetArea(xmin, ymin, Z_AXIS, xmax, ymax, z0);


  // Pion Track Setup
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);
  track -> SetParticle("pi");       // pion-
  track -> SetMomentum(1.E9);        // 1 GeV / c
  track -> EnableElectricField();
  track -> EnablePlotting(vDrift);
  // track -> EnableDebugging();


  // Avalanche Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(200);
  aval -> EnablePlotting(vDrift);


  // Cluster coordinates
  double xc, yc, zc, tc, extra;
  // Number of electrons produced in a collision and total
  int nc = 0, nsum = 0.;
  // Energy loss in a collision and total
  double ec = 0., esum = 0.;
  // Total number of electrons tracks
  int np = 0;

  track -> NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);

  // Loop over the clusters
  while (track -> GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
    esum += ec;
    nsum += nc;
    for (int j = 0; j < nc; j++) {
      // Properties of primary electron (pos, time, energy, velocity vector)
      double xe1, ye1, ze1, te1, e1, dxe, dye, dze;
      track -> GetElectron(j, xe1, ye1, ze1, te1, e1, dxe, dye, dze);

      // Saving primary ionization points on txt file
      // Energy and velocity are not provided by Heed, so they are ignored
      fprintf(fion, "%f;%f;%f;%f\n", xe1, ye1, ze1, te1);

      // Starts an avalanche for every primary electron, generating ne tracks
      aval -> AvalancheElectron(xe1, ye1, ze1, te1, 0, 0., 0., 0.);
      int ne = aval -> GetNumberOfElectronEndpoints();
      np += ne;

      for (int i = 0; i < ne; i++) {
        // Properties of electron (pos, time, energy)
        // Start of track
        double xe2, ye2, ze2, te2, e2;
        // End of track
        double xe3, ye3, ze3, te3, e3;
        int status;
        aval -> GetElectronEndpoint(i, xe2, ye2, ze2, te2, e2,
                                    xe3, ye3, ze3, te3, e3, status);
        fprintf(fele, "%f;%f;%f;%f;%f;%d\n", xe3, ye3, ze3, te3, e3, status);
      }
    }
  }


  fclose(fion);
  fclose(fele);

  std::cout << "\nNumber of Primary Electrons: " << nsum << std::endl;
  std::cout << "Particle Energy Loss: " << esum << std::endl;
  std::cout << "Number of Electron Tracks: " << np << std::endl;


  // Geometry and Track Visualization
  TCanvas *c1 = new TCanvas("tr", "Track");
  ViewFEMesh* vFE = new ViewFEMesh();
  vFE -> SetCanvas(c1);
  vFE -> SetComponent(elm);
  vFE -> SetPlane(0, -1, 0, 0, 0, 0);
  vFE -> SetFillMesh(true);
  vFE -> SetColor(2, kCyan + 1);
  vFE -> SetColor(2, kOrange + 7);
  vFE -> SetColor(3, kOrange + 7);
  vFE -> EnableAxes();
  vFE -> SetXaxisTitle("x (cm)");
  vFE -> SetYaxisTitle("z (cm)");
  vFE -> SetArea(xmin - delta, Z_AXIS, 0., xmax + delta, z0, 0.);
  vFE -> SetViewDrift(vDrift);
  vFE -> Plot();
  c1 -> SaveAs("pion.pdf");

  //app.Run(kTRUE);


  // Position Histogram
  std::ifstream File;
  std::string line;
  File.open(f2);
  std::vector<double> PosX;
  std::vector<double> PosY;
  double px, py, pz;

  while (!File.eof()) {
    File >> line;
    size_t stop1 = line.find(";");
    size_t stop2 = line.find(";", stop1 + 1);
    size_t stop3 = line.find(";", stop2 + 1);
    px = atof(line.substr(0, stop1).c_str());
    py = atof(line.substr(stop1 + 1, stop2 - stop1 - 1).c_str());
    pz = atof(line.substr(stop2 + 1, stop3 - stop2 - 1).c_str());
    if (pz == Z_AXIS) {
      PosX.push_back(px);
      PosY.push_back(py);
    }
  }

  float read = 0.005;   // 50 micrometers
  float nBinsx = (xmax - xmin) / read;
  float nBinsy = (ymax - ymin) / read;
  TH2F* hp = new TH2F("hp", "", nBinsx, xmin, xmax, nBinsy, ymin, ymax);

  for (int i = 0; i < PosX.size(); i++) {
    hp -> Fill(PosX[i], PosY[i]);
  }
  TCanvas *c2 = new TCanvas("h", "Position");
  hp -> Draw("colz");
  c2 -> SaveAs("hist.pdf");


  std::cout << "Exec Time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;
}
