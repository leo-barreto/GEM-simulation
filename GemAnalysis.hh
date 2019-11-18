#ifndef GEMANALYSIS_H
#define GEMANALYSIS_H


#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <dirent.h>
#include <math.h>

#include <TApplication.h>
#include <TMath.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TFile.h>

#include "ComponentElmer.hh"
#include "MediumMagboltz.hh"
#include "Sensor.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"
#include "Random.hh"
#include "TrackHeed.hh"
#include "ViewField.hh"
#include "Plotting.hh"
#include "ViewFEMesh.hh"

#include "GemSampa.hh"


using namespace Garfield;

void ReadTXTGain(std::string txtfolder, bool draw = 0) {

  std::string folder = "./" + txtfolder;
  DIR* dir = opendir(folder.c_str());
  dirent* pdir;
  std::vector<std::string> txts;
  std::string line;
  double rg, eg;
  double t;
  std::string rootname = folder + "_hists.root";


  // Getting all files of folder
  while (pdir = readdir(dir)) {
    std::string fn = pdir -> d_name;
    if (fn != "." and fn != "..") {
      txts.push_back(fn);
    }
  }

  TFile* f = new TFile(rootname.c_str(), "RECREATE");
  chdir(folder.c_str());

  for (int i = 0; i < txts.size(); i++) {
    std::ifstream File;
    File.open(txts[i].c_str());
    std::vector<double> RGain, EGain;
    std::vector<double> time;

    if (File.fail()) {
      std::cerr << "\nFailed to open file\n" << std::endl;
      exit(1);
    }

    while (!File.eof()) {
      getline(File, line);

      if (line[0] != '#' && line.length() != 0) { // Ignore info
        size_t stop1 = line.find(";");
        size_t stop2 = line.find(";", stop1 + 1);
        rg = stod(line.substr(0, stop1));
        eg = stod(line.substr(stop1 + 1, stop2));
        t = stod(line.substr(stop2 + 1, line.size()));
        RGain.push_back(rg);
        EGain.push_back(eg);
        time.push_back(t);
      }
    }

    // Histograms
    int nBins = 100;
    float hmin = 0.;
    float hmaxR = 1.1 * *max_element(RGain.begin(), RGain.end());
    float hmaxE = 1.1 * *max_element(EGain.begin(), EGain.end());
    float hmaxT = 1.1 * *max_element(time.begin(), time.end());
    std::string name_r = "hRGain_" + txts[i].substr(0, txts[i].size() - 4);
    std::string name_e = "hEGain_" + txts[i].substr(0, txts[i].size() - 4);

    TH1F* hRGain = new TH1F(name_r.c_str(), "Real Gain", nBins, hmin, hmaxR);
    TH1F* hEGain = new TH1F(name_e.c_str(), "Effective Gain", nBins, hmin, hmaxE);
    TH1F* hT = new TH1F("Time", "Time", nBins, hmin, hmaxT);

    for (int i = 0; i < RGain.size(); i++) {
      if (EGain[i] > 0) {
        hRGain -> Fill(RGain[i]);
        hEGain -> Fill(EGain[i]);
        hT -> Fill(time[i]);
      }
    }

    std::cout << "\n\nFilled histograms for " << txts[i] << std::endl;
    hRGain -> Write();
    hEGain -> Write();
    std::cout << "Saved histograms for " << txts[i] << std::endl;

    if (draw == 1) {
      TCanvas* c1 = new TCanvas("", "canvas");
      c1 -> Divide(1, 2);
      c1 -> cd(1);
      hRGain -> Draw();
      c1 -> cd(2);
      hEGain -> Draw();
      std::string image = name_r + ".pdf";
      c1 -> SaveAs(image.c_str());
    }

    double m1, m2, s1, s2;
    m1 = hRGain -> GetMean();
    m2 = hEGain -> GetMean();
    s1 = hRGain -> GetStdDev() / sqrt(hRGain -> GetEntries());
    s2 = hEGain -> GetStdDev() / sqrt(hEGain -> GetEntries());
    std::cout << "Real Gain: " << m1 << "(" << s1 << ")" << std::endl;
    std::cout << "Effective Gain: " << m2 << "(" << s2 << ")" << std::endl;
    std::cout << "Number of Entries: " << hRGain -> GetEntries() << std::endl;
    std::cout << "Average time: " << hT -> GetMean() << std::endl;

  }

  f -> Close();
  chdir("..");
  std::cout << "\nFinished saving all histograms!\n" << std::endl;

}


void ReadTXTEnergyResolution(std::string txtfolder, bool draw = 0) {

  std::string folder = "./" + txtfolder;
  DIR* dir = opendir(folder.c_str());
  dirent* pdir;
  std::vector<std::string> txts;
  std::string line;
  int ne;
  std::string rootname = folder + "_hists.root";


  // Getting all files of folder
  while (pdir = readdir(dir)) {
    std::string fn = pdir -> d_name;
    if (fn != "." and fn != "..") {
      txts.push_back(fn);
    }
  }

  TFile* f = new TFile(rootname.c_str(), "RECREATE");
  chdir(folder.c_str());

  for (int i = 0; i < txts.size(); i++) {
    std::ifstream File;
    File.open(txts[i].c_str());
    std::vector<int> NElectrons;

    if (File.fail()) {
      std::cerr << "\nFailed to open file\n" << std::endl;
      exit(1);
    }

    while (!File.eof()) {
      getline(File, line);


      if (line[0] != '#' && line.length() != 0) { // Ignore info
        ne = stod(line);
        if (ne > 300) {
          NElectrons.push_back(ne);
        }
      }
    }


    // Histograms
    int nBins = 100;
    float hmin = 0.;
    float hmaxN = *max_element(NElectrons.begin(), NElectrons.end()) + 100;
    std::string name_r = "hNElectrons_" + txts[i].substr(0, txts[i].size() - 4);

    TH1I* hNElectrons = new TH1I(name_r.c_str(), "", nBins, hmin, hmaxN);

    for (int i = 0; i < NElectrons.size(); i++) {
      hNElectrons -> Fill(NElectrons[i]);
    }

    std::cout << "\n\nFilled histograms for " << txts[i] << std::endl;
    hNElectrons -> Write();
    std::cout << "Saved histograms for " << txts[i] << std::endl;


    // Analysis
    hNElectrons -> Fit("gaus", "R", "", hmaxN / 2, hmaxN);

    if (draw == 1) {
      TCanvas* c1 = new TCanvas("", "canvas");
      hNElectrons -> Draw();
      std::string image = name_r + ".pdf";
      c1 -> SaveAs(image.c_str());
    }

    double mean = hNElectrons -> GetFunction("gaus") -> GetParameter(1);
    double errmean = hNElectrons -> GetFunction("gaus") -> GetParError(1);
    double FWMH = 2.35 * hNElectrons -> GetFunction("gaus") -> GetParameter(2);
    double errFWMH = hNElectrons -> GetFunction("gaus") -> GetParError(2);

    double er = FWMH / mean;
    double errer = sqrt(pow(errFWMH / mean, 2) + pow(errmean * FWMH / pow(mean, 2), 2));
    //std::cout << 2.35 * fit -> GetParameter(2) / fit -> GetParameter(1) << std::endl;
    std::cout << er << std::endl;
    std::cout << errer << std::endl;

  }

  f -> Close();
  chdir("..");
  std::cout << "\nFinished saving all histograms!\n" << std::endl;

}

void ReadTXTPositionResolution(std::string txtfolder, int nBins) {

  std::string folder = "./" + txtfolder;
  DIR* dir = opendir(folder.c_str());
  dirent* pdir;
  std::vector<std::string> txts;
  std::string line;
  int ne;
  std::string rootname = folder + "_hists.root";


  // Getting all files of folder
  while (pdir = readdir(dir)) {
    std::string fn = pdir -> d_name;
    if (fn != "." and fn != "..") {
      txts.push_back(fn);
    }
  }

  TFile* f = new TFile(rootname.c_str(), "RECREATE");
  chdir(folder.c_str());

  for (int i = 0; i < txts.size(); i++) {
    std::ifstream File;
    File.open(txts[i].c_str());
    std::vector<double> PosX;
    std::vector<double> PosY;
    double px, py;

    if (File.fail()) {
      std::cerr << "\nFailed to open file\n" << std::endl;
      exit(1);
    }

    while (!File.eof()) {
      getline(File, line);

      if (line[0] != '#' && line.length() != 0) { // Ignore info
        size_t stop1 = line.find(";");
        size_t stop2 = line.find(";", stop1 + 1);
        px = stod(line.substr(0, stop1));
        py = stod(line.substr(stop1 + 1, stop2 - stop1 - 1));
        PosX.push_back(px);
        PosY.push_back(py);
      }
    }


    // Histograms
    float hmin = *min_element(PosX.begin(), PosX.end()) / 7;
    float hmaxN = *max_element(PosX.begin(), PosX.end()) / 7;
    std::string name_rx = "hPosX_" + txts[i].substr(0, txts[i].size() - 4);
    std::string name_ry = "hPosY_" + txts[i].substr(0, txts[i].size() - 4);

    TH1I* hpx = new TH1I(name_rx.c_str(), "", nBins, hmin, hmaxN);
    TH1I* hpy = new TH1I(name_ry.c_str(), "", nBins, hmin, hmaxN);

    for (int i = 0; i < PosX.size(); i++) {
      hpx -> Fill(PosX[i]);
      hpy -> Fill(PosY[i]);
    }

    std::cout << "\n\nFilled histograms for " << txts[i] << std::endl;
    hpx -> Write();
    hpy -> Write();
    std::cout << "Saved histograms for " << txts[i] << std::endl;

    double m1x, m2x, s1x, s2x;
    double m1y, m2y, s1y, s2y;

    int bin1x = hpx -> FindFirstBinAbove(hpx -> GetMaximum()/2);
    int bin2x = hpx -> FindLastBinAbove(hpx -> GetMaximum()/2);
    m2x = hpx -> GetBinCenter(bin2x) - hpx -> GetBinCenter(bin1x);

    m1x = (hmaxN - hmin) / nBins * 1e4;
    s2x = m1x / sqrt(12);

    int bin1y = hpy -> FindFirstBinAbove(hpy -> GetMaximum()/2);
    int bin2y = hpy -> FindLastBinAbove(hpy -> GetMaximum()/2);
    m2y = hpy -> GetBinCenter(bin2y) - hpy -> GetBinCenter(bin1y);

    m1y = (hmaxN - hmin) / nBins * 1e4;
    s2y = m1y / sqrt(12);

    std::cout << "Pad Size: " << m1x << std::endl;
    std::cout << "Position Resolution X: " << m2x * 1e4 << "(" << s2x << ")" << std::endl;
    std::cout << "Position Resolution Y: " << m2y * 1e4<< "(" << s2y << ")" << std::endl;
    std::cout << "Number of Entries: " << hpx -> GetEntries() << std::endl;

  }

  f -> Close();
  chdir("..");
  std::cout << "\nFinished saving all histograms!\n" << std::endl;

}


void PlotElectricPotential(std::string folder) {

    std::vector<float> g = SetupInfo(folder);
    ComponentElmer* Elm = LoadGas(folder);

    // GEM constants
    double Vup = -1.1 * (g[6] * g[3] + g[8]);
    double Vlow = -0.9 * (g[6] * g[3]);
    double Center = (g[2] - g[3]) / 2;
    double Shift = g[1] / 4;
    double H = sqrt(3) * g[1] / 2;

    // Visualization of Fields
    TCanvas* cFie = new TCanvas("fie", "Field");
    ViewField* vF = new ViewField();

    vF -> SetCanvas(cFie);
    vF -> SetComponent(Elm);
    vF -> SetPlane(0, -1, 0, 0, H / 2, 0);
    vF -> SetArea(-1.5 * g[1] + Shift, -3 * g[4] - Center, 1.5 * g[1] + Shift, 3 * g[4] - Center);
    vF -> SetVoltageRange(Vup, Vlow);
    vF -> PlotContour();

    std::string image = folder + "_field.pdf";
    cFie -> SaveAs(image.c_str());
}


void PlotMesh(ComponentElmer* Elm, double info[9]) {

  // GEM Dimensions in cm
  const double T_DIE = info[4];
  const double T_PLA = info[5];
  const double DIST = info[1];
  const double H = sqrt(3) * DIST / 2;
  const double D_E = info[2];
  const double D_P = info[3];

  double Center = (D_E - D_P) / 2;

  // Visualization of Geometry
  TCanvas *cGeo = new TCanvas("geo", "Geometry");
  ViewFEMesh* vFE = new ViewFEMesh();

  vFE -> SetCanvas(cGeo);
  vFE -> SetComponent(Elm);
  vFE -> SetPlane(0, -1, 0, 0, H / 2, 0);
  vFE -> SetFillMesh(true);
  vFE -> SetColor(1, kCyan - 3);
  vFE -> SetColor(2, kOrange + 7);
  vFE -> SetColor(3, kOrange + 7);
  vFE -> EnableAxes();
  vFE -> SetXaxisTitle("x (cm)");
  vFE -> SetYaxisTitle("z (cm)");
  vFE -> SetArea(-1.5 * DIST, -3 * T_DIE - Center, -1., 1.5 * DIST, 3 * T_DIE - Center, 1.);
  vFE -> Plot();

  std::string image = "mesh.pdf";
  cGeo -> SaveAs(image.c_str());
}


void PM(ComponentElmer *Elm, double DIST = 0.014) {
  TCanvas *cGeo = new TCanvas("geo", "Geometry");
  ViewFEMesh* vFE = new ViewFEMesh();

  const double H = sqrt(3) * DIST / 2;
  const double Z = 0.005;

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
  vFE -> SetArea(-0.5 * DIST, -Z, -1., 0., Z, 1.);
  vFE -> Plot();

  std::string image = "meshtri.png";
  cGeo -> SaveAs(image.c_str());
}


void EF(ComponentElmer *Elm, double Z, double DIST = 0.014) {
  TCanvas* cFie = new TCanvas("fie", "Field");
  ViewField* vF = new ViewField();

  const double H = sqrt(3) * DIST / 2;

  vF -> SetCanvas(cFie);
  vF -> SetComponent(Elm);
  vF -> SetNumberOfContours(80);
  vF -> SetPlane(0, -1, 0, 0, 0, 0);
  vF -> SetArea(-3 * DIST, -0.2 * Z, 3 * DIST, 0.2 * Z);
  vF -> SetElectricFieldRange(0., 100000.);
  vF -> PlotContour("e");

  std::string image = "field.pdf";
  cFie -> SaveAs(image.c_str());
}


void EProfile(ComponentElmer *Elm, double Z, double DIST = 0.014) {
  // Stolen from Geovane's tutorial
  TCanvas* eProfileElectricField = new TCanvas("Profile", "Profile");
  ViewField * pf = new ViewField();

  const double H = sqrt(3) * DIST / 2;
  pf -> SetComponent(Elm);
  pf -> SetArea(-1 * DIST, -0.2 * Z, 1.5 * DIST, 0.2 * Z);
  //pf -> SetNumberOfContours(40);
  pf -> SetPlane(0, 1, 0, H, H, 0);
  pf -> SetCanvas(eProfileElectricField);
  pf -> SetElectricFieldRange(0., 100000.);
  pf -> PlotContour("e");

  std::string image = "profilefield.pdf";
  eProfileElectricField  -> SaveAs(image.c_str());
}




#endif
