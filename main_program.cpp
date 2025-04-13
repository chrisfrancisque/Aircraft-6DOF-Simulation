// main_program.cpp : This file is meant to input data, run the simulation, and plot the results
//

#include "numerical_integration_methods.h"
#include "flat_earth_eom.h"
#include "matplotlibcpp.h"
#include "ussa1976.h"
#include "spheres.h"

namespace plt = matplotlibcpp;


int main()
{
    /*
    Part 1: Initialization of simulation
    */


    // Conversions
    double r2d = 180 / std::numbers::pi;
    double d2r = 1 / r2d;


    // Define Vehicle (A Sphere for now)

    double r_sphere_m = 0.08;
    double m_sphere_kg = 5;
    double J_sphere_kgm2 = 0.4 * m_sphere_kg * std::pow(r_sphere_m, 2);

    //std::unordered_map<std::string, double> amod = {
    //    {"m_kg", 1.0 }, {"Jxz_b_kgm2", 0}, {"Jxx_b_kgm2", J_sphere_kgm2}, {"Jyy_b_kgm2", J_sphere_kgm2},
    //    {"Jzz_b_kgm2", J_sphere_kgm2} }; // aircraft model properties

    

    double u0_bf_mps = 0.001;
    double v0_bf_mps = 0.0;
    double w0_bf_mps = 0.0;
    double p0_bf_rps = 10.0 * d2r;
    double q0_bf_rps = 20.0 * d2r;
    double r0_bf_rps = 30.0 * d2r;
    double phi0_rad = 0 * d2r;
    double theta0_rad = 0.0 * d2r;
    double psi0_rad = 0.0 * d2r;
    double p10_n_m = 0.0;
    double p20_n_m = 0.0;
    double p30_n_m = -30000.0 / 3.28;

    // Assign initial conditions to an array

    std::vector<double> x0 = {
      u0_bf_mps, // x-axis body-fixed velocity (m/s)
      v0_bf_mps, // y-axis body-fixed velocity (m/s)
      w0_bf_mps, // z-axis body-fixed velocity (m/s)
      p0_bf_rps, // roll rate (rad/s)
      q0_bf_rps, // pitch rate (rad/s)
      r0_bf_rps, // yaw rate (rad/s)
      phi0_rad, // roll angle (rad)
      theta0_rad, // pitch angle (rad)
      psi0_rad, // yaw angle (rad)
      p10_n_m, // x-axis position (N*m)
      p20_n_m, // y-axis position (N*m)
      p30_n_m // z-axis position (N*m)
    };

    // Get number of elements in x0
    std::size_t nx0 = x0.size();

    // Set Time conditions
    double t0_s = 0.0; // Intial
    double tf_s = 30; // Final
    double h_s = 0.01; // Time step

    // Set Atmospheric Data
    std::unordered_map<std::string, double> atmosphere = computeProperties(-p30_n_m);

    double alt_m = -p30_n_m;
    double rho_kgpm3 = atmosphere.at("air_density");
    double c_mps = atmosphere.at("speed_of_sound");
    double g_mps2 = 9.81;

    std::unordered_map<std::string, double> airmod = { {"alt_m", alt_m }, {"rho_kgpm3", rho_kgpm3},
        {"c_mps", c_mps}, {"g_mps2", g_mps2} };

    // Define Vehicle
    // std::unordered_map<std::string, double> amod = BlueBerry(); // Need to figure out proper data type
    std::unordered_map <std::string, double> amod = NASA_Atmos03_Brick();

   // std::cout << "The analytical terminal velocity is " << amod.at("Vterm_mps") << " m/s. \n";





    /*
    Part 2: Numerically approximate solutions to the governing equations
    */

    // Preallocate the solution array
    std::vector<double> t_s; // creating empty solution vector

    for (double t = t0_s; t <= tf_s; t += h_s)  // Assigning size of vector using start, end and time step
    {
        t_s.push_back(t);
    }

    std::size_t nt_s = t_s.size();

    std::vector<std::vector<double>> x(nx0, std::vector<double>(nt_s)); // making a 2d solution array with proper size

    // Assign the initial conditions, x0, to solution array x
    for (std::size_t i = 0; i < x.size(); ++i)
    {
        x[i][0] = x0[i];
    }

    // Perform forward Euler integration
    const auto [ut_s, ux] = forward_euler(flat_earth_eom, t_s, x, h_s, amod, airmod); // THIS MAY NEED FIXING

    // data post-processing actions

    

    // Airspeed
    std::vector<double> True_Airspeed_mps(nt_s, 0.0); // Initialize a vector with nt_s elements, all set to 0.0

    for (std::size_t i = 0; i < nt_s; ++i) {
        True_Airspeed_mps[i] = std::sqrt(std::pow(ux[0][i], 2) + std::pow(ux[1][i], 2) + std::pow(ux[2][i], 2));
    }

    // Angle of attack
    std::vector<double> Alpha_rad(nt_s, 0.0); // Initialize a vector with nt_s elements, all set to 0.0

    for (std::size_t i = 0; i < nt_s; ++i) {
        double w_over_v = 0.0;


        if (ux[0][i] == 0.0 && ux[2][i] == 0.0)
        {
            w_over_v = 0.0; // To avoid division by zero
        }
        else
        {
            w_over_v = ux[2][i] / ux[0][i];
        }

        Alpha_rad[i] = std::atan(w_over_v); // Compute the angle of attack in radians
    }


    // Angle of side slip
    std::vector<double> Beta_rad(nt_s, 0.0);

    for (std::size_t i = 0; i < nt_s; ++i)
    {
        double v_over_VT = 0.0;

        if (ux[1][i] == 0 && True_Airspeed_mps[i] == 0)
        {
            v_over_VT = 0;
        }

        else
        {
            v_over_VT = x[1][i] / True_Airspeed_mps[i];
        }
        Beta_rad[i] = std::asin(v_over_VT);
    }


    // Mach Number
    std::vector<double> Mach(nt_s, 0.0);

    for (std::size_t i = 0; i < nt_s; ++i)
    {
        Mach[i] = True_Airspeed_mps[i] / c_mps;
    }

     std::cout << "The numerical ternimal velocity is " << ux[0].back() << " m/s. \n" ;
    /*
    Part 3: Plot Data
    */

    std::cout << "hi";
    std::cout << "t_s size: " << ut_s.size() << "\n";
    for (std::size_t i = 0; i < ux.size(); ++i) {
        std::cout << "x[" << i << "] size: " << ux[i].size() << "\n";
    }

    // Figure 1. tanslational and rotational states
    plt::figure(1);


    // Subplot 1: Axial Velocity u^b_CM/n
    plt::subplot(2, 4, 1);
    plt::plot(ut_s, ux[0], { {"color", "red"} });
    plt::xlabel("Time [s]", { {"color", "black"} });
    plt::ylabel("u [m/s]", { {"color", "black"} });
    plt::grid(true);

    // // Subplot 2: y-axis velocity v^b_CM/n
    plt::subplot(2, 4, 2);
    plt::plot(ut_s, ux[1], { {"color", "red"} });
    plt::xlabel("Time [s]", { {"color", "black"} });
    plt::ylabel("v [m/s]", { {"color", "black"} });
    plt::grid(true);

    // // Subplot 3: z-axis velocity w^b_CM/n
    plt::subplot(2, 4, 3);
    plt::plot(ut_s, ux[2], { {"color", "red"} });
    plt::xlabel("Time [s]", { {"color", "black"} });
    plt::ylabel("w [m/s]", { {"color", "black"} });
    plt::grid(true);

    // // Subplot 4: Roll angle, phi
    plt::subplot(2, 4, 4);
    plt::plot(ut_s, ux[6], { {"color", "yellow"} });
    plt::xlabel("Time [s]", { {"color", "black"} });
    plt::ylabel("phi [rad]", { {"color", "black"} });
    plt::grid(true);

    // // Subplot 5: Roll rate p^b_b/n
    plt::subplot(2, 4, 5);
    plt::plot(ut_s, ux[3], { {"color", "blue"} });
    plt::xlabel("Time [s]", { {"color", "black"} });
    plt::ylabel("p [r/s]", { {"color", "black"} });
    plt::grid(true);

    // // Subplot 6: Pitch rate q^b_b/n
    plt::subplot(2, 4, 6);
    plt::plot(ut_s, ux[4], { {"color", "blue"} });
    plt::xlabel("Time [s]", { {"color", "black"} });
    plt::ylabel("q [r/s]", { {"color", "black"} });
    plt::grid(true);

    // // Subplot 7: Yaw rate r^b_b/n
    plt::subplot(2, 4, 7);
    plt::plot(ut_s, ux[5], { {"color", "blue"} });
    plt::xlabel("Time [s]", { {"color", "black"} });
    plt::ylabel("r [r/s]", { {"color", "black"} });
    plt::grid(true);

    // // Subplot 8: Pitch angle, theta
    plt::subplot(2, 4, 8);
    plt::plot(ut_s, ux[7], { {"color", "yellow"} });
    plt::xlabel("Time [s]", { {"color", "black"} });
    plt::ylabel("theta [rad]", { {"color", "black"} });
    plt::grid(true);

    // Adjust layout and show the plot
    plt::tight_layout();
    /*plt::show();*/



    // Figure 2. position states
    plt::figure(2);

    // North position p1^n_CM/T
    plt::subplot(2, 3, 1);
    plt::plot(ut_s, ux[9], { {"color", "cyan"} });
    plt::xlabel("Time [s]", { {"color" , "black"} });
    plt::ylabel("North [m]", { {"color" , "black"} });
    plt::grid(true);

    // East position p^2n_CM/T
    plt::subplot(2, 3, 2);
    plt::plot(ut_s, ux[10], { {"color" , "cyan"} });
    plt::xlabel("Time [s]", { {"color" , "black"} });
    plt::ylabel("East [m]", { {"color" , "black"} });
    plt::grid(true);

    std::vector<double> plotAlt;
    for (const auto& value : ux[11]) {  // Access the 12th row
        plotAlt.push_back(-value); // Negate each value and store it
    }
   

    // Altitude
    plt::subplot(2, 3, 3);
    plt::plot(ut_s, plotAlt, { {"color" , "cyan"} });
    plt::xlabel("Time [s]", { {"color" , "black"} });
    plt::ylabel("Altitude [m]", { {"color" , "black"} });
    plt::grid(true);


    //North vs East position p2^n_CM/T
    plt::subplot(2, 3, 4);
    plt::plot(ux[10], ux[9], { {"color" , "cyan"} });
    plt::xlabel("East [m]", { {"color" , "black"} });
    plt::ylabel("North [m]", { {"color" , "black"} });
    plt::grid(true);

    //Altitude vs East position p2^n_CM/T
    plt::subplot(2, 3, 5);
    plt::plot(ux[10], plotAlt, { {"color" , "cyan"} });
    plt::xlabel("East [m]", { {"color" , "black"} });
    plt::ylabel("Altitude [m]", { {"color" , "black"} });
    plt::grid(true);

    //Altitude vs North 
    plt::subplot(2, 3, 6);
    plt::plot(ux[9], plotAlt, { {"color" , "cyan"} });
    plt::xlabel("North [m]", { {"color" , "black"} });
    plt::ylabel("Altitude [m]", { {"color" , "black"} });
    plt::grid(true);

    // Adjust layout and show the plot
    plt::tight_layout();
    /*plt::show();*/

    // Figure 3. air data
    plt::figure(3);

    // Angle of Attack
    plt::subplot(1, 3, 1);
    std::vector<double> Alpha_deg(Alpha_rad.size());
    std::transform(Alpha_rad.begin(), Alpha_rad.end(), Alpha_deg.begin(),
        [](double alpha) { return alpha * 180.0 / 3.14; });

    plt::plot(ut_s, Alpha_deg, { {"color", "magenta"} });
    plt::xlabel("Time [s]", { {"color" , "black"} });
    plt::ylabel("Angle of Attack [deg]", { {"color" , "black"} });
    plt::ylim(-90, 90);
    plt::grid(true);


    // Angle of side slip
    plt::subplot(1, 3, 2);
    std::vector<double> Beta_deg(Beta_rad.size());
    std::transform(Beta_rad.begin(), Beta_rad.end(), Beta_deg.begin(),
        [](double beta) { return beta * 180.0 / 3.14; });

    plt::plot(ut_s, Beta_deg, { {"color", "magenta"} });
    plt::xlabel("Time [s]", { {"color" , "black"} });
    plt::ylabel("Angle of Side Slip [deg]", { {"color" , "black"} });
    plt::ylim(-90, 90);
    plt::grid(true);

    // Mach Number
    plt::subplot(1, 3, 3);
    plt::plot(ut_s, Mach, { {"color", "magenta"} });
    plt::xlabel("Time [s]", { {"color" , "black"} });
    plt::ylabel("Mach Number", { {"color" , "black"} });
    plt::ylim(-90, 90);
    plt::grid(true);

    // Adjust layout and show the plot
    plt::tight_layout();
    plt::show();






    return 0;


}


