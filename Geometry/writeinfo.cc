#include <iostream>
#include <dirent.h>
#include <vector>

#include <TFile.h>
#include <TVectorD.h>

using namespace std;

int main(int argc, char * argv[]) {
  vector<float> v;

  for (int i = 1; i < argc; i++) {
    v.push_back(std::stof(argv[i]));
  }

  // Write ROOT file
  cout << "\nWriting ROOT File..." << endl;
  TFile* f = new TFile("info.root", "RECREATE");
  f -> WriteObject(&v, "info");
  f -> Close();
}
