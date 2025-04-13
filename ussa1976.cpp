#include <iostream>
#include <cmath>
#include <unordered_map>
#include <string>
#include <numbers>


const double R = 8.314462618;  // Universal gas constant (J/(mol*K))
const double M = 0.0289644;    // Molar mass of Earth's air (kg/mol)
const double g = 9.80665;      // Gravitational acceleration (m/s^2)
const double T0 = 288.15;      // Sea level standard temperature (K)
const double P0 = 101325;      // Sea level standard pressure (Pa)
const double L = 0.0065;       // Temperature lapse rate (K/m)
const double R_specific = R / M; // Specific gas constant for dry air (J/(kg*K))

std::unordered_map<std::string, double> computeProperties(double altitude) {
    std::unordered_map<std::string, double> properties;

    // Constants
    const double kB = 1.380649e-23; // Boltzmann constant (J/K)
    const double sigma = 3.65e-10;  // Effective diameter of air molecule (m)

    double temperature, pressure;

    // Temperature and pressure based on altitude
    if (altitude < 11000) { // Troposphere
        temperature = T0 - L * altitude;
        pressure = P0 * std::pow((1 - L * altitude / T0), (g / (R_specific * L)));
    }
    else { // Simplified for altitudes above 11 km
        temperature = 216.65;
        pressure = P0 * std::exp(-g * (altitude - 11000) / (R_specific * temperature));
    }

    // Derived properties
    double air_density = pressure / (R_specific * temperature);
    double air_molar_volume = R * temperature / pressure;
    double number_density = air_density / M;
    double air_number_density = pressure / (kB * temperature);
    double pressure_scale_height = R_specific * temperature / g;
    double particles_mean_speed = std::sqrt((8 * kB * temperature) / (M * 1e3)) / std::sqrt(std::numbers::pi);
    double particles_mean_free_path = kB * temperature / (std::sqrt(2) * std::numbers::pi * sigma * sigma * pressure);
    double particles_collision_frequency = particles_mean_speed / particles_mean_free_path;
    double speed_of_sound = std::sqrt(1.4 * R_specific * temperature);
    double dynamic_viscosity = 1.458e-6 * std::pow(temperature, 1.5) / (temperature + 110.4);
    double kinematic_viscosity = dynamic_viscosity / air_density;
    double thermal_conductivity = (dynamic_viscosity * 1005) / 0.72; // Assuming Prandtl number of 0.72

    // Store properties in the map
    properties["temperature"] = temperature;
    properties["pressure"] = pressure;
    properties["air_density"] = air_density;
    properties["air_molar_volume"] = air_molar_volume;
    properties["number_density"] = number_density;
    properties["air_number_density"] = air_number_density;
    properties["pressure_scale_height"] = pressure_scale_height;
    properties["particles_mean_speed"] = particles_mean_speed;
    properties["particles_mean_free_path"] = particles_mean_free_path;
    properties["particles_collision_frequency"] = particles_collision_frequency;
    properties["speed_of_sound"] = speed_of_sound;
    properties["dynamic_viscosity"] = dynamic_viscosity;
    properties["kinematic_viscosity"] = kinematic_viscosity;
    properties["thermal_conductivity"] = thermal_conductivity;

    return properties;
}