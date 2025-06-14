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

inline bool cmp_time(const std::tm& a, const std::tm& b)
{
    return a.tm_hour == b.tm_hour && a.tm_min == b.tm_min;
}

inline std::tm time_sub(const std::tm& a, const std::tm& b)
{
    long total_a = a.tm_hour * 3600 + a.tm_min * 60 + a.tm_sec;
    long total_b = b.tm_hour * 3600 + b.tm_min * 60 + b.tm_sec;

    long diff = total_a - total_b;

    if (diff < 0) diff = 0;

    std::tm result = { 0 };
    result.tm_hour = diff / 3600;
    diff %= 3600;
    result.tm_min = diff / 60;
    result.tm_sec = diff % 60;

    return result;
}

inline void add_time(std::tm& t, int minutes, int seconds = 0)
{
    int sec_remainder = (seconds + t.tm_sec) / 60;
    t.tm_min += sec_remainder;
    t.tm_sec += seconds - (60 * sec_remainder);

    int min_remainder = (minutes + t.tm_min) / 60;
    t.tm_hour += min_remainder;
    t.tm_min += minutes - (60 * min_remainder);
}

inline void print_time(std::ostream& os, const std::tm& time, const std::string& label = "")
{
    char buf[80];
    strftime(buf, sizeof(buf), "%H:%M:%S", &time);
    os << label << buf;
}

#endif //HELPERS_H