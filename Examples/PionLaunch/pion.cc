// Example code for the analysis of a pion launched on a GEM detector
#include "../../GemSampa.hh"
#include <string>
#include "ViewFEMesh.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {


  double g[9];                  // GEM info list
  std::string GEM = "gem70_140_420";   // GEM folder
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


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(elm);
  sensor -> SetArea(-10 * g[1], -10 * H, Z_AXIS, 10 * g[1], 10 * H, z0);


  // Drift Visualization
  ViewDrift* vDrift = new ViewDrift();
  vDrift -> SetArea(-10 * g[1], -10 * H, Z_AXIS, 10 * g[1], 10 * H, z0);


  // Pion Track Setup
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);
  track -> SetParticle("pi");       // pion-
  track -> SetMomentum(1.E9);        // 1 GeV / c
  track -> EnableElectricField();
  track -> EnablePlotting(vDrift);


  // Avalanche Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> SetCollisionSteps(100);
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
  TCanvas *cGeo = new TCanvas("geo", "Geometry");
  ViewFEMesh* vFE = new ViewFEMesh();
  vFE -> SetCanvas(cGeo);
  vFE -> SetComponent(elm);
  vFE -> SetPlane(0, -1, 0, 0, 0, 0);
  vFE -> SetFillMesh(true);
  vFE -> SetColor(2, kCyan + 1);
  vFE -> SetColor(2, kOrange + 7);
  vFE -> SetColor(3, kOrange + 7);
  vFE -> EnableAxes();
  vFE -> SetXaxisTitle("x (cm)");
  vFE -> SetYaxisTitle("z (cm)");
  vFE -> SetArea(-10 * g[1], Z_AXIS, 0., 10 * g[1], z0, 0.);
  vFE -> SetViewDrift(vDrift);
  vFE -> Plot();
  cGeo -> SaveAs("pion.pdf");

}
