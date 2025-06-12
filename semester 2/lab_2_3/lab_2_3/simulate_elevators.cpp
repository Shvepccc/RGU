#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

#include "passenger.h"
#include "elevator.h"
#include"../../common_libs/helpers.h"

using el_s = elevator::elevator_states;


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

        if (!(iss >> ID >> weight >> start_floor >> time_str >> target_floor))
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
        std::cout << std::endl << std::endl;;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

#pragma endregion


bool compare_by_appear_time(const passenger& a, const passenger& b)
{
    std::tm* a_t = const_cast<std::tm*>(&a._appear_time);
    std::tm* b_t = const_cast<std::tm*>(&b._appear_time);

    return (a_t->tm_hour * 60 + a_t->tm_min) <
        (b_t->tm_hour * 60 + b_t->tm_min);
}

void add_minutes(std::tm& t, int minutes)
{
    int remainder = (minutes + t.tm_min) / 60;
    t.tm_hour += remainder;
    t.tm_min += (minutes)-(60 * remainder);
}

void print_time(std::tm& t)
{
    std::cout 
        << std::setw(2) << std::setfill('0') << t.tm_hour << ":"
        << std::setw(2) << std::setfill('0') << t.tm_min
        << "\n";
}


void controller(const char* ifile_path_1, const char* ifile_path_2,
    const char* ofile_path_1, const char* ofile_path_2)
{
    //parse data
    int n, k;
    std::vector<int> limits;
    read_settings_from_file(n, k, limits, ifile_path_2);
    std::vector<passenger> passenger_arr = read_passengers_from_file(ifile_path_1);
    std::sort(passenger_arr.begin(), passenger_arr.end(), compare_by_appear_time);

    //print parsed data (optionally)
    print_parse_data(passenger_arr, limits, n, k);

    //create elevators set
    std::vector<elevator> elevator_arr;
    for (int i = 0; i < k; ++i)
    {
        elevator_arr.push_back(elevator(i + 1, n, limits[i]));
    }

    //getting started with tasks
    std::tm cur_time{};
    int pas_index = 0;
    int total_pas_count = passenger_arr.size();

    std::vector<std::vector<passenger>> building_arr = std::vector<std::vector<passenger>>(n);

    while (pas_index < total_pas_count)
    {
        //std::cout << "Current time: ";
        //print_time(cur_time);

        if (cmp_time(passenger_arr[pas_index]._appear_time, cur_time))
        {
            auto pas = passenger_arr[pas_index];
            std::cout << "---> " << pas << "\n";

            //traverce all elevators to find a suitable one and give it a task
            bool set_task_flag = true;
            for (auto& el : elevator_arr)
            {
                elevator::el_state el_state_cur = el.get_state();

                el_s pas_direction = (pas._start_floor - pas._target_floor) < 0
                    ? el_s::upward_move
                    : el_s::downward_move;

                if (el_state_cur._state == el_s::upward_move
                    && pas_direction == el_s::upward_move
                    && pas._start_floor >= el_state_cur._curr_floor)
                {
                    //set this task to current elevator
                    set_task_flag = el.set_task(pas._start_floor, pas._target_floor);
                }
                else if (el_state_cur._state == el_s::downward_move
                    && pas_direction == el_s::downward_move
                    && pas._start_floor <= el_state_cur._curr_floor)
                {
                    //set this task to current elevator
                    set_task_flag = el.set_task(pas._start_floor, pas._target_floor);
                }
                else if (el_state_cur._state == el_s::doors_closed ||
                    el_state_cur._state == el_s::doors_open)
                {
                    //set this task to current elevator
                    set_task_flag = el.set_task(pas._start_floor, pas._target_floor);
                }

                if (set_task_flag)
                {
                    passenger_arr[pas_index]._el_id = el.get_id();
                    building_arr[pas._start_floor - 1].push_back(pas);
                    break;
                }
            }

            if (!set_task_flag)
            {
                //set this task to elevator which one will be released the fastest
                //TODO: i will make it later)))

                //std::cout << "\nELEVATOR NOT FOUND!!!\n" << std::endl;
            }
        }

        for (auto& el : elevator_arr)
        {
            el.action(cur_time, building_arr);
        }

        add_minutes(cur_time, 1);
    }
}