#include "../GemSampa.hh"

#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>

using namespace std;

int main(int argc, char * argv[]) {
  TApplication app("app", &argc, argv);

  string txt = "gainhole";
  string hR = "hRGain_h";
  string hE = "hEGain_h";
  int N = 12;
  double diam[N] = {20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120., 130.};
  double erdiam[N] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
  double rg[N], eg[N], err[N], ere[N];

  // ROOT File
  ReadTXTGain(txt);
  TFile* f = new TFile("gainhole_hists.root");


  for (int i = 0; i < N; i++) {
    int d = diam[i];
    string hRi = hR + to_string(d);
    string hEi = hE + to_string(d);
    TH1I* h1 = (TH1I*)f -> Get(hRi.c_str());
    TH1I* h2 = (TH1I*)f -> Get(hEi.c_str());

    double m1, m2, s1, s2;
    m1 = h1 -> GetMean();
    m2 = h2 -> GetMean();
    s1 = h1 -> GetStdDev() / sqrt(h1 -> GetEntries());
    s2 = h2 -> GetStdDev() / sqrt(h2 -> GetEntries());

    cout << "\nFor h = " << d << ":" << endl;
    cout << "Real Gain: " << m1 << "(" << s1 << ")" << endl;
    cout << "Effective Gain: " << m2 << "(" << s2 << ")" << endl;
    cout << "Numero de Dados: " << h1 -> GetEntries() << endl;

    rg[i] = m1;
    eg[i] = m2;
    err[i] = s1;
    ere[i] = s2;

    //perc.push_back(m2 / m1);
    //errperc.push_back(sqrt((s2 / m1) ** 2 + (s1 * m2 / (m1 ** 2) ** 2));

  }

  // Graph Properties
  TCanvas* c = new TCanvas();
  c -> SetLogy();
  TMultiGraph *g = new TMultiGraph();

  TGraphErrors* realg = new TGraphErrors(N, diam, rg, erdiam, err);
  TGraphErrors* effeg = new TGraphErrors(N, diam, eg, erdiam, ere);

  realg -> SetMarkerColor(kRed);
  effeg -> SetMarkerColor(kBlue);
  realg -> SetMarkerStyle(21);
  effeg -> SetMarkerStyle(21);

  g -> Add(realg);
  g -> Add(effeg);
  g -> SetTitle("Ganho para diferentes Geometrias (#DeltaV_{GEM} = 500V)");
  g -> Draw("ap");
  g -> GetXaxis() -> SetTitle("Di#hat{a}metro [#mum]");
  g -> GetYaxis() -> SetTitle("Ganho");

  // Legend
  TLegend *leg = new TLegend(0.1, 0.8, 0.3, 0.9);
  leg -> AddEntry(realg, "Ganho Real", "p");
  leg -> AddEntry(effeg, "Ganho Efetivo", "p");
  leg -> Draw();

  cout << "\nDone!" << endl;
  app.Run(kTRUE);
}
