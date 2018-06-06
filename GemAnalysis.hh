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

    std::cout << "\nFilled histograms for " << txts[i] << std::endl;
    hRGain -> Write();
    hEGain -> Write();
    std::cout << "Saved histograms for " << txts[i] << std::endl;

  }

  f -> Close();
  chdir("..");
  std::cout << "\nFinished saving all histograms!\n" << std::endl;

}



#endif
