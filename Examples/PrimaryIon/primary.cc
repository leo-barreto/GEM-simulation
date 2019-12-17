// Analysis of primary ionization of a 5.9 keV launch and its consequent gain
#include "../../GemSampa.hh"
#include "../../GemAnalysis.hh"

using namespace Garfield;
int main(int argc, char * argv[]) {

  int AVALCALC = 0;          // True for avalanche calculations
  std::string GEM = "gem_ion";
  std::string rootname = "results.root";
  double DRIFT = 0.3;    // Drift zone size
  double ZMAX = 0.203;   // Starting position of photon (pad is at -ZMAX)
  double delta = 0.001;
  int NEVENTS = 10000;
  // This calculation is made for a fixed energy, if you desire to simulate
  // photons from a source Fe55, use the function Fe55()
  double ENERGY = 5900.;


  // Creation of histograms
  TH1::StatOverflows(kTRUE);
  TH2::StatOverflows(kTRUE);
  TH1F* hEle = new TH1F("hEle", "Number of Primary Electrons", 100, 0, 300);
  TH1F* hZ = new TH1F("hZ", "Z Primary Ion", 100, -ZMAX, ZMAX);
  TH2F* hP = new TH2F("hP", "Position Primary Ion", 50, -0.3, .3, 50, -0.3, .3);
  TH1F* hRG = new TH1F("hGR", "Real Gain", 100, 0., 350.);
  TH1F* hEG = new TH1F("hER", "Effective Gain", 100, 0., 150.);
  TH1F* hRes = new TH1F("hRes", "Energy Resolution", 100, 0., 25000.);

  FILE* file;
  const char* f_title = "gain.txt";


  // Setup Sensor
  ComponentElmer* elm = LoadGas(GEM, 70.);

  Sensor* sensor = new Sensor();
  sensor -> AddComponent(elm);
  sensor -> SetArea(-5, -5, -ZMAX, 5, 5, ZMAX); // 10 cm each side

  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval -> SetSensor(sensor);


  // Setup Track
  TrackHeed* track = new TrackHeed();
  track -> SetSensor(sensor);

  double radius_beam = 0.1;
  double X0 = 0.014 * (2 * RndmUniform() - 1);
  double Y0 = 0.014 * (2 * RndmUniform() - 1);
  std::cout << "The beam is localized at (" << X0 << ", " << Y0 << ")" << std::endl;

  for (int i = NEVENTS; i--;) {
    std::cout << "\nPhoton launch: "<< i << "/" << NEVENTS << std::endl;
    double x0 = X0 + radius_beam * (2 * RndmUniform() - 1);
    double y0 = Y0 + sqrt(pow(radius_beam, 2) - pow(x0, 2)) * (2 * RndmUniform() - 1);

    int ntot = 0, nel = 0;    //  Number of total detected electrons and primary electrons

    while (nel == 0) {
      track -> TransportPhoton(x0, y0, ZMAX - delta, 0., ENERGY, 0, 0, -1, nel);
    }

    hEle -> Fill(nel);

    for (int j = 0; j < nel; j++) {
      std::cout << "Primary: "<< j << "/" << nel << std::endl;
      // Properties of electron (pos, time, energy, velocity vector)
      double xe1, ye1, ze1, te1, e1, dxe, dye, dze;
      track -> GetElectron(j, xe1, ye1, ze1, te1, e1, dxe, dye, dze);
      hZ -> Fill(ze1);
      hP -> Fill(xe1, ye1);

      if (AVALCALC == 1) {
        aval -> AvalancheElectron(xe1, ye1, ze1, te1, e1, dxe, dye, dze);
        int np = aval -> GetNumberOfElectronEndpoints();
        hRG -> Fill(np);

        // Final Positions Analysis
        double xe2, ye2, ze2, te2, e2;
        double xe3, ye3, ze3, te3, e3;
        int status, nf = 0;
        for (int k = np; k--;) {
          aval -> GetElectronEndpoint(k, xe2, ye2, ze2, te2, e2,
                                      xe3, ye3, ze3, te3, e3, status);

          if (ze3 <= -ZMAX + delta) {  // Added a delta to minimize border effects
            nf += 1;
          }
        }
        hEG -> Fill(nf);
        ntot += nf;
        std::cout << "Effective Gain: " << nf << std::endl;
        file = fopen(f_title, "a");
        fprintf(file, "%d;%d;0\n", np, nf);
        fclose(file);
      }
    }
    hRes -> Fill(ntot);
  }


  // Save histograms
  TFile* f = new TFile(rootname.c_str(), "RECREATE");
  hEle -> Write();
  hZ -> Write();
  hP -> Write();
  if (AVALCALC == 1) {
    hRG -> Write();
    hEG -> Write();
    hRes -> Write();
  }

  // Draw results
  TCanvas *c1 = new TCanvas("hEle", "Primary Electrons");
  hEle -> Draw();
  c1 -> SaveAs("primaryele.pdf");

  TCanvas *c2 = new TCanvas("hZ", "Z Position");
  hZ -> Draw();
  c2 -> SaveAs("zpos.pdf");

  TCanvas *c3 = new TCanvas("hP", "Position");
  hP -> Draw("colz");
  c3 -> SaveAs("xypos.pdf");

  if (AVALCALC == 1) {
    TCanvas *c4 = new TCanvas("hRG", "Real Gain");
    hRG -> Draw();
    c4 -> SaveAs("rgain.pdf");

    TCanvas *c5 = new TCanvas("hEG", "Effective Gain");
    hEG -> Draw();
    c5 -> SaveAs("egain.pdf");

    TCanvas *c6 = new TCanvas("hRes", "Energy Resolution");
    hRes -> Draw();
    c6 -> SaveAs("energyres.pdf");
  }

  f -> Close();



}
