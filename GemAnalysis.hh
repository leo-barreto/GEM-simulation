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



using namespace Garfield;

void ReadTXTGain(std::string txtfolder) {

  std::string folder = "./" + txtfolder;
  DIR* dir = opendir(folder.c_str());
  dirent* pdir;
  std::vector<std::string> txts;
  std::string line;
  int rg, eg;
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
    std::vector<int> RGain, EGain;

    if (File.fail()) {
      std::cerr << "\nFailed to open file\n" << std::endl;
      exit(1);
    }

    while (!File.eof()) {
      File >> line;
      size_t stop1 = line.find(";");
      size_t stop2 = line.find(";", stop1 + 1);
      rg = stod(line.substr(0, stop1));
      eg = stod(line.substr(stop1 + 1, stop2 - stop1 - 1));
      RGain.push_back(rg);
      EGain.push_back(eg);
    }

    // Popping last duplicated elements
    RGain.pop_back();
    EGain.pop_back();

    // Histograms
    int nBins = 100;
    float hmin = 0.;
    float hmaxR = *max_element(RGain.begin(), RGain.end());
    float hmaxE = *max_element(EGain.begin(), EGain.end());
    std::string name_r = "hRGain_" + txts[i].substr(0, txts[i].size() - 4);
    std::string name_e = "hEGain_" + txts[i].substr(0, txts[i].size() - 4);

    TH1I* hRGain = new TH1I(name_r.c_str(), "Real Gain", nBins, hmin, hmaxR);
    TH1I* hEGain = new TH1I(name_e.c_str(), "Effective Gain", nBins, hmin, hmaxE);

    for (int i = 0; i < RGain.size(); i++) {
      hRGain -> Fill(RGain[i]);
      hEGain -> Fill(EGain[i]);
    }

    std::cout << "\n\nFilled histograms for " << txts[i] << std::endl;
    hRGain -> Write();
    hEGain -> Write();
    std::cout << "Saved histograms for " << txts[i] << std::endl;

    double m1, m2, s1, s2;
    m1 = hRGain -> GetMean();
    m2 = hEGain -> GetMean();
    s1 = hRGain -> GetStdDev() / sqrt(hRGain -> GetEntries());
    s2 = hEGain -> GetStdDev() / sqrt(hEGain -> GetEntries());
    std::cout << "Real Gain: " << m1 << "(" << s1 << ")" << std::endl;
    std::cout << "Effective Gain: " << m2 << "(" << s2 << ")" << std::endl;
    std::cout << "Number of Entries: " << hRGain -> GetEntries() << std::endl;

  }

  f -> Close();
  chdir("..");
  std::cout << "\nFinished saving all histograms!\n" << std::endl;

}


void ReadTXTEnergyResolution(std::string txtfolder) {

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
      File >> line;
      ne = stod(line);
      if (ne > 200) {
        NElectrons.push_back(ne);
      }
    }

    // Popping last duplicated elements
    NElectrons.pop_back();

    // Histograms
    int nBins = 50;
    float hmin = 0.;
    float hmaxN = *max_element(NElectrons.begin(), NElectrons.end());
    std::string name_r = "hNElectrons_" + txts[i].substr(0, txts[i].size() - 4);

    TH1I* hNElectrons = new TH1I(name_r.c_str(), "", nBins, hmin, hmaxN);

    for (int i = 0; i < NElectrons.size(); i++) {
      hNElectrons -> Fill(NElectrons[i]);
    }

    std::cout << "\n\nFilled histograms for " << txts[i] << std::endl;
    hNElectrons -> Write();
    std::cout << "Saved histograms for " << txts[i] << std::endl;

    double m1, m2, m3, s1, s2;

    int bin1 = hNElectrons -> FindFirstBinAbove(hNElectrons -> GetMaximum()/2);
    int bin2 = hNElectrons -> FindLastBinAbove(hNElectrons -> GetMaximum()/2);
    m2 = hNElectrons -> GetBinCenter(bin2) - hNElectrons -> GetBinCenter(bin1);

    m1 = hNElectrons -> GetMean();
    s1 = hNElectrons -> GetStdDev() / sqrt(hNElectrons -> GetEntries());
    m3 = m2 / m1;
    s2 = s1 * m2/ (m1 * m1);

    std::cout << "Mean: " << m1 << "(" << s1 << ")" << std::endl;
    std::cout << "FWHM: " << m2 << std::endl;
    std::cout << "Relative Energy Resolution: " << m3 << "(" << s2 << ")" << std::endl;
    std::cout << "Number of Entries: " << hNElectrons -> GetEntries() << std::endl;

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
      File >> line;
      size_t stop1 = line.find(";");
      size_t stop2 = line.find(";", stop1 + 1);
      px = stod(line.substr(0, stop1));
      py = stod(line.substr(stop1 + 1, stop2 - stop1 - 1));
      PosX.push_back(px);
      PosY.push_back(py);
    }

    // Popping last duplicated elements
    PosX.pop_back();
    PosY.pop_back();

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


void PlotElectricField(ComponentElmer* Elm, double info[9]) {

  // GEM Dimensions in cm
  const double T_DIE = info[4];
  const double T_PLA = info[5];
  const double DIST = info[1];
  const double H = sqrt(3) * DIST / 2;
  const double D_E = info[2];
  const double D_P = info[3];

  // Electric Properties in V and V/cm
  const double EIND = info[6];
  const double EDRI = info[7];
  const double VGEM = info[8];

  double Vup = -1.1 * (EIND * D_P + VGEM);
  double Vlow = -0.9 * (EIND * D_P);
  double Center = (D_E - D_P) / 2;
  double Shift = DIST / 4;

  // Visualization of Fields
  TCanvas* cFie = new TCanvas("fie", "Field");
  ViewField* vF = new ViewField();

  vF -> SetCanvas(cFie);
  vF -> SetComponent(Elm);
  vF -> SetPlane(0, -1, 0, 0, H / 2, 0);
  vF -> SetArea(-1.5 * DIST + Shift, -3 * T_DIE - Center, 1.5 * DIST + Shift, 3 * T_DIE - Center);
  vF -> SetVoltageRange(Vup, Vlow);
  vF -> PlotContour();

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

}




#endif
