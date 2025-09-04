# GEM-simulation

A comprehensive simulation framework for Gas Electron Multiplier (GEM) detectors using the Garfield++ library. This toolkit provides detailed particle tracking, field calculations, and performance analysis for single and multi-GEM detector configurations

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

---

This code was developed as part of the undergrad research project *Simulation of Particle Detector Performance: Validation of GEM Detector Simulation* (2016-2018) supervised by Prof. Dr. Marcelo Munhoz at the University of São Paulo, funded by the university and the National Council for Scientific and Technological Development (CNPq)

This READme was written by Copilot
