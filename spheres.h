#pragma once
#ifndef SPHERES_H
#define SPHERES_H

#include <tuple>
#include <unordered_map>
#include <string>

// Function to calculate properties of a sphere
std::tuple<double, double, double, double> CalcSphereProps(double r_sphere_m, double rho_sphere_kgpm3);

// Function to return the properties of a 50 Cal Lead Ball
std::unordered_map<std::string, double> Musketball50cal();

// Function to return the properties of a Carronade 12 lb Cannonball
std::unordered_map<std::string, double> Carronade12lb();

// Function to return the properties of a Blueberry
std::unordered_map<std::string, double> BlueBerry();

// Function to return the properties of a Bowling Ball
std::unordered_map<std::string, double> Bowlingball();

std::unordered_map<std::string, double> TsarCannonball();

std::unordered_map<std::string, double> NASA_Atmos01_Sphere();

std::unordered_map<std::string, double> NASA_Atmos02_Brick();

std::unordered_map<std::string, double> NASA_Atmos03_Brick();

double Cl_brick(double Clp, double Clr, double p_b_rps, double r_b_rps, double b_m, double true_airspeed_mps);

double Cm_brick(double Cmq, double q_b_rps, double c_m, double true_airspeed_mps);

double Cn_brick(double Cnp, double Cnr, double p_b_rps, double r_b_rps, double b_m, double true_airspeed_mps);

double sphere_drag(double mach);





#endif // SPHERES_H
