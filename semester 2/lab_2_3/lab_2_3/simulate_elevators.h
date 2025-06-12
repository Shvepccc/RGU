#ifndef SIMULATE_ELEVATORS_H
#define SIMULATE_ELEVATORS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <stdexcept>

#include "passenger.h"
#include"../../common_libs/helpers.h"


#pragma region  Parse data

std::tm parse_time(const std::string& time_str);

std::vector<passenger> read_passengers_from_file(const std::string& file_path);

void read_settings_from_file(int& n, int& k, std::vector<int>& limits, const std::string& file_path);

void print_parse_data(std::vector<passenger> const& passengers,
    std::vector<int> const& limits,
    int n, int k);

#pragma endregion


bool compare_by_appear_time(const passenger& a, const passenger& b);

void add_minutes(std::tm& t, int minutes);
bool c_time(std::tm& a, std::tm& b);
void print_time(std::tm& t);

void controller(const char* ifile_path_1, const char* ifile_path_2,
    const char* ofile_path_1, const char* ofile_path_2);


#endif // !SIMULATE_ELEVATORS_H
