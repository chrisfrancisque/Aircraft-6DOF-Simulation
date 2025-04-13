#ifndef USSA1976_H
#define USSA1976_H

#include <unordered_map>
#include <string>

// Constants
const double R = 8.314462618;  // Universal gas constant (J/(mol*K))
const double M = 0.0289644;    // Molar mass of Earth's air (kg/mol)
const double g = 9.80665;      // Gravitational acceleration (m/s^2)
const double T0 = 288.15;      // Sea level standard temperature (K)
const double P0 = 101325;      // Sea level standard pressure (Pa)
const double L = 0.0065;       // Temperature lapse rate (K/m)
const double R_specific = R / M; // Specific gas constant for dry air (J/(kg*K))

// Function Declaration
std::unordered_map<std::string, double> computeProperties(double altitude);

#endif // USSA1976_H