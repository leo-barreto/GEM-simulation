// Example code for signal calculation
#include "../../GemSampa.hh"
#include "ViewSignal.hh"
#include "ViewSignal.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {
  clock_t begin_time = clock();

  std::string GEM = "../gem_example";   // GEM folder


  // GEM Setup
  ComponentElmer* elm = LoadGas(GEM, 70.);    // 70% Ar, 30% CO2
  double ZMAX = 0.103;
  double delta = 0.001;


  // Position
  double x0 = 0.014 * (2 * RndmUniform() - 1);
  double y0 = 0.014 * (2 * RndmUniform() - 1);
  const double z0 = ZMAX - delta;
  // Time
  const double t0 = 0., tEnd = 120.;
  const int nTimeBins = 100;
  // Velocity vector (direction only)
  const double dx0 = RndmUniform() - 0.5;
  const double dy0 = RndmUniform() - 0.5;
  const double dz0 = -1;


  // Sensor
  Sensor* sensor = new Sensor();
  sensor -> AddComponent(elm);
  sensor -> AddElectrode(elm, "WT");
  sensor -> SetArea(-10, -10, -ZMAX, 10, 10, ZMAX);
  sensor -> SetTimeWindow(t0, (tEnd - t0) / nTimeBins, nTimeBins);

  // Pion Track Setup
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);
  track -> SetParticle("pi");       // pion-
  track -> SetMomentum(1.E9);        // 1 GeV / c



  // Avalanche Setup
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);
  aval -> EnableSignalCalculation();


  // Cluster properties
  double xc, yc, zc, tc, ec, extra;
  // Number of electrons produced
  int nc = 0, nf = 0;

  track -> NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);

  // Loop over the clusters
  while (track -> GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
    for (int j = 0; j < nc; j++) {
      // Properties of primary electron (pos, time, energy, velocity vector)
      double xe1, ye1, ze1, te1, e1, dxe, dye, dze;
      track -> GetElectron(j, xe1, ye1, ze1, te1, e1, dxe, dye, dze);

      // Starts an avalanche for every primary electron, generating ne tracks
      aval -> AvalancheElectron(xe1, ye1, ze1, te1, 0., 0., 0., 0.);
      int ne = aval -> GetNumberOfElectronEndpoints();

      for (int k = 0; k < ne; k++) {
        // Properties of electron (pos, time, energy)
        // Start of track
        double xe2, ye2, ze2, te2, e2;
        // End of track
        double xe3, ye3, ze3, te3, e3;
        int status;
        aval -> GetElectronEndpoint(k, xe2, ye2, ze2, te2, e2,
                                    xe3, ye3, ze3, te3, e3, status);
        if (ze3 <= -ZMAX + delta) {  // Added a delta to minimize border effects
          nf += 1;
        }
      }
    }
  }

  // View Signal
  TCanvas *c1 = new TCanvas("c", "Current");
  ViewSignal* signalView = new ViewSignal();
  signalView -> SetSensor(sensor);
  signalView -> SetCanvas(c1);
  signalView -> PlotSignal("WT");

  c1 -> SaveAs("signal.pdf");

  // Integrate current to get total charge on pad
  double sum = 0.;
  for (int i = 0; i < nTimeBins; i++) {
    double wt = -1 * sensor -> GetSignal("WT", i) / ElementaryCharge;
    sum += wt;
  }
  std::cout << "\n\nNumber of Electrons: " << nf << std::endl;
  std::cout << "Integrated Signal Final: " << sum << std::endl;

  std::cout << "Exec Time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;

}
