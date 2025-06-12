#ifndef PASSENGER_H
#define PASSENGER_H

#include <vector>
#include <ctime>
#include <iostream>
#include <iomanip>

class passenger
{
public:
    int _ID;
    std::tm _appear_time;
    int _start_floor;
    int _target_floor;
    double _weight;

    std::tm load_time;
    std::tm total_time;
    std::vector<passenger> meet_arr;
    int exceed_limit;

    int _el_id;

    passenger(int ID, std::tm appear_time, int start_floor, int target_floor, double weight) :
        _ID(ID), _appear_time(appear_time), _start_floor(start_floor),
        _target_floor(target_floor), _weight(weight)
    {}

    friend std::ostream& operator<<(std::ostream& os, const passenger& p);

};

inline std::ostream& operator<<(std::ostream& os, const passenger& p)
{
    os << "ID: " << p._ID
        << " Time: "
        << std::setw(2) << std::setfill('0') << p._appear_time.tm_hour << ":"
        << std::setw(2) << std::setfill('0') << p._appear_time.tm_min
        << ", From: " << p._start_floor
        << ", To: " << p._target_floor
        << ", Weight: " << p._weight;
    return os;
}

#endif // !PASSENGER_H