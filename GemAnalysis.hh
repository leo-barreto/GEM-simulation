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

  double Vup = -1.5 * (EIND * D_P + VGEM);
  double Vlow = -0.8 * (EIND * D_P);
  double Center = (D_E - D_P) / 2;

  // Visualization of Fields
  TCanvas* cFie = new TCanvas("fie", "Field");
  ViewField* vF = new ViewField();

  vF -> SetCanvas(cFie);
  vF -> SetComponent(Elm);
  vF -> SetPlane(0, -1, 0, 0, H / 2, 0);
  vF -> SetArea(-0.5 * DIST, -0.02 - Center, 0.5 * DIST, 0.02 - Center);
  vF -> SetVoltageRange(-350, 0.);
  vF -> PlotContour();

}




#endif
