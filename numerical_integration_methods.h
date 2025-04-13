#pragma once
#ifndef NUMERICAL_INTEGRATION_METHODS_H
#define NUMERICAL_INTEGRATION_METHODS_H

#include <vector>
#include <functional>
#include <utility>
#include <string>
#include <unordered_map>
#include <algorithm>


std::pair<std::vector<double>, std::vector<std::vector<double>>> forward_euler(std::function<std::vector<double>(double, const std::vector<double>, const std::unordered_map<std::string, double>&, const std::unordered_map<std::string, double>&)> f, const std::vector<double>& t_s, std::vector<std::vector<double>> sx, double h_s, const std::unordered_map<std::string, double>& amod, const std::unordered_map<std::string, double> airmod);


std::pair<std::vector<double>, std::vector<std::vector<double>>> AB2(std::function<std::vector<double>(double, const std::vector<double>, const std::unordered_map<std::string, double>&, const std::unordered_map<std::string, double>&)> f, const std::vector<double>& t_s, std::vector<std::vector<double>> sx, double h_s, const std::unordered_map<std::string, double>& amod, const std::unordered_map<std::string, double> airmod);


std::pair<std::vector<double>, std::vector<std::vector<double>>> RK4(std::function<std::vector<double>(double, const std::vector<double>, const std::unordered_map<std::string, double>&, const std::unordered_map<std::string, double>&)> f, const std::vector<double>& t_s, std::vector<std::vector<double>> sx, double h_s, const std::unordered_map<std::string, double>& amod, const std::unordered_map<std::string, double> airmod);



#endif // NUMERICAL_INTEGRATION_METHODS_H
