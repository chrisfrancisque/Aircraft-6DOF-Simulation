#pragma once
#ifndef FLAT_EARTH_EOM_H
#define FLAT_EARTH_EOM_H

#include <vector>
#include <unordered_map>
#include <string>


std::vector<double> flat_earth_eom(
    double t,
    const std::vector<double> x,
    const std::unordered_map<std::string, double>& amod, std::unordered_map<std::string, double> airmod
);

#endif // FLAT_EARTH_EOM_H
