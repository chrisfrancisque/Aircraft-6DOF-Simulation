# Aircraft 6-DoF Simulator (C++20)

A small C++20 project that simulates rigid-body dynamics in a flat-earth frame. Includes both a command-line version with matplotlib plotting and an interactive 3D web frontend using WebAssembly and Three.js.

Features:
- A 12-state 6-DoF equations-of-motion (EoM) function
- Standard atmosphere (USSA-1976) properties
- Several numerical integrators (Forward Euler, AB2, RK4)
- Example "vehicles" modeled as spheres/bricks with drag approximations
- Command-line `main_program.cpp` that sets ICs, integrates, and plots
- Interactive 3D web visualization with real-time physics

---

## Project Structure
```
.
├── flat_earth_eom.cpp / .h        # 12-state EoM (body rates, Euler angles, NED pos)
├── numerical_integration_methods.cpp / .h  # Forward Euler, Adams-Bashforth 2, RK4
├── ussa1976.cpp / .h              # Atmosphere (temperature, pressure, rho, a, μ, etc.)
├── spheres.cpp / .h               # "Vehicle" presets + simple aero/drag helpers
├── matplotlibcpp.h                # Header-only plotting bridge (to Python/matplotlib)
├── main_program.cpp               # Example: sets ICs, integrates, plots
├── wasm_wrapper.cpp               # WebAssembly bindings for browser use
└── web/                           # Web frontend
    ├── index.html
    ├── style.css
    ├── src/main.js                # Three.js 3D visualization
    ├── simulation.js              # Compiled WASM module
    └── simulation.wasm            # Compiled WASM binary
```

---

## Web Frontend

The web frontend provides an interactive 3D visualization of the simulation running in your browser.

### Running Locally

1) Install dependencies:
```bash
cd web
npm install
```

2) Start the development server:
```bash
npm run dev
```

3) Open http://localhost:5173 in your browser.

### Features
- Select vehicle type (Tumbling Brick, Bowling Ball, Blueberry)
- Adjust starting altitude (100m - 20,000m)
- Adjustable playback speed
- Real-time display of altitude, velocity, Mach number
- 3D visualization with camera tracking
- Bounce physics on ground impact

### Recompiling WebAssembly

If you modify the C++ simulation code, recompile the WASM:
```bash
source ~/emsdk/emsdk_env.sh
emcc -std=c++20 -O2 \
    wasm_wrapper.cpp \
    flat_earth_eom.cpp \
    numerical_integration_methods.cpp \
    ussa1976.cpp \
    spheres.cpp \
    -I. \
    -lembind \
    -o web/simulation.js \
    -s MODULARIZE=1 \
    -s EXPORT_NAME="createSimulationModule" \
    -s ALLOW_MEMORY_GROWTH=1
```

Requires [Emscripten](https://emscripten.org/docs/getting_started/downloads.html).

---

## Model Summary

### State Vector (length 12)
`x = [u, v, w, p, q, r, φ, θ, ψ, p1, p2, p3]` where
- `u, v, w` (m/s): body-fixed translational velocities  
- `p, q, r` (rad/s): body rates (roll, pitch, yaw)  
- `φ, θ, ψ` (rad): Euler angles (roll, pitch, yaw)  
- `p1, p2, p3` (m): NED position (north, east, down; here `p3` is down)

### Forces/Environment
- USSA-1976 to compute `ρ`, `a` (speed of sound), viscosity, etc.
- Simple drag models for spheres/bricks (selectable "vehicle" presets)
- Gravity applied in flat-earth frame

### Integrators
- Forward Euler (explicit)
- Adams-Bashforth 2 (AB2)
- Classical 4th-order Runge-Kutta (RK4)

---

## Command-Line Build

### Dependencies
- **C++20** (GCC/Clang/MSVC)
- **Python 3** with **development headers** (so `python3-config` works)
- **NumPy** (`pip install numpy`)
- **matplotlib** (`pip install matplotlib`)

> On macOS (Homebrew):
```bash
brew install cmake python
pip3 install numpy matplotlib --break-system-packages
```
> On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake python3-dev python3-pip
python3 -m pip install numpy matplotlib
```

### Option A: CMake (recommended)

1) Configure & build:
```bash
cmake -S . -B build
cmake --build build --config Release
```
2) Run:
```bash
./build/bin/flat_earth_sim
```

### Option B: One-liner g++/clang++ build
```bash
g++ -std=c++20 -O2 \
  main_program.cpp flat_earth_eom.cpp numerical_integration_methods.cpp ussa1976.cpp spheres.cpp \
  -I. $(python3-config --includes) \
  $(python3 -c "import numpy; print('-I' + numpy.get_include())") \
  $(python3-config --ldflags) \
  -o sim
./sim
```

---

## Extending

- **Vehicles**: Add a new function in `spheres.cpp` returning an `unordered_map<string,double>` with keys like `m_kg`, `Jxx_b_kgm2`, `Aref_m2`, and any aero coefficients you use in `flat_earth_eom`.
- **Aerodynamics**: Implement additional stability/derivative terms and call them from `flat_earth_eom.cpp`.
- **Integrators**: Drop in more schemes (e.g., RKF45) into `numerical_integration_methods.cpp` following the existing signatures.

---

## Quick Checks

- Units are SI throughout (meters, seconds, kilograms, radians).
- NED convention (`p3` positive **down**).
- Angle wrapping and singularities (e.g., `tan(θ)`) are handled simply—consider robust handling for extreme attitudes.

---

## Troubleshooting

- **Linker can't find Python / NumPy**: ensure `python3-dev` (Linux) or the Python framework (macOS) is installed and that `python3-config` exists. Try `python3 -m pip install --upgrade pip setuptools wheel`.
- **NumPy headers not found**: The `CMakeLists.txt` automatically detects NumPy's include path. If building manually, add `$(python3 -c "import numpy; print('-I' + numpy.get_include())")` to your compile flags.
- **Matplotlib import errors**: verify `python3 -c "import matplotlib, numpy; print('OK')"` works.
- **Plots freeze on servers**: set `MPLBACKEND=Agg` and save figures instead of showing them.
- **Multiple Python versions on macOS**: Ensure `python3` and `pip3` point to the same Python version. Check with `python3 --version` and `pip3 --version`.
- **WASM not loading in browser**: Check the browser console for errors. Ensure `simulation.js` and `simulation.wasm` are in the `web/` folder.


---