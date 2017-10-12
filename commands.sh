#!/bin/bash
gmsh gem.geo -3 -order 2 -optimize;
echo "==============================================";
ElmerGrid 14 2 gem.msh -autoclean -centralize;
echo "==============================================";
cd ./gem;
ElmerSolver gem.sif;
echo "==============================================";
ElmerSolver gemWT.sif;
echo "==============================================";
cd ..;
make;
./sim;
