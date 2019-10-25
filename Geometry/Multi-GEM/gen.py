# Generator Script for Multi-GEM Geometric and Electrostatic Conditions

import os
import time

GEN_SETUP = True
GEN_GEOMETRY = True
GEN_FIELDS = True
GEN_ROOT = False


# ======================= START OF USER INPUT =======================
# Parameters
FOLDER_NAME = 'gem_exemplo'
TYPE = 'g'      # g for GEM, t for THGEM (THGEM not yet implemented)

# Geometry in mm, all are lists
RADIUS = [0.035]
SECONDARY_RADIUS = [RADIUS[0] * 2 / 7]
DISTANCE_HOLES = [0.140]
THICKNESS_DIE = [0.05]
THICKNESS_PLA = [0.005]

# Region in mm, only TRANSFER is a list
DRIFT = 1.
TRANSFER = []
INDUCTION = 1.

# Electric Field in V/cm; Potential in V, only E_TRANSFER and DELTA_V are lists
E_DRIFT = 1000
E_TRANSFER = [3000]
E_INDUCTION = 5000
DELTA_V = [500]
POTENTIALS = []
PERMITTIVITY_DIE = 3.23 # relative
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

if GEN_SETUP:
    start = time.time()
    NTOT = len(TYPE)


    # Read user input
    rad = fill_parameter(RADIUS, NTOT)
    s_rad = fill_parameter(SECONDARY_RADIUS, NTOT)
    d_holes = fill_parameter(DISTANCE_HOLES, NTOT)
    t_die = fill_parameter(THICKNESS_DIE, NTOT)
    t_pla = fill_parameter(THICKNESS_PLA, NTOT)
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
            translate.append(translate[-1] - ele[i] - pad[i - 1] - t_die[i - 1] - 2 * t_pla[i - 1])

        if i == NTOT - 1:
            pad.append(regions[-1])
        else:
            pad.append(regions[i + 1] / 2)


if GEN_GEOMETRY:
    geos = []
    # Write .geo scripts
    for i in range(NTOT):
        geo_name = 'gem_part' + str(i)
        geos.append(geo_name)
        geo_file = open(geo_name + '.geo', 'w')

        geo_file.write('Include "gf_mgem.geo";\n')
        geo_file.write('NTOT = ' + str(NTOT) + ';\n')
        geo_file.write('ID = ' + str(i) + ';\n')
        geo_file.write('RADIUS = ' + str(rad[i]) + ';\n')
        geo_file.write('DISTANCE_HOLES = ' + str(d_holes[i]) + ';\n')
        geo_file.write('RADIUS_DIE = ' + str(s_rad[i]) + ';\n')
        geo_file.write('THICKNESS_DIE = ' + str(t_die[i]) + ';\n')
        geo_file.write('THICKNESS_PLA = ' + str(t_pla[i]) + ';\n')
        geo_file.write('DISTANCE_PAD = ' + str(pad[i]) + ';\n')
        geo_file.write('DISTANCE_ELE = ' + str(ele[i]) + ';\n')
        geo_file.write('Z = ' + str(translate[i]) + ';\n')
        geo_file.write('Call gf_mgem;')
        geo_file.close()


    cmd = 'ElmerGrid 2 2 ' + geos[0]


    # Mesh and Elmer preparation
    for i in range(NTOT):
        print('\nSolving for GEM layer [{0} / {1}]...'.format(i + 1, NTOT))
        print('\nMeshing Geometry...')
        os.system('gmsh ' + geos[i] + '.geo -3 -order 2 -optimize')
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



time = time.time() - start
print(potentials)
print('\n\nDONE! (in %.2f seconds)' % time)
