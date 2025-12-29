#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include "ussa1976.h"
#include "spheres.h"

std::vector<double> flat_earth_eom(double t, const std::vector<double> x, const std::unordered_map<std::string, double>& amod, std::unordered_map<std::string, double> airmod)
{
	/*  flat_earth_eom.cpp contains the essential elements of a 6 degree of freedom
		simualation. The purpose of this function is to allow the numerical approximation of 
		solutions of the governing equations for an aircraft.

		The naming convention is <variable name>_<coordinate_system>_units. For example
		the pitch rate q, resolved in the body fixed frame, b, with units of radians 
		per second is named, q_b_rps

		Arguments:

		t - time [s], scalar

		x - state vector at time t [various units], std::vector

			x[0] = u_b_mps, axial velocity of CM(center of mass) wrt(With respect to) intertial CS(coordinate sytem
			resolved in the aircraft body fixed CS

			x[1] = v_b_mps, lateral velocity of CM wrt interial CS resolved in aircraft body fixed CS

			x[2] = w_b_mps, vertical veloctiy of CM wrt inertial CS resolved in aircraft body fixed CS 

			x[3] = p_b_rps, roll angular veloctiy of body fixed CS wrt inertial CS

			x[4] = q_b_rps, pitch angular veloctiy of body fixed CS wrt inertial CS 

			x[5] = r_b_rps, yaw angular veloctiy of body fixed CS wrt inertial CS

			x[6] = phi_rad, roll angle

			x[7] = theta_rad, pitch angle

			x[8] = psi_rad, yaw angle

			x[9] = p1_n_m, x-axis position of aircraft resolved in NED(north, east, down) CS

			x[10] = p2_n_m, y-axis position of aircraft resolved in NED CS

			x[11] = p3_n_m, z-axis position of aircraft resolved in NED CS

		amod = aircraft model data stored as an unordered_map containing various parameters
		
		airmod = atmosphere and gravity model data stored as an unordered_map

		Returns:

		dx : the time derivative of each state in x (RHS of governing equations)
		
	*/


	// Preallocate left hand side of eqautions
	std::vector<double> dx(12, 0.0);


	// Assign current state values to variable names

	double u_b_mps = x[0];
	double v_b_mps = x[1];
	double w_b_mps = x[2];
	double p_b_rps = x[3];
	double q_b_rps = x[4];
	double r_b_rps = x[5];
	double phi_rad = x[6];
	double theta_rad = x[7];
	double psi_rad = x[8];
	double p1_n_m = x[9];
	double p2_n_m = x[10];
	double p3_n_m = x[11];

	// Compute trigonometric operations on Euler angles
	double c_phi = std::cos(phi_rad);
	double c_theta = std::cos(theta_rad);
	double c_psi = std::cos(psi_rad);
	double s_phi = std::sin(phi_rad);
	double s_theta = std::sin(theta_rad);
	double s_psi = std::sin(psi_rad);
	double t_theta = std::tan(theta_rad);

	// Compute Direction Cosine Matrix
	double C_b2n_11 = c_theta * c_psi;
	double C_b2n_12 = -c_phi * s_psi + s_phi * s_theta * c_psi;
	double C_b2n_13 = s_phi * s_psi + c_phi * s_theta * c_psi;
	double C_b2n_21 = c_theta * s_psi;
	double C_b2n_22 = c_phi * c_psi + s_phi * s_theta * s_psi;
	double C_b2n_23 = -s_phi * c_psi + c_phi * s_theta * s_psi;
	double C_b2n_31 = -s_theta;
	double C_b2n_32 = s_phi * c_theta;
	double C_b2n_33 = c_phi * c_theta;
	double C_n2b_13 = C_b2n_31;
	double C_n2b_23 = C_b2n_32;
	double C_n2b_33 = C_b2n_33;
	


	// Get mass and Moments of inertia
	double m_kg = amod.at("m_kg"); // THESE MIGHT NEED FIXING DEPENDING ON WHAT AMOD IS
	double Jxz_b_kgm2 = amod.at("Jxz_b_kgm2");
	double Jxx_b_kgm2 = amod.at("Jxx_b_kgm2");
	double Jyy_b_kgm2 = amod.at("Jyy_b_kgm2");
	double Jzz_b_kgm2 = amod.at("Jzz_b_kgm2");

	// Get reference dimensions
	double A_ref_m2 = amod.at("Aref_m2");
	double b_m = amod.at("b_m");
	double c_m = amod.at("c_m");

	// Aerodynamic Coefficients
	double Clp = amod.at("Clp");
	double Clr = amod.at("Clr");
	double Cmq = amod.at("Cmq");
	double Cnp = amod.at("Cnp");
	double Cnr = amod.at("Cnr");
	

	// US Standard Atmosphere 1976
	std::unordered_map<std::string, double> atmosphere = computeProperties(-p3_n_m);

	double rho_kgpm3 = atmosphere.at("air_density");
	double c_mp2 = atmosphere.at("speed_of_sound");


   // Air data calculation (Mach, altitude, AoA, AoS)
	double true_airspeed_mps = std::sqrt
	(std::pow(u_b_mps, 2) + std::pow(v_b_mps, 2) + std::pow(w_b_mps, 2));

	double qbar_kgpms2 = 0.5 * rho_kgpm3 * std::pow(true_airspeed_mps, 2);

	double w_over_u{};

	if (u_b_mps == 0 && w_b_mps == 0) // avoid division by 0 error in AoA and AoS
	{
		w_over_u = 0;
	}
	
	else
	{
		w_over_u = w_b_mps / u_b_mps;
	}

	double v_over_VT{};
	
	if (true_airspeed_mps == 0 && v_b_mps == 0)
	{
		v_over_VT = 0;
	}
	
	else
	{
		v_over_VT = v_b_mps / true_airspeed_mps;
	}

	double Mach = true_airspeed_mps / c_mp2;
	double alpha_rad = std::atan2(w_b_mps, u_b_mps);
	double beta_rad = std::asin(v_over_VT);


	// Trig Calculations for body to wind/stability axes DCM
	double s_alpha = std::sin(alpha_rad);
	double c_alpha = std::cos(alpha_rad);
	double s_beta = std::sin(beta_rad);
	double c_beta = std::cos(beta_rad);
	double C_w2b_11 = c_alpha * c_beta;
	double C_w2b_12 = -c_alpha * s_beta;
	double C_w2b_13 = -s_alpha;
	double C_w2b_21 = s_beta;
	double C_w2b_22 = c_beta;
	double C_w2b_23 = 0;
	double C_w2b_31 = s_alpha * c_beta;
	double C_w2b_32 = -s_alpha * s_beta;
	double C_w2b_33 = c_alpha;
   

   // Gravity acts normal to earth tangent CS
	const double gz_n_mps2 = 9.81;

	// Resolve gravity in body coordinate system
	double gx_b_mps2 = C_n2b_13 * gz_n_mps2;
	double gy_b_mps2 = C_n2b_23 * gz_n_mps2;
	double gz_b_mps2 = C_n2b_33 * gz_n_mps2;


	// Aerodynamic Forces
	double drag_kgmps2 = amod.at("CD_approx") * qbar_kgpms2 * amod.at("Aref_m2");
	double side_kgmps2 = 0.0;
	double lift_kgmps2 = 0.0;


	// External Forces 
	double Fx_b_kgmps2 = -(C_w2b_11 * drag_kgmps2 + C_w2b_12 * side_kgmps2 + C_w2b_13 * lift_kgmps2);
	double Fy_b_kgmps2 = -(C_w2b_21 * drag_kgmps2 + C_w2b_22 * side_kgmps2 + C_w2b_23 * lift_kgmps2);
	double Fz_b_kgmps2 = -(C_w2b_31 * drag_kgmps2 + C_w2b_32 * side_kgmps2 + C_w2b_33 * lift_kgmps2);

	// External Moments
	double l_b_kgm2ps2 = Cl_brick(Clp, Clr, p_b_rps, r_b_rps, b_m, true_airspeed_mps) * qbar_kgpms2 * A_ref_m2 * b_m;
	double m_b_kgm2ps2 = Cm_brick(Cmq, q_b_rps, c_m, true_airspeed_mps) * qbar_kgpms2 * A_ref_m2 * c_m;
	double n_b_kgm2ps2 = Cn_brick(Cnp, Cnr, p_b_rps, r_b_rps, b_m, true_airspeed_mps) * qbar_kgpms2 * A_ref_m2 * b_m;


	// Denominator in roll and yaw rate equations
	double Den = Jxx_b_kgm2 * Jzz_b_kgm2 - std::pow(Jxz_b_kgm2, 2);

	// x-axis (roll axis) velocity equation
	// State: u_b_mps
	dx[0] = (1.0 / m_kg) * Fx_b_kgmps2 + gx_b_mps2 - w_b_mps * q_b_rps + v_b_mps * r_b_rps;

	//y-axis (pitch axis) velocity equation
	// state : v_b_mps

	dx[1] = (1.0 / m_kg) * Fy_b_kgmps2 + gy_b_mps2 - u_b_mps * r_b_rps + w_b_mps * p_b_rps;

	// z-axis (yaw-axis) velocity equation
	// state : w_b_mps

	dx[2] = (1.0 / m_kg) * Fz_b_kgmps2 + gz_b_mps2 - v_b_mps * p_b_rps + u_b_mps * q_b_rps;


	// Roll equation
	// state: p_b_rps

	dx[3] = (Jzz_b_kgm2 * (Jxx_b_kgm2 - Jyy_b_kgm2 + Jzz_b_kgm2) * p_b_rps * q_b_rps -
		Jzz_b_kgm2 * (Jzz_b_kgm2 * (Jzz_b_kgm2 - Jyy_b_kgm2) + std::pow(Jxz_b_kgm2, 2)) *
		q_b_rps * r_b_rps + Jzz_b_kgm2 * l_b_kgm2ps2 + Jxz_b_kgm2 * n_b_kgm2ps2) / Den ;

	// Pitch equation
	// State: q_b_rps

	dx[4] = ((Jzz_b_kgm2 - Jxx_b_kgm2) * p_b_rps * r_b_rps - Jxz_b_kgm2 *
		(std::pow(p_b_rps, 2) - std::pow(r_b_rps, 2)) + m_b_kgm2ps2) / Jyy_b_kgm2;

	// yaw equation
	// state :r_b_rps

	dx[5] = ((Jzz_b_kgm2 * (Jxx_b_kgm2 - Jyy_b_kgm2) + std::pow(Jxz_b_kgm2, 2)) *
		p_b_rps * q_b_rps + Jxz_b_kgm2 * (Jxx_b_kgm2 - Jyy_b_kgm2 + Jzz_b_kgm2) *
		q_b_rps * r_b_rps + Jxz_b_kgm2 * l_b_kgm2ps2 + Jxz_b_kgm2 * n_b_kgm2ps2) / Den ;

	// Kinematic Equations
	dx[6] = p_b_rps + std::sin(phi_rad) * std::tan(theta_rad) * q_b_rps +
		std::cos(phi_rad) * std::tan(theta_rad) * r_b_rps;

	dx[7] = std::cos(phi_rad) * q_b_rps - std::sin(phi_rad) * r_b_rps;

	dx[8] = std::sin(phi_rad)/std::cos(theta_rad) * q_b_rps + 
		std::cos(phi_rad)/std::cos(theta_rad) * r_b_rps ;

	// Position (Navagation) equations

	dx[9] = c_theta*c_psi*u_b_mps + (-c_phi*s_psi + s_phi*s_theta*c_psi) * v_b_mps
		  + (s_phi*s_psi + c_phi*s_theta*c_psi)* w_b_mps ;

	dx[10] = c_theta * s_psi * u_b_mps + (c_phi*c_psi+s_phi*s_theta*s_psi) * v_b_mps 
		   + (-s_phi*c_psi + c_phi*s_theta*s_psi) * w_b_mps;

	dx[11] = -s_theta * u_b_mps + s_phi*c_theta*v_b_mps + c_phi*c_theta*w_b_mps;
	 
	return dx;








	









}