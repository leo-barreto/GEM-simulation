#include "../GemSampa.hh"
#include <string>
#include <fstream>


using namespace std;

int main(int argc, char * argv[]) {

  double g[9];
  float potent = atof(argv[1]);
  string T = argv[1];
  string S = "gem70_140_" + T;
  string TXT = "t" + T + ".txt";

  SetupInfo(g, S, 70., 140., 0.1, 0.1, 0.005, 0.0005, 3000., 1000., potent);
  GainOneElectron(S, g, TXT, false, 0, 200, 0.05, true);

}
