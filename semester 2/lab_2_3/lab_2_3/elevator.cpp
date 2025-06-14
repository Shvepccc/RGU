#include <iostream>
#include <algorithm>
#include "elevator.h"
#include "passenger.h"
#include"../../common_libs/helpers.h"

elevator::elevator()
{}

elevator::elevator(int ID, int n, int weight_limit)
{
    _ID = ID;
    _state = el_state();
    _buttons_arr = std::vector<el_button>(n);
    _weight_limit = weight_limit;

    std::tm a{};
    _total_work_time = a;
    _total_waste_time = a;
    _floors_count = 0;
    _total_weight = 0;
    _max_weight = 0;
    _exceed_limit_count = 0;

    _remainder_sec = 0;
    _floors_queue = std::vector<int>{};
    _current_weight = 0;
    _sec_per_floor = 0;
    curr_passengers = std::vector<passenger>{};
}

elevator::el_state elevator::get_state() const
{
    return _state;
}

void elevator::set_state(elevator_states state)
{
    _state._state = state;
}

int elevator::get_floors_queue_size() const
{
    return _floors_queue.size();
}

bool elevator::set_task(int start_floor, int target_floor)
{
    if (_weight_limit * 0.9 < _current_weight)
        return false;

    _floors_queue.push_back(start_floor);
    _floors_queue.push_back(target_floor);
    
    return true;
}

void elevator::action(std::tm& current_time,
    std::vector<std::vector<passenger>>& building_arr,
    std::vector<passenger>& result_passengers_arr)
{
    // check if we have some tasks
    if (_floors_queue.empty())
    {
        set_state(doors_closed);
        return;
    }

    // go through one minute with ticks, which size = _sec_per_floor
    int current_remainder = 60 + _remainder_sec;
    int start_size = current_remainder;
    while (current_remainder > _sec_per_floor && !_floors_queue.empty())
    {
        // set state
        elevator_states curr_el_state =
            (_state._curr_floor < *_floors_queue.begin())
            ? upward_move
            : downward_move;
        set_state(curr_el_state);

        // if elevator not on the target needed floor, let's move elevator to one floor
        calculate_speed();
        if ((_state._curr_floor != *_floors_queue.begin()) || (start_size != current_remainder))
        {
            std::cout << "move from: " << _state._curr_floor;
            _state._curr_floor += (_state._state == upward_move) ? 1 : -1;
            std::cout << " to: " << _state._curr_floor << " ...\n";
            _floors_count += 1;
            add_time(_total_work_time, 0, _sec_per_floor);
        }
        else if (*_floors_queue.begin() < *std::next(_floors_queue.begin())) //(curr_el_state == upward_move)
        {
            std::sort(_floors_queue.begin(), _floors_queue.end());
        }
        else if (*_floors_queue.begin() > *std::next(_floors_queue.begin()))
        {
            std::sort(_floors_queue.begin(), _floors_queue.end(), std::greater<int>());
        }

        // check if we need to stay there or not
        if ((_state._state == upward_move
            && *_floors_queue.begin() <= _state._curr_floor)
            || (_state._state == downward_move
            && *_floors_queue.begin() >= _state._curr_floor))
        {
            // I'm on one of the target floors. Passengers should be loaded and unloaded
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
                    _current_weight -= it->_weight;
                    std::tm temp_total_time = time_sub(current_time, it->load_time);
                    it->total_time = temp_total_time;

                    result_passengers_arr.push_back(*it);

                    // remove from elevator passengers arr
                    it = curr_passengers.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // loading
            std::vector<int> newcomers_id;
            std::vector<int> el_pas_id;

            // collect current elevator passengers id
            for (auto& el_pas : curr_passengers)
            {
                el_pas_id.push_back(el_pas._ID);
            }

            for (auto it = building_arr[_state._curr_floor - 1].begin();
                it != building_arr[_state._curr_floor - 1].end(); )
            {
                if (it->_el_id == _ID)
                {
                    // try load passenger
                    if ((_current_weight + it->_weight) <= _weight_limit)
                    {
                        _current_weight += it->_weight;
                        _total_weight += it->_weight;
                        _max_weight = (_current_weight > _max_weight) ? _current_weight : _max_weight;

                        std::tm temp_load_time = current_time;
                        add_time(temp_load_time, 0, start_size - current_remainder);
                        it->load_time = temp_load_time;
                        it->meet_arr = el_pas_id;

                        newcomers_id.push_back(it->_ID);

                        curr_passengers.push_back(*it);

                        _buttons_arr[it->_target_floor - 1].set_state(1); 

                        // remove passenger from main arr
                        it = building_arr[_state._curr_floor - 1].erase(it);
                    }
                    else // Exceeding weight limit
                    {
                        _exceed_limit_count += 1;
                        it->exceed_limit = 1;
                        ++it;
                    }
                }
                else
                {
                    ++it;
                }
            }

            // update current elevator passengers id
            for (auto& el_pas : curr_passengers)
            {
                // check if passenger isn't newcomer
                auto it = std::find(el_pas_id.begin(), el_pas_id.end(), el_pas._ID);
                if (it != el_pas_id.end())
                {
                    el_pas.meet_arr.insert(el_pas.meet_arr.end(), newcomers_id.begin(), newcomers_id.end());
                }
            }
        }

        set_state(doors_closed);
        // de increase seconds remainder in size of one minute
        current_remainder -= _sec_per_floor;
    }
    if (_floors_queue.empty())
    {
        // if we dont have any tasks - set state and wait
        _remainder_sec = 0;
    }
    else
    {
        // otherwise set new remainder and new state
        _remainder_sec = current_remainder;
        set_state(
            (_state._curr_floor < *_floors_queue.begin())
            ? upward_move
            : downward_move);
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

int elevator::get_id() const
{
    return _ID;
}

std::ostream& operator<<(std::ostream& os, const elevator& el)
{
    // Helper function for time printing (as provided)
    auto print_time_inner = [&os](const std::tm& time, const std::string& label) {
        char buf[80];
        strftime(buf, sizeof(buf), "%H:%M:%S", &time);
        os << label << buf;
        };

    // Print basic elevator info
    os << "Elevator ID: " << el.get_id() << "\n";
    os << "State: " << el.get_state()._state << " (";
    switch (el.get_state()._state) {
        case elevator::elevator_states::doors_open: os << "doors_open"; break;
        case elevator::elevator_states::doors_closed: os << "doors_closed"; break;
        case elevator::elevator_states::upward_move: os << "upward_move"; break;
        case elevator::elevator_states::downward_move: os << "downward_move"; break;
        default: os << "unknown"; break;
    }
    os << "), Current floor: " << el.get_state()._curr_floor << "\n";

    // Print weight information
    os << "Total weight: " << el._total_weight << " kg; " 
        << " Max weight: " << el._max_weight << " kg; "
        << "Exced limits count: " << el._exceed_limit_count 
        << "\n";

    // Print time information
    print_time_inner(el._total_work_time, "Total operating time: ");
    os << "\n";
    print_time_inner(el._total_waste_time, "Total wasted time: ");
    os << "\n";

    // Print other statistics
    os << "Floors count: " << el._floors_count << "; ";
    os << "Exceeded weight limit count: " << el._exceed_limit_count << "; ";
    os << "Last speed: " << el._sec_per_floor << " sec/floor; " << "\n";

    return os;
}