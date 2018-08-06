# Generator Script for GEM Geometrical and Electrostatics Conditions

import os
import time


# Parameters
# Options (what to generate)
GEN_GEOMETRY = True
GEN_FIELDS = True

# Geometry in mm
RADIUS = 0.035
INTERIOR_RADIUS = RADIUS * 2 / 7
DISTANCE_HOLES = 0.140
THICKNESS_DIE = 0.05
THICKNESS_PLA = 0.005
DISTANCE_PAD = 1.
DISTANCE_ELE = 1.

# Electric Field in V/cm; Potential in V.
E_DIFF = 1000
E_IND = 3000
DELTA_V = 500
PERMITTIVITY_DIE = 3.23 # relative


start = time.time()


# Geometry Generation
d_str = str(int(RADIUS * 2000))
h_str = str(int(DISTANCE_HOLES * 1000))
v_str = str(int(DELTA_V))
geo_name = 'gem' + d_str + '_' + h_str + '_' + v_str
if GEN_GEOMETRY:
    geo_file = open(geo_name + '.geo', 'w')

    geo_file.write('Include "gf_gem.geo";\n')
    geo_file.write('RADIUS = ' + str(RADIUS) + ';\n')
    geo_file.write('DISTANCE_HOLES = ' + str(DISTANCE_HOLES) + ';\n')
    geo_file.write('RADIUS_DIE = ' + str(INTERIOR_RADIUS) + ';\n')
    geo_file.write('THICKNESS_DIE = ' + str(THICKNESS_DIE) + ';\n')
    geo_file.write('THICKNESS_PLA = ' + str(THICKNESS_PLA) + ';\n')
    geo_file.write('DISTANCE_PAD = ' + str(DISTANCE_PAD) + ';\n')
    geo_file.write('DISTANCE_ELE = ' + str(DISTANCE_ELE) + ';\n')
    geo_file.write('Call gf_gem;')
    geo_file.close()

    print('\nMeshing Geometry...')
    os.system('gmsh ' + geo_name + '.geo -3 -order 2 -optimize')

    print('\nConverting to Elmer...')
    os.system('ElmerGrid 14 2 ' + geo_name + '.msh -autoclean -centralize')

    print('\nDeleting Files...')
    os.system('rm ' + geo_name + '.geo ' + geo_name + '.msh')


if GEN_FIELDS:
    print('\nWriting dieletrics.dat...')
    dielectric = open(geo_name + '/' + 'dielectrics.dat', 'w')
    dielectric.write('''4
1 1
2 ''' + str(PERMITTIVITY_DIE) + '''
3 1e10
4 1e10''')
    dielectric.close()

    print('\nWriting .sif...')
    sif = open(geo_name + '/' + 'gem.sif', 'w')
    WTsif = open(geo_name + '/' + 'gemWT.sif', 'w')

    sif.write('''Header
  CHECK KEYWORDS Warn
  Mesh DB "." "."
  Include Path ""
  Results Directory ""
End

Simulation
  Coordinate System = Cartesian 3D
  Simulation Type = Steady State
  Steady State Max Iterations = 1
  Output File = "gem.result"
  Post File = "gem.ep"
End

Constants
  Permittivity Of Vacuum = 8.8542e-12
End

Body 1
  Equation = 1
  Material = 1
End

Body 2
  Equation = 1
  Material = 2
End

Body 3
  Equation = 1
  Material = 3
End

Body 4
  Equation = 1
  Material = 3
End

Solver 1
  Equation = Stat Elec Solver
  Variable = Potential
  Variable DOFs = 1
  Procedure = "StatElecSolve" "StatElecSolver"
  Calculate Electric Field = True
  Calculate Electric Flux = False
  Linear System Solver = Iterative
  Linear System Iterative Method = BiCGStab
  Linear System Max Iterations = 1000
  Linear System Abort Not Converged = True
  Linear System Convergence Tolerance = 1.0e-10
  Linear System Preconditioning = ILU1
  Steady State Convergence Tolerance = 5.0e-7
End


! Gas
Material 1
  Relative Permittivity = 1
End

! Dielectric
Material 2
  Relative Permittivity = ''' + str(PERMITTIVITY_DIE) + '''
End

! Copper
Material 3
  Relative Permittivity = 1.0e10
End

! Upper electrode
Boundary Condition 1
  Target Boundaries = 5
  Potential = ''' + str(-0.1 * E_IND * DISTANCE_PAD - DELTA_V + \
                        -0.1 * E_DIFF * DISTANCE_ELE) + '''
End

! Upper copper plate
Boundary Condition 2
  Target Boundaries = 6
  Potential = ''' + str(-0.1 * E_IND * DISTANCE_PAD - DELTA_V) + '''
End

! Lower copper plate
Boundary Condition 3
  Target Boundaries = 7
  Potential = ''' + str(-0.1 * E_IND * DISTANCE_PAD) + '''
End

! Lower electrode
Boundary Condition 4
  Target Boundaries = 8
  Potential = 0
End


! Periodicity in X
Boundary Condition 5
  Target Boundaries = 1
End

Boundary Condition 6
  Target Boundaries = 3
  Periodic BC = 5
  Periodic BC Rotate(3) = Real 0 180 0
End


! Periodicity in Y
Boundary Condition 7
  Target Boundaries = 2
End

Boundary Condition 8
  Target Boundaries = 4
  Periodic BC = 7
  Periodic BC Rotate(3) = Real 180 0 0
End''')

    sif.close()
    WTsif.write('''Header
  CHECK KEYWORDS Warn
  Mesh DB "." "."
  Include Path ""
  Results Directory ""
End

Simulation
  Coordinate System = Cartesian 3D
  Simulation Type = Steady State
  Steady State Max Iterations = 1
  Output File = "gemWT.result"
  Post File = "gemWT.ep"
End

Constants
  Permittivity Of Vacuum = 8.8542e-12
End

Body 1
  Equation = 1
  Material = 1
End

Body 2
  Equation = 1
  Material = 2
End

Body 3
  Equation = 1
  Material = 3
End

Body 4
  Equation = 1
  Material = 3
End

Solver 1
  Equation = Stat Elec Solver
  Variable = Potential
  Variable DOFs = 1
  Procedure = "StatElecSolve" "StatElecSolver"
  Calculate Electric Field = True
  Calculate Electric Flux = False
  Linear System Solver = Iterative
  Linear System Iterative Method = BiCGStab
  Linear System Max Iterations = 1000
  Linear System Abort Not Converged = True
  Linear System Convergence Tolerance = 1.0e-10
  Linear System Preconditioning = ILU1
  Steady State Convergence Tolerance = 5.0e-7
End


! Gas
Material 1
  Relative Permittivity = 1
End

! Dielectric
Material 2
  Relative Permittivity = ''' + str(PERMITTIVITY_DIE) + '''
End

! Copper
Material 3
  Relative Permittivity = 1.0e10
End

! Upper electrode
Boundary Condition 1
  Target Boundaries = 5
  Potential = 0
End

! Upper copper plate
Boundary Condition 2
  Target Boundaries = 6
  Potential = 0
End

! Lower copper plate
Boundary Condition 3
  Target Boundaries = 7
  Potential = 0
End

! Lower electrode
Boundary Condition 4
  Target Boundaries = 8
  Potential = 1
End


! Periodicity in X
Boundary Condition 5
  Target Boundaries = 1
End

Boundary Condition 6
  Target Boundaries = 3
  Periodic BC = 5
  Periodic BC Rotate(3) = Real 0 180 0
End


! Periodicity in Y
Boundary Condition 7
  Target Boundaries = 2
End

Boundary Condition 8
  Target Boundaries = 4
  Periodic BC = 7
  Periodic BC Rotate(3) = Real 180 0 0
End''')

    WTsif.close()

    print('\nSolving Main Field...')
    os.chdir('./' + geo_name)
    os.system('ElmerSolver gem.sif')
    print('\nSolving WT Field...')
    os.system('ElmerSolver gemWT.sif')
    os.chdir('..')



time = time.time() - start
print('\n\nDONE! (in %.2f seconds)' % time)
