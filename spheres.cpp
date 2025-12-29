#include <cmath>
#include <iostream>
#include <tuple>
#include <numbers>
#include <unordered_map>
#include <string>
#include <any>


std::tuple<double, double, double, double> CalcSphereProps(double r_sphere_m, double rho_sphere_kgpm3)
{ 
	double vol_sphere_m3 = (4.0 / 3.0) * std::numbers::pi * std::pow(r_sphere_m, 3);
	double m_sphere_kg = rho_sphere_kgpm3 * vol_sphere_m3;
	double J_sphere_kgm2 = .4 * m_sphere_kg * std::pow(r_sphere_m, 2);
	double Aref_m2 = std::numbers::pi * std::pow(r_sphere_m, 2);

	return std::tuple(vol_sphere_m3, m_sphere_kg, J_sphere_kgm2, Aref_m2);

}

std::unordered_map<std::string, double> Musketball50cal()
{
	//Inches to meteres conversion
	double in2m = 0.0254;

	std::string vehicle_name = "50 Cal Lead Ball";

	double rho_lead_kgpm3 = 11300.0;

	double r_sphere_in = 0.495;
	
	double r_sphere_m = r_sphere_in * in2m;

	double CD_approx = 0.5;

	auto [vol_sphere_m3, m_sphere_kg, J_sphere_kgm2, Aref_m2] = CalcSphereProps(r_sphere_m, rho_lead_kgpm3);


	double Vterm_mps = std::sqrt((2 * m_sphere_kg * 9.81) / (1.2 * CD_approx * Aref_m2));

	std::unordered_map<std::string, double> amod = {

		
		{"m_kg", m_sphere_kg},
		{"Jxz_b_kgm2", 0.0},
		{"Jxx_b_kgm2", J_sphere_kgm2},
		{"Jyy_b_kgm2", J_sphere_kgm2},
		{"Jzz_b_kgm2", J_sphere_kgm2},
		{"r_sphere_m", r_sphere_m},
		{"m_sphere_kg", m_sphere_kg},
		{"CD_approx", CD_approx},
		{"Aref_m2", Aref_m2},
		{"Vterm_mps", Vterm_mps} };

	 return amod;


}

std::unordered_map<std::string, double> Carronade12lb()
{
	//Inches to meteres conversion
	double in2m = 0.0254;

	std::string vehicle_name = "Carronade 12 lb (5.4 kg) Cannonball";

	double rho_castiron_kgpm3 = 7000.0;

	double r_sphere_in = 4.4;

	double r_sphere_m = r_sphere_in * in2m;

	double CD_approx = 0.5;

	auto [vol_sphere_m3, m_sphere_kg, J_sphere_kgm2, Aref_m2] = CalcSphereProps(r_sphere_m, rho_castiron_kgpm3);

	double Vterm_mps = std::sqrt((2 * m_sphere_kg * 9.81) / (1.2 * CD_approx * Aref_m2));

	std::unordered_map<std::string, double> amod = {

		
		{"m_kg", m_sphere_kg},
		{"Jxz_b_kgm2", 0.0},
		{"Jxx_b_kgm2", J_sphere_kgm2},
		{"Jyy_b_kgm2", J_sphere_kgm2},
		{"Jzz_b_kgm2", J_sphere_kgm2},
		{"r_sphere_m", r_sphere_m},
		{"m_sphere_kg", m_sphere_kg},
		{"CD_approx", CD_approx},
		{"Aref_m2", Aref_m2},
		{"Vterm_mps", Vterm_mps} };

	return amod;


}

std::unordered_map<std::string, double> BlueBerry()
{
	//Inches to meteres conversion
	double in2m = 0.0254;

	std::string vehicle_name = "A Blueberry";

	double rho_blueberry_kgpm3 = 786.0;

	double r_sphere_in = 0.3;

	double r_sphere_m = r_sphere_in * in2m;

	double CD_approx = 0.5;

	auto [vol_sphere_m3, m_sphere_kg, J_sphere_kgm2, Aref_m2] = CalcSphereProps(r_sphere_m, rho_blueberry_kgpm3);

	double Vterm_mps = std::sqrt((2 * m_sphere_kg * 9.81) / (1.2 * CD_approx * Aref_m2));

	std::unordered_map<std::string, double> amod = {

		
		{"m_kg", m_sphere_kg},
		{"Jxz_b_kgm2", 0.0},
		{"Jxx_b_kgm2", J_sphere_kgm2},
		{"Jyy_b_kgm2", J_sphere_kgm2},
		{"Jzz_b_kgm2", J_sphere_kgm2},
		{"r_sphere_m", r_sphere_m},
		{"m_sphere_kg", m_sphere_kg},
		{"CD_approx", CD_approx},
		{"Aref_m2", Aref_m2},
		{"Vterm_mps", Vterm_mps} };

	
	return amod;


}


std::unordered_map<std::string, double> Bowlingball()
{
	//Inches to meteres conversion
	double in2m = 0.0254;

	std::string vehicle_name = "Bowling Ball";

	double rho_bowlingball_kgpm3 = 1500.0;

	double r_sphere_in = 4.40;

	double r_sphere_m = r_sphere_in * in2m;

	double CD_approx = 0.5;

	auto [vol_sphere_m3, m_sphere_kg, J_sphere_kgm2, Aref_m2] = CalcSphereProps(r_sphere_m, rho_bowlingball_kgpm3);

	double Vterm_mps = std::sqrt((2 * m_sphere_kg * 9.81) / (1.2 * CD_approx * Aref_m2));

	std::unordered_map<std::string, double> amod = {

		{"m_kg", m_sphere_kg},
		{"Jxz_b_kgm2", 0.0},
		{"Jxx_b_kgm2", J_sphere_kgm2},
		{"Jyy_b_kgm2", J_sphere_kgm2},
		{"Jzz_b_kgm2", J_sphere_kgm2},
		{"r_sphere_m", r_sphere_m},
		{"m_sphere_kg", m_sphere_kg},
		{"CD_approx", CD_approx},
		{"Aref_m2", Aref_m2},
		{"Vterm_mps", Vterm_mps} };

	return amod;


}


std::unordered_map<std::string, double> TsarCannonball()
{
	double in2m = 0.0254;

	std::string vehicle_name = "Tsar Cannonball";

	std::string short_name = "Tsar";

	double rho_castiron_kgpm3 = 7000.0;

	double r_sphere_in = 35.0;
	double r_sphere_m = r_sphere_in * in2m;

	//reference wing span
	double b_m = r_sphere_m;

	//reference wing chord
	double c_m = r_sphere_m;

	double CD_approx = 0.5;

	// Roll damping from roll rate
	double Clp = 0.0;

	// Roll Damping from yaw rate
	double Clr = 0.0;

	//Pitch damping from pitch rate
	double Cmq = 0.0;

	// Yaw damping from roll rate
	double Cnp = 0.0;

	//Yaw damping from yaw rate
	double Cnr = 0.0;

	auto [vol_sphere_m3, m_sphere_kg, J_sphere_kgm2, Aref_m2] = CalcSphereProps(r_sphere_m, rho_castiron_kgpm3);

	double Vterm_mps = std::sqrt((2 * m_sphere_kg * 9.81) / (1.2 * CD_approx * Aref_m2));


	std::unordered_map<std::string, double> amod = {

		{"m_kg", m_sphere_kg},
		{"Jxz_b_kgm2", 0.0},
		{"Jxx_b_kgm2", J_sphere_kgm2},
		{"Jyy_b_kgm2", J_sphere_kgm2},
		{"Jzz_b_kgm2", J_sphere_kgm2},
		{"r_sphere_m", r_sphere_m},
		{"m_sphere_kg", m_sphere_kg},
		{"CD_approx", CD_approx},
		{"Clp", Clp},
		{"Clr", Clr},
		{"Cmq", Cmq},
		{"Cnp", Cnp},
		{"Cnr", Cnr},
		{"Aref_m2", Aref_m2},
		{"Vterm_mps", Vterm_mps},
		{"b_m", b_m},
		{"c_m", c_m} };

	return amod;

}

std::unordered_map<std::string, double> NASA_Atmos01_Sphere()
{
	double in2m = 0.0254;

	std::string vehicle_name = "NASA Atmos01 1-Slug Cannonball";

	std::string short_name = "Atmos01";

	double rho_castiron_kgpm3 = 7868.36;

	double r_sphere_in = 3.0;
	double r_sphere_m = r_sphere_in * in2m;

	//reference wing span
	double b_m = r_sphere_m;

	//reference wing chord
	double c_m = r_sphere_m;

	double CD_approx = 0.5;

	// Roll damping from roll rate
	double Clp = 0.0;

	// Roll Damping from yaw rate
	double Clr = 0.0;

	//Pitch damping from pitch rate
	double Cmq = 0.0;

	// Yaw damping from roll rate
	double Cnp = 0.0;

	//Yaw damping from yaw rate
	double Cnr = 0.0;

	auto [vol_sphere_m3, m_sphere_kg, J_sphere_kgm2, Aref_m2] = CalcSphereProps(r_sphere_m, rho_castiron_kgpm3);

	double Vterm_mps = std::sqrt((2 * m_sphere_kg * 9.81) / (1.2 * CD_approx * Aref_m2));


	std::unordered_map<std::string, double> amod = {

		{"m_kg", m_sphere_kg},
		{"Jxz_b_kgm2", 0.0},
		{"Jxx_b_kgm2", J_sphere_kgm2},
		{"Jyy_b_kgm2", J_sphere_kgm2},
		{"Jzz_b_kgm2", J_sphere_kgm2},
		{"r_sphere_m", r_sphere_m},
		{"m_sphere_kg", m_sphere_kg},
		{"CD_approx", CD_approx},
		{"Clp", Clp},
		{"Clr", Clr},
		{"Cmq", Cmq},
		{"Cnp", Cnp},
		{"Cnr", Cnr},
		{"Aref_m2", Aref_m2},
		{"Vterm_mps", Vterm_mps},
		{"b_m", b_m},
		{"c_m", c_m} };

	return amod;








}

std::unordered_map<std::string, double> NASA_Atmos02_Brick()
{
	double in2m = 0.0254;

	std::string vehicle_name = "Tumbling Brick (No Damping or Drag)";

	std::string short_name = "Atmos02";

	double slug2kg = 14.5959;
	double kg2slug = 1 / slug2kg;

	double ft2m = 0.304878;


	double m_brick_slug = 0.1554048;
	double m_brick_kg = slug2kg * m_brick_slug;  // Correct: 0.155 slugs × 14.6 = 2.27 kg

	// Moments and products of inertia
	double Jxx_slugft2 = 0.00189422;
	double Jxx_kgm2 = slug2kg * std::pow(ft2m, 2) * Jxx_slugft2;

	double Jyy_slugft2 = 0.00621102;
	double Jyy_kgm2 = slug2kg * std::pow(ft2m, 2) * Jyy_slugft2;

	double Jzz_slugft2 = 0.00719467;
	double Jzz_kgm2 = slug2kg * std::pow(ft2m, 2) * Jzz_slugft2;

	double Jxz_slugft2 = 0;
	double Jxz_kgm2 = slug2kg * std::pow(ft2m, 2) * Jxz_slugft2;
	
	// Body position of center of mass with respect to the geometric center of the brick
	double dxcg_ft = 0;
	double dycg_ft = 0;
	double dzcg_ft = 0;


	//Dimensions of brick
	double length_brick_m = 8 * in2m;
	double width_brick_m = 4 * in2m;


	//reference area
	double A_ref_m2 = length_brick_m * width_brick_m;

	// reference wing span
	double b_m = 0.33333 * ft2m;

	//reference wing chord
	double c_m = .66666667 * ft2m;


	double CD_approx = 0.0;

	// Roll damping from roll rate
	double Clp = 0.0;

	// Roll Damping from yaw rate
	double Clr = 0.0;

	//Pitch damping from pitch rate
	double Cmq = 0.0;

	// Yaw damping from roll rate
	double Cnp = 0.0;

	//Yaw damping from yaw rate
	double Cnr = 0.0;


	std::unordered_map<std::string, double> amod = {

		{"m_kg", m_brick_kg},
		{"Jxz_b_kgm2", Jxz_kgm2},
		{"Jxx_b_kgm2", Jxx_kgm2},
		{"Jyy_b_kgm2", Jyy_kgm2},
		{"Jzz_b_kgm2", Jzz_kgm2},
		{"m_kg", m_brick_kg},
		{"CD_approx", CD_approx},
		{"Clp", Clp},
		{"Clr", Clr},
		{"Cmq", Cmq},
		{"Cnp", Cnp},
		{"Cnr", Cnr},
		{"Aref_m2", A_ref_m2},
		{"b_m", b_m},
		{"c_m", c_m} };

	return amod;

}

std::unordered_map<std::string, double> NASA_Atmos03_Brick()
{
	double in2m = 0.0254;

	std::string vehicle_name = "Tumbling Brick Wirh Aerodynamic Damping";

	std::string short_name = "Atmos03";

	double slug2kg = 14.5959;
	double kg2slug = 1 / slug2kg;

	double ft2m = 0.304878;


	double m_brick_slug = 0.1554048;
	double m_brick_kg = slug2kg * m_brick_slug;  // Correct: 0.155 slugs × 14.6 = 2.27 kg
	// Moments and products of inertia
	double Jxx_slugft2 = 0.00189422;
	double Jxx_kgm2 = slug2kg * std::pow(ft2m, 2) * Jxx_slugft2;

	double Jyy_slugft2 = 0.00621102;
	double Jyy_kgm2 = slug2kg * std::pow(ft2m, 2) * Jyy_slugft2;

	double Jzz_slugft2 = 0.00719467;
	double Jzz_kgm2 = slug2kg * std::pow(ft2m, 2) * Jzz_slugft2;

	double Jxz_slugft2 = 0;
	double Jxz_kgm2 = slug2kg * std::pow(ft2m, 2) * Jxz_slugft2;

	// Body position of center of mass with respect to the geometric center of the brick
	double dxcg_ft = 0;
	double dycg_ft = 0;
	double dzcg_ft = 0;


	//Dimensions of brick
	double length_brick_m = 8 * in2m;
	double width_brick_m = 4 * in2m;


	//reference area
	double A_ref_m2 = length_brick_m * width_brick_m;

	// reference wing span
	double b_m = 0.33333 * ft2m;

	//reference wing chord
	double c_m = .66666667 * ft2m;


	double CD_approx = 0.0;

	// Roll damping from roll rate
	double Clp = -1.0;

	// Roll Damping from yaw rate
	double Clr = 0.0;

	//Pitch damping from pitch rate
	double Cmq = -1.0;

	// Yaw damping from roll rate
	double Cnp = 0.0;

	//Yaw damping from yaw rate
	double Cnr = -1.0;


	std::unordered_map<std::string, double> amod = {

		{"m_kg", m_brick_kg},
		{"Jxz_b_kgm2", Jxz_kgm2},
		{"Jxx_b_kgm2", Jxx_kgm2},
		{"Jyy_b_kgm2", Jyy_kgm2},
		{"Jzz_b_kgm2", Jzz_kgm2},
		{"m_kg", m_brick_kg},
		{"CD_approx", CD_approx},
		{"Clp", Clp},
		{"Clr", Clr},
		{"Cmq", Cmq},
		{"Cnp", Cnp},
		{"Cnr", Cnr},
		{"Aref_m2", A_ref_m2},
		{"b_m", b_m},
		{"c_m", c_m} };

	return amod;

}

// Roll, Pitch, Yaw moment coefficent for dampended tumbling brick simulation
double Cl_brick(double Clp, double Clr, double p_b_rps, double r_b_rps, double b_m, double true_airspeed_mps)
{
    if (true_airspeed_mps < 1e-6) return 0.0;  // Avoid division by zero
    double Cl = Clp * p_b_rps * b_m / (2 * true_airspeed_mps) + Clr * r_b_rps * b_m / (2 * true_airspeed_mps);
    return Cl;
}

double Cm_brick(double Cmq, double q_b_rps, double c_m, double true_airspeed_mps)
{
	if (true_airspeed_mps < 1e-6) return 0.0;  
	double Cm = Cmq * q_b_rps * c_m / (2 * true_airspeed_mps);

	return Cm;
}

double Cn_brick(double Cnp, double Cnr, double p_b_rps, double r_b_rps, double b_m, double true_airspeed_mps)
{
	if (true_airspeed_mps < 1e-6) return 0.0;  
	double Cn = Cnp * p_b_rps * b_m / (2 * true_airspeed_mps) + Cnr * r_b_rps * b_m / (2 * true_airspeed_mps);

	return Cn;

}

double sphere_drag(double mach)
{

	double cd = 0;

	if (mach <= 0.722)
	{
		cd = 0.45 * std::pow(mach, 2) + 0.424;
 	}
	else
	{
		cd = 2.1 * std::exp(-1.16 * (mach + 0.35)) - 8.9 * std::exp(-2.2 * (mach + 0.35)) + 0.92;
	}

	return cd;
}