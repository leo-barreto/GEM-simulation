#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <TApplication.h>
#include "../GemSim.hh"
#include <TCanvas.h>
#include <TGraphErrors.h>


using namespace std;

int main(int argc, char * argv[]) {

  auto t_start = std::chrono::high_resolution_clock::now();
  TApplication app("app", &argc, argv);

  double g[9];
  string S1 = "gem20_140_500";
  string S2 = "gem30_140_500";
  string S3 = "gem40_140_500";
  string S4 = "gem50_140_500";
  string S5 = "gem60_140_500";
  string S6 = "gem70_140_500";
  string S7 = "gem80_140_500";
  string S8 = "gem90_140_500";
  string S9 = "gem100_140_500";
  string S10 = "gem110_140_500";
  string S11 = "gem120_140_500";
  string S12 = "gem130_140_500";

  /*SetupInfo(g, S1, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S1, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S2, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S2, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S3, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S3, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S4, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S4, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S5, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S5, g, false, 20, 100, 0.05, true);

  SetupInfo(g, S6, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S6, g, false, 20, 100, 0.05, true);

  SetupInfo(g, S7, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S7, g, false, 20, 100, 0.05, true);

  SetupInfo(g, S8, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S8, g, false, 20, 100, 0.05, true);

  SetupInfo(g, S9, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S9, g, false, 20, 100, 0.05, true);

  SetupInfo(g, S10, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S10, g, false, 20, 100, 0.05, true);

  SetupInfo(g, S11, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S11, g, false, 20, 100, 0.05, true);

  SetupInfo(g, S12, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S12, g, false, 20, 100, 0.05, true);*/

  TFile* f1 = new TFile("gem20_140_500_hists.root");
  TH1F* hRGain1 = (TH1F*)f1 -> Get("hRGain");
  TH1F* hEGain1 = (TH1F*)f1 -> Get("hEGain");

  TFile* f2 = new TFile("gem30_140_500_hists.root");
  TH1F* hRGain2 = (TH1F*)f2 -> Get("hRGain");
  TH1F* hEGain2 = (TH1F*)f2 -> Get("hEGain");

  TFile* f3 = new TFile("gem40_140_500_hists.root");
  TH1F* hRGain3 = (TH1F*)f3 -> Get("hRGain");
  TH1F* hEGain3 = (TH1F*)f3 -> Get("hEGain");

  TFile* f4 = new TFile("gem50_140_500_hists.root");
  TH1F* hRGain4 = (TH1F*)f4 -> Get("hRGain");
  TH1F* hEGain4 = (TH1F*)f4 -> Get("hEGain");

  TFile* f5 = new TFile("gem60_140_500_hists.root");
  TH1F* hRGain5 = (TH1F*)f5 -> Get("hRGain");
  TH1F* hEGain5 = (TH1F*)f5 -> Get("hEGain");

  TFile* f6 = new TFile("gem70_140_500_hists.root");
  TH1F* hRGain6 = (TH1F*)f6 -> Get("hRGain");
  TH1F* hEGain6 = (TH1F*)f6 -> Get("hEGain");

  TFile* f7 = new TFile("gem80_140_500_hists.root");
  TH1F* hRGain7 = (TH1F*)f7 -> Get("hRGain");
  TH1F* hEGain7 = (TH1F*)f7 -> Get("hEGain");

  TFile* f8 = new TFile("gem90_140_500_hists.root");
  TH1F* hRGain8 = (TH1F*)f8 -> Get("hRGain");
  TH1F* hEGain8 = (TH1F*)f8 -> Get("hEGain");

  TFile* f9 = new TFile("gem100_140_500_hists.root");
  TH1F* hRGain9 = (TH1F*)f9 -> Get("hRGain");
  TH1F* hEGain9 = (TH1F*)f9 -> Get("hEGain");

  TFile* f10 = new TFile("gem110_140_500_hists.root");
  TH1F* hRGain10 = (TH1F*)f10 -> Get("hRGain");
  TH1F* hEGain10 = (TH1F*)f10 -> Get("hEGain");

  TFile* f11 = new TFile("gem120_140_500_hists.root");
  TH1F* hRGain11 = (TH1F*)f11 -> Get("hRGain");
  TH1F* hEGain11 = (TH1F*)f11 -> Get("hEGain");

  TFile* f12 = new TFile("gem130_140_500_hists.root");
  TH1F* hRGain12 = (TH1F*)f12 -> Get("hRGain");
  TH1F* hEGain12 = (TH1F*)f12 -> Get("hEGain");



  double mr[12], me[12], sr[12], se[12];
  mr[0] = hRGain1 -> GetMean(); cout << mr[0] << endl;
  mr[1] = hRGain2 -> GetMean(); cout << mr[1] << endl;
  mr[2] = hRGain3 -> GetMean(); cout << mr[2] << endl;
  mr[3] = hRGain4 -> GetMean(); cout << mr[3] << endl;
  mr[4] = hRGain5 -> GetMean(); cout << mr[4] << endl;
  mr[5] = hRGain6 -> GetMean(); cout << mr[5] << endl;
  mr[6] = hRGain7 -> GetMean(); cout << mr[6] << endl;
  mr[7] = hRGain8 -> GetMean(); cout << mr[7] << endl;
  mr[8] = hRGain9 -> GetMean(); cout << mr[8] << endl;
  mr[9] = hRGain10 -> GetMean(); cout << mr[9] << endl;
  mr[10] = hRGain11 -> GetMean(); cout << mr[10] << endl;
  mr[11] = hRGain12 -> GetMean(); cout << mr[11] << endl;

  sr[0] = hRGain1 -> GetRMS();
  sr[1] = hRGain2 -> GetRMS();
  sr[2] = hRGain3 -> GetRMS();
  sr[3] = hRGain4 -> GetRMS();
  sr[4] = hRGain5 -> GetRMS();
  sr[5] = hRGain6 -> GetRMS();
  sr[6] = hRGain7 -> GetRMS();
  sr[7] = hRGain8 -> GetRMS();
  sr[8] = hRGain9 -> GetRMS();
  sr[9] = hRGain10 -> GetRMS();
  sr[10] = hRGain11 -> GetRMS();
  sr[11] = hRGain12 -> GetRMS();

  cout << "======" << endl;

  me[0] = hEGain1 -> GetMean(); cout << me[0] << endl;
  me[1] = hEGain2 -> GetMean(); cout << me[1] << endl;
  me[2] = hEGain3 -> GetMean(); cout << me[2] << endl;
  me[3] = hEGain4 -> GetMean(); cout << me[3] << endl;
  me[4] = hEGain5 -> GetMean(); cout << me[4] << endl;
  me[5] = hEGain6 -> GetMean(); cout << me[5] << endl;
  me[6] = hEGain7 -> GetMean(); cout << me[6] << endl;
  me[7] = hEGain8 -> GetMean(); cout << me[7] << endl;
  me[8] = hEGain9 -> GetMean(); cout << me[8] << endl;
  me[9] = hEGain10 -> GetMean(); cout << me[9] << endl;
  me[10] = hEGain11 -> GetMean(); cout << me[10] << endl;
  me[11] = hEGain12 -> GetMean(); cout << me[11] << endl;

  se[0] = hEGain1 -> GetRMS();
  se[1] = hEGain2 -> GetRMS();
  se[2] = hEGain3 -> GetRMS();
  se[3] = hEGain4 -> GetRMS();
  se[4] = hEGain5 -> GetRMS();
  se[5] = hEGain6 -> GetRMS();
  se[6] = hEGain7 -> GetRMS();
  se[7] = hEGain8 -> GetRMS();
  se[8] = hEGain9 -> GetRMS();
  se[9] = hEGain10 -> GetRMS();
  se[10] = hEGain11 -> GetRMS();
  se[11] = hEGain12 -> GetRMS();

  double X[12] = {20., 30., 40., 50., 60., 70., 80., 90., 100., 110., 120., 130.};
  double EX[12] = {};

  // Graph of Real Gain
  TCanvas *c1 = new TCanvas();
  TGraphErrors *gr = new TGraphErrors(12, X, mr, EX, sr);
  TGraphErrors *ge = new TGraphErrors(12, X, me, EX, se);
  c1 -> SetGrid();

  gr -> SetMarkerColor(kBlue);
  gr -> SetMarkerStyle(kFullCircle);
  gr -> GetXaxis() -> SetTitle("Di#hat{a}metro dos Buracos [#mum]");
  gr -> GetYaxis() -> SetTitle("Ganho Real");
  gr -> SetTitle("");
  gr -> Draw("ap");

  TCanvas *c2 = new TCanvas();
  c2 -> SetGrid();
  ge -> SetMarkerColor(kRed);
  ge -> SetMarkerStyle(kFullCircle);
  ge -> GetXaxis() -> SetTitle("Di#hat{a}metro dos Buracos [#mum]");
  ge -> GetYaxis() -> SetTitle("Ganho Efetivo");
  ge -> SetTitle("");
  ge -> Draw("ap");


  auto t_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = t_end - t_start;
  std::cout << "\n... END OF SIMULATION: "
            << diff.count() << " s.\n"<< std::endl;

  app.Run(kTRUE);
}
