#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>

inline int convert_TO_decimal(const char* n, int base)
{
    if (n == nullptr)
        throw std::invalid_argument("Input string is null");
    if (base < 2 || base > 36)
        throw std::invalid_argument("Base should be in range from 2 to 36");

    int result = 0;
    int sign = 1;
    if (*n == '-') {
        sign = -1;
        ++n;
    }
    else if (*n == '+') {
        ++n;
    }

    while (*n) {
        char c = std::toupper(*n);
        int value;

        if (std::isdigit(c))
            value = c - '0';
        else if (std::isalpha(c))
            value = c - 'A' + 10;
        else
            throw std::invalid_argument("Invalid character in number");

        if (value >= base)
            throw std::invalid_argument("Digit outside base range");

        result = result * base + value;
        ++n;
    }

    return sign * result;
}

bool cmp_time(const std::tm& a, const std::tm& b)
{
    return a.tm_hour == b.tm_hour && a.tm_min == b.tm_min;
}

std::tm time_sub(const std::tm& a, const std::tm& b)
{
    std::tm result = a;

    if (a.tm_min < b.tm_min)
    {
        result.tm_hour -= 1;
        result.tm_min += 60;
    }

    result.tm_min -= b.tm_min;
    result.tm_hour -= b.tm_hour;

    return result;
}

#endif //HELPERS_H