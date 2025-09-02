# GEM Detector Simulation Framework

A comprehensive simulation framework for Gas Electron Multiplier (GEM) detectors using the Garfield++ library. This toolkit provides detailed particle tracking, field calculations, and performance analysis for single and multi-GEM detector configurations.

## Overview

Gas Electron Multiplier (GEM) detectors are widely used in high-energy physics experiments for their excellent spatial resolution, high rate capability, and radiation hardness. This simulation framework enables detailed study of:

- **Electron avalanche processes** in gas mixtures
- **Gain characteristics** and optimization
- **Energy and position resolution** studies  
- **Primary ionization** tracking and analysis
- **Electric field** visualization and optimization
- **Multi-particle tracking** (photons, alpha particles, pions, etc.)

## Features

### Core Simulation Capabilities
- 🔬 **Microscopic avalanche simulation** with detailed collision tracking
- ⚡ **Electric field calculations** using finite element methods (Elmer)
- 📊 **Statistical analysis** with ROOT histograms and fitting
- 🎯 **Position resolution** studies with pad readout simulation
- 🔋 **Energy resolution** analysis for X-ray and particle sources
- 📈 **Gain optimization** for various detector geometries

### Supported Analyses
- Real and effective gain measurements
- Energy resolution for Fe-55 X-ray sources
- Position resolution with different pad configurations
- Primary ionization studies
- Signal formation and timing analysis
- Multi-GEM cascade simulations

## Dependencies

### Required Software
- **ROOT** (≥6.0) - CERN's data analysis framework
- **Garfield++** - Particle detector simulation toolkit  
- **Elmer** - Finite element software for field calculations
- **C++ compiler** with C++11 support (GCC/Clang)
- **Python 3** - For geometry generation scripts

### Setup Environment
```bash
# Set Garfield++ environment
export GARFIELD_HOME=/path/to/garfield++

# Ensure ROOT is available
source /path/to/root/bin/thisroot.sh
```

## Project Structure

```
GEM-simulation/
├── README.md                 # This documentation
├── GemSampa.hh              # Core simulation functions and utilities
├── GemAnalysis.hh           # Analysis and visualization tools
├── Examples/                # Simulation examples and tutorials
│   ├── Triple-GEM/         # Multi-GEM detector simulation
│   ├── PionLaunch/         # Pion particle tracking
│   ├── PrimaryIon/         # Primary ionization studies  
│   ├── SignalOut/          # Signal formation analysis
│   └── gem_example/        # Basic single GEM example
├── NewGeometry/            # Geometry generation tools
│   ├── gen.py             # Python geometry generator
│   ├── gf_functions.geo   # Gmsh geometry functions
│   └── gf_gem.geo         # GEM-specific geometry templates
└── Old/                   # Legacy code and references
```

## Quick Start

### 1. Basic Single GEM Simulation

```cpp
#include "GemSampa.hh"
#include "GemAnalysis.hh"

int main() {
    // Load detector configuration
    std::string folder = "gem_example";
    ComponentElmer* detector = LoadGas(folder);
    
    // Setup detector parameters
    std::vector<float> gemInfo = SetupInfo(folder);
    
    // Run gain simulation
    Gain(detector, gemInfo, "gain_results.txt", 100, 1000);
    
    // Analyze results
    ReadTXTGain("gain_results", true);
    
    return 0;
}
```

### 2. Generate Custom Geometry

```python
# Edit NewGeometry/gen.py parameters
FOLDER_NAME = 'my_gem'
DISTANCE_HOLES = [0.140]  # 140 μm hole pitch
RADIUS_UPPER_PLA = [0.035]  # 35 μm hole radius
DELTA_V = [420]  # 420V GEM voltage

# Run geometry generation
python3 NewGeometry/gen.py
```

### 3. Build and Run Examples

```bash
# Navigate to example directory
cd Examples/Triple-GEM

# Build simulation
make

# Run simulation
./3gem
```

## Available Examples

### Single GEM Studies
- **`gem_example/`** - Basic single GEM detector simulation
- **`PrimaryIon/`** - Primary ionization tracking and analysis

### Multi-GEM Configurations  
- **`Triple-GEM/`** - Three-stage GEM amplification system
- **`SignalOut/`** - Signal formation and readout studies

### Particle Sources
- **`PionLaunch/`** - Charged pion tracking simulation
- Fe-55 X-ray source simulation (built into analysis functions)

## Key Functions Reference

### Simulation Functions (GemSampa.hh)
```cpp
// Load detector geometry and gas properties
ComponentElmer* LoadGas(std::string folder, double percent=70.);

// Calculate detector gain
void Gain(ComponentElmer* detector, std::vector<float> info, 
          std::string output, int sizeLimit=10, int events=100);

// Energy resolution studies
void EnergyResolution(ComponentElmer* detector, std::vector<float> info,
                     std::string output, double energy, int events=1);

// Position resolution analysis  
void PositionResolution(ComponentElmer* detector, double info[9],
                       std::string output, double energy, int events=1);
```

### Analysis Functions (GemAnalysis.hh)
```cpp
// Analyze gain data from text files
void ReadTXTGain(std::string folder, bool draw=false);

// Energy resolution analysis
void ReadTXTEnergyResolution(std::string folder, bool draw=false);

// Position resolution analysis
void ReadTXTPositionResolution(std::string folder, int nBins);

// Visualize electric fields
void PlotElectricPotential(std::string folder);
```

## Gas Mixtures

The framework supports various gas mixtures commonly used in GEM detectors:

- **Ar/CO₂** mixtures (default: 70/30)
- **Penning gas** mixtures with customizable transfer rates
- **Temperature and pressure** control (293.15K, 760 Torr default)

## Output and Analysis

### Generated Files
- **`.txt`** - Raw simulation data (gain, positions, energies)
- **`.root`** - ROOT histograms and analysis results  
- **`.pdf`** - Visualization plots and field maps

### Typical Analysis Workflow
1. Run simulation to generate raw data
2. Use analysis functions to create histograms
3. Extract physics parameters (gain, resolution, etc.)
4. Generate publication-quality plots

## Contributing

When contributing to this project:

1. Follow the existing code style and structure
2. Document new functions with clear comments
3. Add examples for new simulation capabilities
4. Test changes with provided example configurations

## Acknowledgments

This simulation framework is built upon:
- **Garfield++** - The foundation for all detector physics simulations
- **ROOT** - Data analysis and visualization framework  
- **Elmer** - Finite element field calculations
- **CERN detector physics community** - Methods and validation

## License

This project follows academic open-source principles. Please cite appropriately when using this code in scientific publications.

---

For detailed physics background on GEM detectors, see:
- F. Sauli, "GEM: A new concept for electron amplification in gas detectors", *Nucl. Instrum. Methods Phys. Res. A* **386** (1997) 531-534
- The Garfield++ collaboration, "Garfield++ simulation of gaseous detectors", *arXiv:1707.09262*
