#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <random>
#include <string>
#include <algorithm>
#include <functional>
#include <cmath>

#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

enum event_type
{
    assembly = 0,
    recycle = 1
};

struct recipe_component
{
    int component_id;
    int quantity;
};

struct process_event
{
    long long finish_time;
    int process_type;
    int base_quality;
};

struct factory_config
{
    int assembly_machines;
    int recycle_machines;
    int assembly_modules;
    int recycle_modules;
    int t_assembly;
    int t_recycle;
    int component_count;
    std::vector<recipe_component> recipe;
    std::vector<int> arrival_rates;
};

double calculate_probability_increase(int modules, int k)
{
    double base = 0.52 * std::pow(10.0, -2.0 - k);
    return modules * base;
}

int generate_new_quality(int base_quality, int modules, std::mt19937 &rng)
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double r = dist(rng);

    double cumulative = 0.0;

    for(int k = 4; k >= 1; --k)
    {
        double p = calculate_probability_increase(modules, k);
        cumulative += p;
        if(r < cumulative)
        {
            return std::min(5, base_quality + k);
        }
    }

    return base_quality;
}

factory_config load_config(const std::string &file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + file_name);
    }
    
    factory_config config;
    std::string line;
    int line_count = 0;
    int expected_sections = 9;
    
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        
        std::istringstream iss(line);
        line_count++;
        
        switch (line_count)
        {
            case 1:
                iss >> config.assembly_machines;
                break;
            case 2:
                iss >> config.recycle_machines;
                break;
            case 3:
                iss >> config.assembly_modules;
                break;
            case 4:
                iss >> config.recycle_modules;
                break;
            case 5:
                iss >> config.t_assembly;
                break;
            case 6:
                iss >> config.t_recycle;
                break;
            case 7:
                iss >> config.component_count;
                config.recipe.resize(config.component_count);
                config.arrival_rates.resize(config.component_count);
                break;
            case 8:
                for(int i = 0; i < config.component_count; ++i)
                {
                    config.recipe[i].component_id = i;
                    if (!(iss >> config.recipe[i].quantity))
                    {
                        throw std::runtime_error("Not enough recipe values at line " + std::to_string(line_count));
                    }
                }
                break;
            case 9:
                for(int i = 0; i < config.component_count; ++i)
                {
                    if (!(iss >> config.arrival_rates[i]))
                    {
                        throw std::runtime_error("Not enough arrival rate values at line " + std::to_string(line_count));
                    }
                }
                break;
            default:
                throw std::runtime_error("Unexpected extra data in config file at line " + std::to_string(line_count));
        }
        
        if (iss.fail()) {
            throw std::runtime_error("Failed to read value at line " + std::to_string(line_count));
        }
    }
    
    if (line_count < expected_sections)
    {
        throw std::runtime_error("Incomplete config file: expected " + 
            std::to_string(expected_sections) + 
            " sections, got " + std::to_string(line_count));
    }
    
    return config;
}

int main()
{
    factory_config config = 
        load_config("/Users/stepanorlov/Documents/Stepan Orlov/RGU/semester 4/teor ver/tv_hw_3/task_3_config.txt");

    std::ofstream log_file("simulation_log.txt");

    std::map<int, std::map<int, long long>> component_stock;
    std::map<int, long long> product_stock;

    std::vector<long long> times_level_3;
    std::vector<long long> times_level_4;
    std::vector<long long> times_level_5;

    std::priority_queue<
        process_event,
        std::vector<process_event>,
        std::function<bool(process_event, process_event)>
    > event_queue(
        [](process_event a, process_event b)
        {
            return a.finish_time > b.finish_time;
        }
    );

    std::mt19937 rng(std::random_device{}());

    long long current_time = 0;
    int busy_assembly = 0;
    int busy_recycle = 0;
    int legendary_count = 0;

    std::cout << "Simulation started\n";

    while(legendary_count < 25)
    {
        log_file << "TIME: " << current_time << "\n";

        for(int i = 0; i < config.component_count; ++i)
        {
            component_stock[i][1] += config.arrival_rates[i];
        }

        //finish events
        while(!event_queue.empty() && event_queue.top().finish_time <= current_time)
        {
            process_event ev = event_queue.top();
            event_queue.pop();

            if(ev.process_type == event_type::assembly)
            {
                busy_assembly--;
                int new_quality = generate_new_quality(ev.base_quality, config.assembly_modules, rng);
                product_stock[new_quality]++;

                log_file << "Assembly finished. Quality: " << new_quality << "\n";

                if(new_quality == 3)
                {
                    times_level_3.push_back(current_time);
                }
                if(new_quality == 4)
                {
                    times_level_4.push_back(current_time);
                }
                if(new_quality == 5)
                {
                    times_level_5.push_back(current_time);
                    legendary_count++;
                    std::cout << "NEW LEGENDARY (" + std::to_string(legendary_count) + ")" << std::endl;
                }
            }
            else
            {
                busy_recycle--;
                int new_quality = generate_new_quality(ev.base_quality, config.recycle_modules, rng);

                log_file << "Recycle finished. New component quality: " << new_quality << "\n";

                for(auto &rc : config.recipe)
                {
                    long long returned = rc.quantity / 4;
                    component_stock[rc.component_id][new_quality] += returned;
                }
            }
        }

        //start assemleies
        while(busy_assembly < config.assembly_machines)
        {
            bool can_build = true;

            for(auto &rc : config.recipe)
            {
                if(component_stock[rc.component_id][1] < rc.quantity)
                {
                    can_build = false;
                    break;
                }
            }

            if(!can_build)
            {
                break;
            }

            for(auto &rc : config.recipe)
            {
                component_stock[rc.component_id][1] -= rc.quantity;
            }

            process_event ev;
            ev.process_type = event_type::assembly;
            ev.base_quality = 1;

            double slow_factor = 1.0 + 0.1 * config.assembly_modules;
            long long duration = static_cast<long long>(config.t_assembly * slow_factor);
            ev.finish_time = current_time + duration;

            log_file << "Assembly started. Finish at: " << ev.finish_time << "\n";

            event_queue.push(ev);
            busy_assembly++;
        }

        //start recycles 
        while(busy_recycle < config.recycle_machines)
        {
            int recycle_quality = 0;

            for(int q = 4; q >= 1; --q)
            {
                if(product_stock[q] > 0)
                {
                    recycle_quality = q;
                    break;
                }
            }

            if(recycle_quality == 0)
            {
                break;
            }

            product_stock[recycle_quality]--;

            process_event ev;
            ev.process_type = event_type::recycle;
            ev.base_quality = recycle_quality;

            double slow_factor = 1.0 + 0.1 * config.recycle_modules;
            long long duration = static_cast<long long>(config.t_recycle * slow_factor);
            ev.finish_time = current_time + duration;

            log_file << "Recycle started. Finish at: " << ev.finish_time << "\n";

            event_queue.push(ev);
            busy_recycle++;
        }

        log_file << "Legendary count: " << legendary_count << "\n";
        log_file << "--------------------------\n";

        current_time++;
    }

    std::cout << "Simulation finished at time: " << current_time << "\n";
    log_file << "Simulation finished at time: " << current_time << "\n";
    log_file.close();

    

    return 0;
}