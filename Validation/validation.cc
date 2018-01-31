#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <TApplication.h>
#include "../GemSim.hh"


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

  SetupInfo(g, S1, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S1, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S2, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S2, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S3, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S3, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S4, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S4, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S5, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S5, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S6, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S6, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S7, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S7, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S8, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S8, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S9, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S9, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S10, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S10, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S11, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S11, g, false, 20, 500, 0.05, true);

  SetupInfo(g, S12, 0.1, 0.1, 0.005, 0.0005, 3000, 1000);
  GainOneElectron(S12, g, false, 20, 500, 0.05, true);













  auto t_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = t_end - t_start;
  std::cout << "\n... END OF SIMULATION: "
            << diff.count() << " s.\n"<< std::endl;

  //app.Run(kTRUE);
}
