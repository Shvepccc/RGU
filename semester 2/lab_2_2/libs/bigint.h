#ifndef COURSE1_CPP_RGU_bigint_H
#define COURSE1_CPP_RGU_bigint_H

#include <corecrt.h>
#include <iostream>

class bigint final
{

    static unsigned int get_max(
        unsigned int first,
        unsigned int second) noexcept
    {
        return first > second
            ? first
            : second;
    }

private:

    static constexpr unsigned int HALF_DIGIT_SHIFT = (sizeof(int) << 2);

    static constexpr unsigned int SHIFT = (sizeof(int) << 2);

    static constexpr unsigned int MASK = (1 << SHIFT) - 1;

private:

    static unsigned int get_loword(
        unsigned int value)
    {
        return value & MASK;
    }

    static unsigned int get_hiword(
        unsigned int value)
    {
        return value >> SHIFT;
    }

private:

    static void addition_for_multiplication(
        bigint& summand,
        unsigned int* words_multiplication_result_digits,
        unsigned int this_half_digit,
        unsigned int multiplier_half_digit,
        unsigned int shift_in_half_digits)
    {
        unsigned int pre_res = 
            static_cast<unsigned long long>(this_half_digit) * 
            static_cast<unsigned long long>(multiplier_half_digit);

        words_multiplication_result_digits[0] = static_cast<unsigned int>(pre_res & 0xFFFFFFFF);
        words_multiplication_result_digits[1] = static_cast<uint32_t>(pre_res >> 32);

        summand += (bigint(reinterpret_cast<int*>(words_multiplication_result_digits), 3) << 
            (HALF_DIGIT_SHIFT * shift_in_half_digits));
    }

private:

    int _oldest_digit;
    int* _other_digits;

private:

    void dispose();

    void copy_from(
        bigint const& other);

    void move_from(
        bigint&& other);

public:

    ~bigint() noexcept;

    bigint(
        bigint const& other);

    bigint(
        bigint&& other) noexcept;

    bigint& operator=(
        bigint const& other);

    bigint& operator=(
        bigint&& other) noexcept;

private:

    bigint& from_array(
        int const* digits_array,
        size_t digits_count);

public:

    bigint();

    bigint(
        int const* digits,
        size_t digits_count);

    bigint(
        char const* string_representation,
        size_t base);

    bigint(int digit);

private:

    bigint& _raw_positive_increment();
    bigint& _raw_positive_decrement();
    bigint& _raw_negative_increment();
    bigint& _raw_negative_decrement();

private:

    inline int get_sign() const noexcept;

    inline size_t get_digits_count() const noexcept;

    unsigned int operator[](
        size_t index) const noexcept;

    int& operator[](
        size_t index);

    bigint& negate()&;

    bigint& invert()&;

    int compare(bigint const& a, bigint const& b) const;

    void remove_leading_zeros();

public:

    bigint operator-() const;

    bigint& operator+=(
        bigint const& summand)&;

    bigint operator+(
        bigint const& summand) const;

    bigint& operator++();

    bigint const operator++(
        int);

    bigint& operator-=(
        bigint const& minuend)&;

    bigint operator-(
        bigint const& minuend) const;

    bigint& operator--();

    bigint const operator--(
        int);

    bigint& operator*=(
        bigint const& multiplier)&;

    bigint operator*(
        bigint const& multiplier) const;

    bigint& operator/=(
        bigint const& divisor)&;

    bigint operator/(
        bigint const& divisor) const;

    bigint& operator%=(
        bigint const& divisor)&;

    bigint operator%(
        bigint const& divisor) const;

    struct division_result
    {

        //bigint german;
        //bigint remainder;
//
        //division_result(
        //    bigint const &german,
        //    bigint const &remainder):
        //    german(german),
        //    remainder(remainder)
        //{
//
        //}

    };

    division_result division(
        bigint const& divisor) const;

    bigint abs() const;

public:

    bool operator==(
        bigint const& other) const;

    bool operator!=(
        bigint const& other) const;

public:

    bool operator<(
        bigint const& other) const;

    bool operator<=(
        bigint const& other) const;

    bool operator>(
        bigint const& other) const;

    bool operator>=(
        bigint const& other) const;

public:

    bigint operator~() const;

    bigint& operator&=(
        bigint const& other)&;

    bigint operator&(
        bigint const& other) const;

    bigint& operator|=(
        bigint const& other)&;

    bigint operator|(
        bigint const& other) const;

    bigint& operator^=(
        bigint const& other)&;

    bigint operator^(
        bigint const& other) const;

    bigint& operator<<=(
        size_t shift)&;

    bigint operator<<(
        size_t shift) const;

    bigint& operator>>=(
        size_t shift)&;

    bigint operator>>(
        size_t shift) const;

public:

    friend std::ostream& operator<<(
        std::ostream& stream,
        bigint const& value);

    friend std::istream& operator>>(
        std::istream& stream,
        bigint& value);

};

#endif //COURSE1_CPP_RGU_bigint_H