# Generator Script for Multi-GEM Geometric and Electrostatic Conditions

import os
import time

GEN_SETUP = True
GEN_GEOMETRY = True
GEN_FIELDS = True
# Still deciding if I'll implement the results on the GEM folder
GEN_ROOT = False


# ======================= START OF USER INPUT =======================
# Parameters
FOLDER_NAME = 'gem_example'
NTOT = 2
SHIFT_X = [0]
SHIFT_Y = [0]

# Geometry in mm, all are lists
DISTANCE_HOLES = [0.140]
RADIUS_UPPER_PLA = [0.035]
RADIUS_LOWER_PLA = [0.035]
THICKNESS_UPPER_PLA = [0.005]
THICKNESS_LOWER_PLA = [0.005]
# Inner Geometry of the Dieletric
# Note that THICKNESS_DIE = POSDIE[0] - POSDIE[NDIE - 1]
NDIE = [3]                      # Number of dieletric planes
POSDIE = [[0.025, 0, -0.025]]   # Positions of dieletric planes
RDIE = [[0.035, 0.025, 0.035]]  # Radius of each dieletric plane

# Region in mm, only TRANSFER is a list
DRIFT = 1.
TRANSFER = [1.]
INDUCTION = 1.

# Electric Field in V/cm; Potential in V, only E_TRANSFER and DELTA_V are lists
E_DRIFT = 1000
E_TRANSFER = [3000]
E_INDUCTION = 5000
DELTA_V = [420]
POTENTIALS = []
PERMITTIVITY_DIE = 3.23 # relative

# Mesh Quality on Chambers (near GEM is fixed on DISTANCE_HOLES / 15)
LC_CHA = 0.1;
# ======================= END OF USER INPUT =======================


# Auxiliary functions
def fill_parameter(par, n):
    out = list(par)
    l_out = len(out)
    for i in range(n - l_out):
        out.append(par[-1])

    return out

def potential_calculator(regions, fields, deltav, pots):
    p = [0, regions[-1] * fields[-1] / -10]
    if pots == []:
        for i in reversed(range(len(deltav))):
            p.append(p[-1] - deltav[i])
            p.append(p[-1] - regions[i] * fields[i] / 10)

        return p[::-1]
    else:
        return pots


start = time.time()
if GEN_SETUP:
    print('\n============== SETUP ==============\n')

    # Read user input
    x0 = fill_parameter(SHIFT_X, NTOT)
    y0 = fill_parameter(SHIFT_Y, NTOT)
    r1 = fill_parameter(RADIUS_UPPER_PLA, NTOT)
    r2 = fill_parameter(RADIUS_LOWER_PLA, NTOT)
    d_holes = fill_parameter(DISTANCE_HOLES, NTOT)
    t_pla1 = fill_parameter(THICKNESS_UPPER_PLA, NTOT)
    t_pla2 = fill_parameter(THICKNESS_LOWER_PLA, NTOT)
    ndie = fill_parameter(NDIE, NTOT)
    posdie = fill_parameter(POSDIE, NTOT)
    rdie = fill_parameter(RDIE, NTOT)
    regions = [DRIFT] + fill_parameter(TRANSFER, NTOT - 1) + [INDUCTION]
    fields = [E_DRIFT] + fill_parameter(E_TRANSFER, NTOT - 1) + [E_INDUCTION]
    deltav = fill_parameter(DELTA_V, NTOT)
    potentials = potential_calculator(regions, fields, deltav, POTENTIALS)

    # Calculate regions for each cell
    ele = []
    pad = []
    translate = [0]
    for i in range(NTOT):
        if i == 0:
            ele.append(regions[0])
        else:
            ele.append(regions[i] / 2)
            tdie1 = posdie[i - 1][0] - posdie[i - 1][ndie[i - 1] - 1]
            tdie2 = posdie[i][0] - posdie[i][ndie[i] - 1]
            translate.append(translate[-1] - ele[i] - pad[i - 1] - \
                            (tdie1 + tdie2) / 2 - t_pla1[i] - t_pla2[i - 1])

        if i == NTOT - 1:
            pad.append(regions[-1])
        else:
            pad.append(regions[i + 1] / 2)


if GEN_GEOMETRY:
    print('\n============== GEOMETRY ==============\n')
    geos = []
    # Write .geo scripts
    for i in range(NTOT):
        geo_name = 'gem_part' + str(i)
        geos.append(geo_name)
        geo_file = open(geo_name + '.geo', 'w')

        geo_file.write('Include "gf_gem.geo";\n')
        # General properties
        geo_file.write('NTOT = ' + str(NTOT) + ';\n')
        geo_file.write('ID = ' + str(i) + ';\n')
        geo_file.write('DIST = ' + str(d_holes[i]) + ';\n')
        geo_file.write('Z = ' + str(translate[i]) + ';\n')
        geo_file.write('X = ' + str(x0[i]) + ';\n')
        geo_file.write('Y = ' + str(y0[i]) + ';\n')
        # Plates
        geo_file.write('R1 = ' + str(r1[i]) + ';\n')
        geo_file.write('R2 = ' + str(r2[i]) + ';\n')
        geo_file.write('TPLA1 = ' + str(t_pla1[i]) + ';\n')
        geo_file.write('TPLA2 = ' + str(t_pla2[i]) + ';\n')
        # Dielectric
        geo_file.write('NDIE = ' + str(ndie[i]) + ';\n')
        geo_file.write('POSDIE = {' +  ",".join([str(x) for x in posdie[i]]) + '};\n')
        geo_file.write('RDIE = {' +  ",".join([str(x) for x in rdie[i]]) + '};\n')
        # Regions
        geo_file.write('DRI = ' + str(ele[i]) + ';\n')
        geo_file.write('IND = ' + str(pad[i]) + ';\n')
        geo_file.write('LC_CHA = ' + str(LC_CHA) + ';\n')
        geo_file.write('Call gf_gem;')
        geo_file.close()


    cmd = 'ElmerGrid 2 2 ' + geos[0]


    # Mesh and Elmer preparation
    for i in range(NTOT):
        print('\nSolving for GEM layer [{0} / {1}]...'.format(i + 1, NTOT))
        print('\nMeshing Geometry...')
        os.system('gmsh ' + geos[i] + '.geo -3 -order 2')
        print('\nConverting to Elmer...')
        os.system('ElmerGrid 14 2 ' + geos[i] + '.msh')

        if i > 0:
            cmd += ' -in ' + geos[i]

    cmd += ' -unite -out ' + FOLDER_NAME + ' -merge 1.0e-8 -autoclean'

    if NTOT > 1:
        os.system(cmd)
        os.system('ElmerGrid 2 2 ' + FOLDER_NAME + ' -centralize')
    else:
        os.system('ElmerGrid 14 2 ' + geo_name + '.msh -out ' + FOLDER_NAME +' -autoclean -centralize')

    print('\nDeleting Files and Folders...')
    os.system('rm ' + ' '.join([x + '.geo' for x in geos]))
    os.system('rm ' + ' '.join([x + '.msh' for x in geos]))
    os.system('rm -r ' + ' '.join([x for x in geos]))


if GEN_FIELDS:
    print('\n============== FIELDS ==============\n')
    # SIF Files
    sif_pot = ''
    WTsif_pot = ''
    for i in range(len(potentials)):
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
  Material = 2
End

Body 2
  Equation = 1
  Material = 3
End

Body 3
  Equation = 1
  Material = 3
End

Body 4
  Equation = 1
  Material = 1
End

Equation 1
  Active Solvers(1) = 1
  Calculate Electric Energy = True
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
  Density = 1
End

! Dielectric
Material 2
  Relative Permittivity = ''' + str(PERMITTIVITY_DIE) + '''
  Density = 2
End

! Copper
Material 3
  Relative Permittivity = 1.0e10
  Density = 3
End


! Periodicity in Y
Boundary Condition 1
  Target Boundaries = 1
End

Boundary Condition 2
  Target Boundaries = 3
  Periodic BC = 1
  Periodic BC Potential = Logical True
End


! Periodicity in X
Boundary Condition 3
  Target Boundaries = 2
End

Boundary Condition 4
  Target Boundaries = 4
  Periodic BC = 3
  Periodic BC Potential = Logical True
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
    1 ''' + str(PERMITTIVITY_DIE) + '''
    2 1e10
    3 1e10
    4 1''')
    dielectric.close()



time = time.time() - start
print('\n\nDONE! (in %.2f seconds)' % time)
