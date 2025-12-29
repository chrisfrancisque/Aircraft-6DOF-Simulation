#include <emscripten/bind.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>

#include "flat_earth_eom.h"
#include "numerical_integration_methods.h"
#include "ussa1976.h"
#include "spheres.h"

using namespace emscripten;

// Store simulation results
struct SimulationResult {
    std::vector<double> time;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> roll;
    std::vector<double> pitch;
    std::vector<double> yaw;
    std::vector<double> velocity;
    std::vector<double> mach;
};

SimulationResult g_result;

// Run the simulation and store results
void runSimulation(
    std::string vehicleType,
    double altitude,
    double u0, double v0, double w0,
    double p0, double q0, double r0,
    double phi0, double theta0, double psi0,
    double duration,
    double timeStep
) {
    // Clear previous results
    g_result = SimulationResult();

    // Select vehicle
   std::unordered_map<std::string, double> amod = NASA_Atmos03_Brick();

    // Initial conditions
    std::vector<double> x0 = {
        u0, v0, w0,           // velocities
        p0, q0, r0,           // angular rates
        phi0, theta0, psi0,   // Euler angles
        0.0, 0.0, -altitude   // position (NED, so altitude is negative)
    };

    // Time vector
    std::vector<double> t_s;
    for (double t = 0; t <= duration; t += timeStep) {
        t_s.push_back(t);
    }

    std::size_t nt_s = t_s.size();
    std::size_t nx0 = x0.size();

    // Preallocate solution array
    std::vector<std::vector<double>> x(nx0, std::vector<double>(nt_s));

    // Set initial conditions
    for (std::size_t i = 0; i < nx0; ++i) {
        x[i][0] = x0[i];
    }

    // Atmosphere
    std::unordered_map<std::string, double> atmosphere = computeProperties(altitude);
    std::unordered_map<std::string, double> airmod = {
        {"alt_m", altitude},
        {"rho_kgpm3", atmosphere.at("air_density")},
        {"c_mps", atmosphere.at("speed_of_sound")},
        {"g_mps2", 9.81}
    };

    // Run integration
    auto [ut_s, ux] = forward_euler(flat_earth_eom, t_s, x, timeStep, amod, airmod);

    // Store results
    double speed_of_sound = atmosphere.at("speed_of_sound");

    for (std::size_t i = 0; i < nt_s; ++i) {
        g_result.time.push_back(ut_s[i]);
        g_result.x.push_back(ux[9][i]);       // North
        g_result.y.push_back(-ux[11][i]);     // Altitude (convert from down to up)
        g_result.z.push_back(ux[10][i]);      // East
        g_result.roll.push_back(ux[6][i]);    // phi
        g_result.pitch.push_back(ux[7][i]);   // theta
        g_result.yaw.push_back(ux[8][i]);     // psi

        double vel = std::sqrt(
            ux[0][i] * ux[0][i] +
            ux[1][i] * ux[1][i] +
            ux[2][i] * ux[2][i]
        );
        g_result.velocity.push_back(vel);
        g_result.mach.push_back(vel / speed_of_sound);
    }
}

// Accessor functions for JavaScript
int getResultLength() {
    return g_result.time.size();
}

double getTime(int i) { return g_result.time[i]; }
double getX(int i) { return g_result.x[i]; }
double getY(int i) { return g_result.y[i]; }
double getZ(int i) { return g_result.z[i]; }
double getRoll(int i) { return g_result.roll[i]; }
double getPitch(int i) { return g_result.pitch[i]; }
double getYaw(int i) { return g_result.yaw[i]; }
double getVelocity(int i) { return g_result.velocity[i]; }
double getMach(int i) { return g_result.mach[i]; }

// Bind functions to JavaScript
EMSCRIPTEN_BINDINGS(simulation_module) {
    function("runSimulation", &runSimulation);
    function("getResultLength", &getResultLength);
    function("getTime", &getTime);
    function("getX", &getX);
    function("getY", &getY);
    function("getZ", &getZ);
    function("getRoll", &getRoll);
    function("getPitch", &getPitch);
    function("getYaw", &getYaw);
    function("getVelocity", &getVelocity);
    function("getMach", &getMach);
}