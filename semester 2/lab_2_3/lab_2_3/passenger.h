#ifndef PASSENGER_H
#define PASSENGER_H

#include <vector>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <set>

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
    std::vector<int> meet_arr;
    int exceed_limit;

    int _el_id;
    int _order_id;

    passenger(int ID, std::tm appear_time, int start_floor, int target_floor, double weight) :
        _ID(ID), _appear_time(appear_time), _start_floor(start_floor),
        _target_floor(target_floor), _weight(weight),
        load_time(std::tm{}), total_time(std::tm{}), meet_arr(std::vector<int>{}), exceed_limit(0),
        _el_id(0), _order_id(0)
    {}

    friend std::ostream& operator<<(std::ostream& os, const passenger& p);

};

inline std::ostream& operator<<(std::ostream& os, const passenger& p)
{
    auto print_time_inner = [&os](const std::tm& time, const std::string& label) {
        char buf[80];
        strftime(buf, sizeof(buf), "%H:%M:%S", &time);
        os << label << buf;
        };

    os << "Passenger ID: " << p._ID << "\n"
        << "Appear time: ";
    print_time_inner(p._appear_time, "");
    os << " Start floor: " << p._start_floor << " "
        << "Target floor: " << p._target_floor << " "
        << "Weight: " << p._weight << " kg "
        << "Load time: ";
    print_time_inner(p.load_time, "");
    os << " Total time: ";
    print_time_inner(p.total_time, "");
    os << "\nExceed limit: " << (p.exceed_limit ? "Yes" : "No");

    os << "\nMet passengers: [";
    for (size_t i = 0; i < p.meet_arr.size(); ++i) {
        os << p.meet_arr[i];
        if (i != p.meet_arr.size() - 1) os << ", ";
    }
    os << "]\n";

    return os;
}

#endif // !PASSENGER_H