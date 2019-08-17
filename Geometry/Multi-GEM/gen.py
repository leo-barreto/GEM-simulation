# Generator Script for Multi-GEM Geometric and Electrostatic Conditions

import os
import time

# Number of GEMs
NTOT = 3;

ShiftX = {0.05, 0.}
ShiftY = {0.05, 0.}
ZZ = -2.06



start = time.time()


for i in range(NTOT):
