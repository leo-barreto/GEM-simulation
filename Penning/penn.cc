#include "../GemSampa.hh"

using namespace std;

int main(int argc, char * argv[]) {

  double g[9];
  string S = "gem70_140_500";

  SetupInfo(g, S, 70., 140., 0.1, 0.1, 0.005, 0.0005, 3000., 1000., 500.);
  GainOneElectron(S, g, false, 0, 1000, 0.05, true);
  GainOneElectron(S, g, false, 0, 1000, 0.05, false);

}
