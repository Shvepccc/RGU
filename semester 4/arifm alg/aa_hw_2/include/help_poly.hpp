#ifndef HELP_POLY_H_
#define HELP_POLY_H_

#include <iostream>

void print_poly_vector(const std::vector<int>& poly)
{
    std::cout << "[";
    for (size_t i = 0; i < poly.size(); ++i)
    {
        std::cout << poly[i];
        if (i < poly.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "]";
}

std::string poly_to_string(const std::vector<int>& poly)
{
    if (poly.empty())
    {
        return "0";
    }
    
    std::string result;
    bool first = true;
    
    for (int i = static_cast<int>(poly.size()) - 1; i >= 0; --i)
    {
        if (poly[i] != 0)
        {
            if (!first)
            {
                result += " + ";
            }
            
            if (i == 0)
            {
                result += "1";
            }
            else if (i == 1)
            {
                result += "x";
            }
            else
            {
                result += "x^" + std::to_string(i);
            }
            
            first = false;
        }
    }
    
    if (first)
    {
        result = "0";
    }
    
    return result;
}

#endif