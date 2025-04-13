#include "numerical_integration_methods.h"
#include <vector>
#include <functional>
#include <string>

std::pair<std::vector<double>, std::vector<std::vector<double>>> forward_euler(std::function<std::vector<double>(double, const std::vector<double>, const std::unordered_map<std::string, double>&, const std::unordered_map<std::string, double>&)> f, const std::vector<double>& t_s, std::vector<std::vector<double>> sx, double h_s, const std::unordered_map<std::string, double>& amod, const std::unordered_map<std::string, double> airmod)
{

	/* This Function performs forward Euler integration to approximate the solution of a differntial equation
	


		Arguments 
		f: A function representing the right-hand sode of the differential equation (dx/dt = f(t,x))
		t_s : A vector of points in time at which numerical solutions will be approximated
		sx: the numerically approximated solution data to the DE, f
		h_s: the step size in seconds 
		amod: Vehicle model data
		airmod: Atmosphere data

		Return
		t_s: a vector of points in time at which numerical solutions was approximated
		sx: the numerically approximated solution to the DE, f
	
	
	*/

	// Forward Euler numerical integration
	for (std::size_t i = 1; i < t_s.size(); ++i)
	{
		std::vector <double> column;
		for (const auto& row : sx)
		{
			column.push_back(row[i-1]);
		}
		std::vector<double> dx = f(t_s[i - 1], column, amod, airmod);

		for (std::size_t j = 0; j < dx.size(); ++j)

		{
			sx[j][i] = sx[j][i - 1] + h_s * dx[j];
		}
		// THIS may need to be changed to a 2d array
	}


	return { t_s, sx };

}


std::pair<std::vector<double>, std::vector<std::vector<double>>> AB2(std::function<std::vector<double>(double, const std::vector<double>, const std::unordered_map<std::string, double>&, const std::unordered_map<std::string, double>&)> f, const std::vector<double>& t_s, std::vector<std::vector<double>> sx, double h_s, const std::unordered_map<std::string, double>& amod, const std::unordered_map<std::string, double> airmod)
{
	// Performs the 2nd order Adams-Bashforth method to approximate the solution of a differential equation.


	//Forward Euler method for first step(i=0)
	std::vector <double> column;
	for (const auto& row : sx)
	{
		column.push_back(row[0]);
	}

	std::vector<double> dx = f(t_s[0], column, amod, airmod);

	for (std::size_t j = 0; j < dx.size(); ++j)
	{
		sx[j][1] = sx[j][0] + h_s * dx[j];
	}

	//Adams-Bashforth method for i>=1
	for (std::size_t i = 2; i < t_s.size(); ++i)
	{
		std::vector<double> fim1, fim2;

		std::vector<double> column_fim1, column_fim2;
		for (const auto& row : sx)
		{
			column_fim1.push_back(row[i - 1]);
			column_fim2.push_back(row[i - 2]);
		}

		fim1 = f(t_s[i - 1], column_fim1, amod, airmod);
		fim2 = f(t_s[i - 2], column_fim2, amod, airmod);

		for (std::size_t j = 0; j < sx.size(); ++j)
		{
			sx[j][i] = sx[j][i - 1] + 1.5 * h_s * fim1[j] - 0.5 * h_s * fim2[j];
		}
	}

	return { t_s, sx };
}


std::pair<std::vector<double>, std::vector<std::vector<double>>> RK4(std::function<std::vector<double>(double, const std::vector<double>, const std::unordered_map<std::string, double>&, const std::unordered_map<std::string, double>&)> f, const std::vector<double>& t_s, std::vector<std::vector<double>> sx, double h_s, const std::unordered_map<std::string, double>& amod, const std::unordered_map<std::string, double> airmod)
{

	// Performs 4th order Runge-Kutta method to approximate solution of a differential equation


	for (std::size_t i = 1; i < t_s.size(); ++i)
	{
		std::vector <double> column;
		for (const auto& row : sx)
		{
			column.push_back(row[i - 1]);
		}

		std::vector <double> k1 = f(t_s[i - 1], column, amod, airmod);

		std::vector<double> temp_k2(column.size());
		for (std::size_t j = 0; j < column.size(); ++j)
		{
			temp_k2[j] = column[j] + 0.5 * h_s * k1[j];
		}

		std::vector <double> k2 = f(t_s[i - 1] + 0.5 * h_s, temp_k2, amod, airmod);


		std::vector<double> temp_k3(column.size());
		for (std::size_t j = 0; j < column.size(); ++j)
		{
			temp_k3[j] = column[j] + 0.5 * h_s * k2[j];
		}
		std::vector<double> k3 = f(t_s[i - 1] + 0.5 * h_s, temp_k3, amod, airmod);

		std::vector<double> temp_k4(column.size());
		for (std::size_t j = 0; j < column.size(); ++j)
		{
			temp_k4[j] = column[j] + h_s * k3[j];
		}

		std::vector<double> k4 = f(t_s[i - 1] + h_s, temp_k4, amod, airmod);

		// I suspect a shape issue may arise from this
		for (std::size_t j = 0; j < sx.size(); ++j)
		{
			sx[j][i] = sx[j][i - 1] + (1.0 / 6.0) * h_s * (k1[j] + 2.0 * k2[j] + 2.0 * k3[j] + k4[j]);
		}



		}

	return { t_s, sx };

}
