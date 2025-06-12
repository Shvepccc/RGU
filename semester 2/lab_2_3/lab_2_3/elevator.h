#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>
#include <queue>
#include <set>
#include "passenger.h"

class el_button
{
private:
    int _state;

public:
    el_button() : _state(0)
    {}

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
public:
    struct el_state
    {
        int _curr_floor;
        int _state;

        el_state() : _curr_floor(1), _state(doors_closed)
        {}
    };

    enum elevator_states
    {
        doors_open,
        doors_closed,
        upward_move,
        downward_move
    };

private:
    int _ID;
    el_state _state;
    std::vector<el_button> _buttons_arr;
    int _weight_limit;

    std::tm _total_time;
    int _floors_count;
    double _total_weight;
    double _max_weight;
    int _exceed_limit_count;

    std::set<int> _floors_queue;
    int _remainder_sec;
    int _current_weight;
    int _sec_per_floor;
    std::vector<passenger> curr_passengers;

public:
    //elevator();
    elevator(int ID, int n, int weight_limit);
    el_state get_state();
    int get_id();
    int get_limit();
    bool set_task(int start_floor, int target_floor);
    void action(std::tm& current_time,
        std::vector<std::vector<passenger>>& building_arr);

private:
    void set_state(elevator_states state);
    void close_doors();
    void open_doors();
    void move_one_floor();
    void calculate_speed();
};

#endif