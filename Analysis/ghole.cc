#include "../GemSampa.hh"

using namespace std;

int main(int argc, char * argv[]) {
  TApplication app("app", &argc, argv);

  string txt = "gainhole";
  //ReadTXTGain(txt);

  TFile* f = new TFile("gainhole_hists.root");
  TH1I* h1 = (TH1I*)f -> Get("hRGain_h70");
  TH1I* h2 = (TH1I*)f -> Get("hEGain_h70");

  float m1, m2, s1, s2;
  m1 = h1 -> GetMean();
  m2 = h2 -> GetMean();
  s1 = h1 -> GetStdDev() / sqrt(h1 -> GetEntries());
  s2 = h2 -> GetStdDev() / sqrt(h2 -> GetEntries());

  cout << "Real Gain: " << m1 << "(" << s1 << ")" << endl;
  cout << "Effective Gain: " << m2 << "(" << s2 << ")" << endl;

  app.Run(kTRUE);
}
