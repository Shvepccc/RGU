#ifndef SIMULATE_ELEVATORS_H
#define SIMULATE_ELEVATORS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <stdexcept>

#include"../../common_libs/helpers.h"

#pragma region Data types

struct passenger
{
    int _ID;
    std::tm _appear_time;
    int _start_floor;
    int _target_floor;
    double _weight;

    std::tm load_time;
    std::tm tootal_time;
    std::vector<passenger> meet_arr;
    int exceed_limit;

    passenger(int ID, std::tm appear_time, int start_floor, int target_floor, double weight) :
        _ID(ID), _appear_time(appear_time), _start_floor(start_floor),
        _target_floor(target_floor), _weight(weight)
    {
    }

};

class el_button
{
private:
    int _state;

public:
    el_button(int state) :
        _state(state)
    {
    }

    void change_state()
    {
        _state = _state * -1;
    }

    void set_state(int state)
    {
        _state = state;
    }

    int get_state()
    {
        return _state;
    }
};

class elevator
{
    enum elevator_states
    {
        doors_open,
        doors_closed,
        upward_move,
        downward_move
    };

private:
    int _state;
    std::vector<el_button> buttons_arr;
    int weight_limit;

    std::tm total_time;
    int floors_count;
    double total_weight;
    double max_weight;
    int exceed_limit_count;
    
public:
    elevator();
    ~elevator();

    int get_state();
    int get_limit();

    void close_doors();
    void open_doors();
    
    void set_task(int target_floor);

private:

};

#pragma endregion


#pragma region  Parse data

std::tm parse_time(const std::string& time_str)
{
    std::tm tm = {};
    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, "%H:%M");

    if (ss.fail())
    {
        throw std::runtime_error("Failed to parse time: " + time_str);
    }
    return tm;
}

std::vector<passenger> read_passengers_from_file(const std::string& file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + file_path);
    }

    std::vector<passenger> passengers;
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        int ID, start_floor, target_floor;
        std::string time_str;
        double weight;

        if (!(iss >> ID >> weight >> start_floor  >> time_str >> target_floor))
        {
            throw std::runtime_error("Invalid line format: " + line);
        }

        passengers.emplace_back(ID, parse_time(time_str), start_floor, target_floor, weight);
    }

    return passengers;
}

void read_settings_from_file(int& n, int& k, std::vector<int>& limits, const std::string& file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + file_path);
    }

    std::string line;
    if (!std::getline(file, line))
    {
        throw std::runtime_error("File is empty: " + file_path);
    }

    std::istringstream iss(line);

    if (!(iss >> n >> k))
    {
        throw std::runtime_error("Failed to read n and k from file: " + file_path);
    }

    int num;
    limits.clear();
    while (iss >> num)
    {
        limits.push_back(num);
    }

    if (limits.empty())
    {
        throw std::runtime_error("No limits provided in file: " + file_path);
    }
}

void print_parse_data(std::vector<passenger> const& passengers,
    std::vector<int> const& limits,
    int n, int k)
{
    std::cout << "Count of floors: " << n << "\n";
    std::cout << "Count of elevators: " << k << "\n";

    try
    {
        for (const auto& p : passengers)
        {
            std::cout << "ID: " << p._ID
                << " Time: "
                << std::setw(2) << std::setfill('0') << p._appear_time.tm_hour << ":"
                << std::setw(2) << std::setfill('0') << p._appear_time.tm_min
                << ", From: " << p._start_floor
                << ", To: " << p._target_floor
                << ", Weight: " << p._weight << std::endl;
        }

        std::cout << "Limits: ";
        for (const auto& l : limits)
        {
            std::cout << l << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

#pragma endregion






#endif // !SIMULATE_ELEVATORS_H
