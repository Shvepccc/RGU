#include "elevator.h"
#include "passenger.h"
#include <iostream>
#include"../../common_libs/helpers.h"

elevator::elevator(int ID, int n, int weight_limit)
{
    _ID = ID;
    _state = el_state();
    _buttons_arr = std::vector<el_button>(n);
    _weight_limit = weight_limit;

    std::tm a{};
    _total_time = a;
    _floors_count = 0;
    _total_weight = 0;
    _max_weight = 0;
    _exceed_limit_count = 0;

    _remainder_sec = 0;
    _floors_queue = std::set<int>{};
    _current_weight = 0;
    _sec_per_floor = 0;
    curr_passengers = std::vector<passenger>{};
}

elevator::el_state elevator::get_state()
{
    return _state;
}

void elevator::set_state(elevator_states state)
{
    _state._state = state;
}

//int get_limit();
//
//void close_doors();
//void open_doors();
//
bool elevator::set_task(int start_floor, int target_floor)
{
    if (_weight_limit * 0.9 < _current_weight)
        return false;

    _floors_queue.insert(start_floor);
    _floors_queue.insert(target_floor);
    
    return true;
}

void elevator::action(std::tm& current_time,
    std::vector<std::vector<passenger>>& building_arr)
{
    // check if we have some tasks
    if (_floors_queue.empty())
    {
        set_state(doors_closed);
        return;
    }

    // set state
    elevator_states curr_el_state = 
        (_state._curr_floor - *_floors_queue.begin()) < 0 
        ? upward_move 
        : downward_move;
    set_state(curr_el_state);

    // go through one minute with ticks, which size = _sec_per_floor
    int current_remainder = 60 + _remainder_sec;
    int start_size = current_remainder;
    while (current_remainder > _sec_per_floor && !_floors_queue.empty())
    {
        // move elevator to one floor
        std::cout << "move from: " << _state._curr_floor;
        calculate_speed();
        _state._curr_floor += (_state._state == upward_move) ? 1 : -1;
        std::cout << " to: " << _state._curr_floor << " ...\n";

        // check if ewe need to stay there or not
        if (_state._state == upward_move
            && *_floors_queue.begin() <= _state._curr_floor)
        {
            // I'm on one of the target floors
            // passengers should be loaded and unloaded
            std::cout << "loading and unloading...\n";

            _floors_queue.erase(_floors_queue.begin());
            _buttons_arr[_state._curr_floor - 1].set_state(0);

            set_state(doors_open);

            //unloading
            for (auto it = curr_passengers.begin();
                it != curr_passengers.end(); )
            {
                if (it->_target_floor == _state._curr_floor)
                {
                    //it->total_time = time_sub(current_time, it->load_time);
                    //TODO: add time control
                    //TODO: change main passengers vector to hesh map
                    it = building_arr[_state._curr_floor].erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // loading
            for (auto it = building_arr[_state._curr_floor].begin();
                it != building_arr[_state._curr_floor].end(); )
            {
                if (it->_el_id == _ID && (_current_weight + it->_weight) <= _weight_limit)
                {
                    _current_weight += it->_weight;
                    it->load_time = current_time;
                    curr_passengers.push_back(*it);
                    _buttons_arr[it->_target_floor].set_state(1);
                    it = building_arr[_state._curr_floor].erase(it);
                }
                else
                {
                    ++it;
                }
            }

            set_state(doors_closed);
            set_state(curr_el_state);
        }
        else if (_state._state == downward_move
            && *_floors_queue.begin() >= _state._curr_floor)
        {
            std::cout << "loading and unloading...\n";
            _floors_queue.erase(_floors_queue.begin());
        }

        // de increase seconds remainder in size of one minute
        current_remainder -= _sec_per_floor;
    }
    if (_floors_queue.empty())
    {
        // if we dont have any tasks - set state and wait
        _remainder_sec = 0;
        set_state(doors_closed);
    }
    else
    {
        // otherwise set new remainder
        _remainder_sec = current_remainder;
    }
}

void elevator::move_one_floor()
{
    std::cout << "move...\n";
    calculate_speed();

    _state._curr_floor += 1;
}

void elevator::calculate_speed()
{
    _sec_per_floor = 3 + (int)floor(5 * (_current_weight / _weight_limit));
}

int elevator::get_id()
{
    return _ID;
}