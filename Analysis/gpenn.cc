#include "../GemAnalysis.hh"


using namespace std;

int main(int argc, char * argv[]) {
  TApplication app("app", &argc, argv);
  ReadTXTGain("penn");
  TFile* f1 = new TFile("penn_hists.root");

  TH1I* h1 = (TH1I*)f1 -> Get("hRGain_h70");
  TH1I* h2 = (TH1I*)f1 -> Get("hRGain_nopenning");
  //h1 -> GetXaxis() -> SetRangeUser(0., 5000.);
  //h1 -> Draw();
  h2 -> Draw();

  cout << "\nDone!" << endl;
  app.Run(kTRUE);
}
