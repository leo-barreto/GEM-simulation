#include "../GemSampa.hh"
#include <string>
#include <fstream>


using namespace std;

int main(int argc, char * argv[]) {

  double g[9];
  float diam = atof(argv[1]);
  string T = argv[1];
  string S = "gem" + T + "_140_500";
  string TXT = "d" + T + ".txt";

  SetupInfo(g, S, diam, 140., 0.1, 0.1, 0.005, 0.0005, 3000., 1000., 500.);
  GainOneElectron(S, g, TXT, false, 10, 2, 0.05, true);

}
