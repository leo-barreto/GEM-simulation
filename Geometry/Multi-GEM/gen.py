# Generator Script for Multi-GEM Geometric and Electrostatic Conditions

import os
import time


# Parameters

NTOT = 3
FOLDER_NAME = 'triplegem'
geos = ['testem', 'testem1', 'testem2']
potentials = [-2100, -1800, -1400, -1300, -900, -800, -400, 0] # 3 * NTOT + 2
cmd = 'ElmerGrid 2 2 ' + geos[0]

PERMITTIVITY_DIE = 3.23



for i in range(NTOT):
    print('\nSolving for GEM layer [{0} / {1}]...'.format(i + 1, NTOT))
    print('\nMeshing Geometry...')
    os.system('gmsh ' + geos[i] + '.geo -3 -order 2 -optimize')
    print('\nConverting to Elmer...')
    os.system('ElmerGrid 14 2 ' + geos[i] + '.msh')

    if i > 0:
        cmd += ' -in ' + geos[i]

cmd += ' -unite -out ' + FOLDER_NAME + ' -merge 1.0e-8 -autoclean'
os.system(cmd)
os.system('ElmerGrid 2 2 ' + FOLDER_NAME + ' -centralize')


print('\nDeleting Files and Folders...')
os.system('rm ' + ' '.join([x + '.msh' for x in geos])) #add geo files
os.system('rm -r ' + ' '.join([x for x in geos]))






# SIF Files

sif_pot = ''
WTsif_pot = ''
for i in range(len(potentials)):
    pot = potentials[i]
    bc = str(5 + i)
    sif_pot += '''
Boundary Condition ''' + bc + '''
    Target Boundaries = ''' + bc + '''
    Potential = ''' + str(potentials[i]) + '''
End\n'''

    WTsif_pot += '''
Boundary Condition ''' + bc + '''
    Target Boundaries = ''' + bc + '''
    Potential = ''' + str(i // (len(potentials) - 1)) + '''
End\n'''

print('\nWriting .sif...')
sif = open(FOLDER_NAME + '/' + 'gem.sif', 'w')
WTsif = open(FOLDER_NAME + '/' + 'gemWT.sif', 'w')


header = '''Header
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
'''
headerWT = '''Header
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
'''
sif_str = '''Constants
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


! Periodicity in X
Boundary Condition 1
  Target Boundaries = 1
End

Boundary Condition 2
  Target Boundaries = 3
  Periodic BC = 1
  Periodic BC Rotate(3) = Real 0 0 180
End


! Periodicity in Y
Boundary Condition 3
  Target Boundaries = 2
End

Boundary Condition 4
  Target Boundaries = 4
  Periodic BC = 3
  Periodic BC Rotate(3) = Real 0 0 180
End


! Potential definitions'''

sif.write(header + sif_str + sif_pot)
sif.close()

WTsif.write(headerWT + sif_str + WTsif_pot)
WTsif.close()

print('\nSolving Main Field...')
os.chdir('./' + FOLDER_NAME)
os.system('ElmerSolver gem.sif')
print('\nSolving WT Field...')
os.system('ElmerSolver gemWT.sif')


# Dieletrics File
print('\nWriting dieletrics.dat...')
dielectric = open('dielectrics.dat', 'w')
dielectric.write('''4
1 1
2 ''' + str(PERMITTIVITY_DIE) + '''
3 1e10
4 1e10''')
dielectric.close()
