#include "../bigint.h"
#include <bitset>
#include <cstdint>

#pragma region Supporting functions to Rule of five

void bigint::dispose()
{
    delete[] _other_digits;
    _other_digits = nullptr;
}

void bigint::copy_from(
    bigint const& other)
{
    _oldest_digit = other._oldest_digit;
    _other_digits = nullptr;

    if (other._other_digits != nullptr)
    {
        _other_digits = new int[*other._other_digits];
        memcpy(_other_digits, other._other_digits, *other._other_digits * sizeof(int));
    }
}

void bigint::move_from(
    bigint&& other)
{
    _oldest_digit = other._oldest_digit;
    other._oldest_digit = 0;

    _other_digits = other._other_digits;
    other._other_digits = nullptr;
}


#pragma endregion


#pragma region Rule of five

bigint::~bigint() noexcept
{
    dispose();
}

bigint::bigint(
    bigint const& other) :
    _other_digits(nullptr)
{
    copy_from(other);
}

bigint::bigint(
    bigint&& other) noexcept :
    _other_digits(nullptr)
{
    move_from(std::move(other));
}

bigint& bigint::operator=(
    bigint const& other)
{
    if (this != &other)
    {
        dispose();
        copy_from(other);
    }

    return *this;
}

bigint& bigint::operator=(
    bigint&& other) noexcept
{
    if (this != &other)
    {
        dispose();
        move_from(std::move(other));
    }

    return *this;
}

bigint::bigint() : _oldest_digit(0), _other_digits(nullptr) {}


bigint::bigint(int digit) : _oldest_digit(digit), _other_digits(nullptr) {}

bigint::bigint(
    int const* digits,
    size_t digits_count) :
    _other_digits(nullptr)
{
    from_array(digits, digits_count);
}

bigint::bigint(
    char const* string_representation,
    size_t base) :
    _other_digits(nullptr)
{
    // TODO: this requires implemented operator+= and operator*=
}
#pragma endregion


#pragma region Supporting functions

bigint& bigint::from_array(
    int const* digits_array,
    size_t digits_count)
{
    if (digits_array == nullptr)
    {
        throw std::invalid_argument("Pointer to digits array can't be EQ to nullptr");
    }

    if (digits_count == 0)
    {
        throw std::invalid_argument("Digits count can't be EQ to 0");
    }

    dispose();

    if (digits_count == 1)
    {
        _oldest_digit = *digits_array;
        return *this;
    }

    while (digits_count != 1 && 
        ((digits_array[digits_count - 1] == 0 && digits_array[digits_count - 2] >= 0) ||
        (digits_array[digits_count - 1] == -1 && digits_array[digits_count - 2] < 0)))
    {
        --digits_count;
    }

    *(_other_digits = new int[digits_count]) = static_cast<int>(digits_count);
    memcpy(_other_digits + 1, digits_array, (digits_count - 1) * sizeof(int));
    _oldest_digit = digits_array[digits_count - 1];

    return *this;
}

int bigint::get_sign() const noexcept
{
    if (_oldest_digit == 0 && _other_digits == nullptr)
    {
        return 0;
    }

    return _oldest_digit > 0
        ? 1
        : -1;
}

size_t bigint::get_digits_count() const noexcept
{
    return _other_digits == nullptr
        ? 1
        : *_other_digits;
}

unsigned int bigint::operator[](
    size_t index) const noexcept
{
    auto const digits_count = get_digits_count();

    if (index >= digits_count)
    {
        return 0;
    }

    return *reinterpret_cast<unsigned int const*>((index == digits_count - 1)
        ? &_oldest_digit
        : _other_digits + 1 + index);
}

int& bigint::operator[](
    size_t index)
{
    auto const digits_count = get_digits_count();

    if (index >= digits_count)
    {   
        throw std::out_of_range("out of range of digits array");
    }

    return (index == digits_count - 1)
        ? _oldest_digit
        : *(_other_digits + 1 + index);
}

bigint& bigint::invert()&
{
    _oldest_digit = ~_oldest_digit;
    if (_other_digits == nullptr)
    {
        return *this;
    }

    for (int i = 1; i < *_other_digits - 1; ++i)
    {
        _other_digits[i] = ~_other_digits[i];
    }

    return *this;
}

void bigint::remove_leading_zeros() 
{
    size_t size = this->get_digits_count();
    while (size > 1 && (((*this)[size - 1] == 0 && (*this)[size - 2] >= 0))) 
    {
        --size;
    }
    if (size < this->get_digits_count()) 
    {
        _oldest_digit = _other_digits[size];
        _other_digits[0] = static_cast<int>(size);
    }
}

#pragma endregion


#pragma region  Raw increments and decrements

bigint& bigint::_raw_positive_increment() 
{
    auto const digits_count = get_digits_count();

    for (int i = 0; i < digits_count - 1; ++i) 
    {
        if (++((*this)[i]) != 0) 
        { 
            return *this;
        }
    }

    if (++_oldest_digit != INT_MIN) 
    {
        return *this;
    }

    if (_other_digits == nullptr) 
    {
        _other_digits = new int[2];
        _other_digits[0] = 2;
        _other_digits[1] = _oldest_digit;
        _oldest_digit = 0;

        return *this;
    }

    int* new_array = new int[digits_count + 1];
    std::memcpy(new_array, _other_digits, sizeof(int) * digits_count);
    delete[] _other_digits;
    _other_digits = new_array;
    new_array = nullptr;

    (*this)[digits_count] = _oldest_digit;
    ++(*this)._other_digits[0];
    _oldest_digit = 0;

    return *this;
}

bigint& bigint::_raw_positive_decrement()
{
    if (get_sign() == 0) 
    {
        _oldest_digit = -1;
        return *this;
    }

    auto const digits_count = get_digits_count();
    for (int i = 0; i < digits_count - 1; ++i) 
    {
        if (--((*this)[i]) != -1) 
        {
            remove_leading_zeros();
            return *this;
        }
    }

    if (--_oldest_digit != INT_MAX) 
    {
        remove_leading_zeros();
        return *this;
    }

    if (_other_digits == nullptr) 
    {
        _other_digits = new int[2];
        _other_digits[0] = 2;
        _other_digits[1] = _oldest_digit;
        _oldest_digit = -1;

        remove_leading_zeros();
        return *this;
    }

    int* new_array = new int[digits_count + 1];
    memcpy(new_array, _other_digits, sizeof(int) * digits_count);
    delete[] _other_digits;
    _other_digits = new_array;

    (*this)[digits_count] = _oldest_digit;
    --(*this)._other_digits[0];
    _oldest_digit = 0;

    remove_leading_zeros();
    return *this;
}

bigint& bigint::_raw_negative_increment() 
{
    return *this += 1; //TODO: make it properly
}

bigint& bigint::_raw_negative_decrement() 
{
    return *this += -1; //TODO: make it properly
}


#pragma endregion


bigint& bigint::negate()&
{
    if (get_sign() == 0)
    {
        return *this;
    }

    if (get_sign() == 1)
    {
        invert();
        _raw_negative_increment(); 
    }
    else
    {
        _raw_positive_increment(); 
        invert();
    }
    return *this;
}

bigint bigint::operator-() const
{
    bigint negative(*this);
    return negative.negate();
}

bigint bigint::abs() const {
    return get_sign() < 0 ? -*this : *this;
}

bigint& bigint::operator+=(bigint const& other)& {

    int this_sign = get_sign();
    int other_sign = other.get_sign();

    if (this_sign == 0) {
        return *this = other;
    }

    if (other_sign == 0) {
        return *this;
    }

    auto this_size = get_digits_count();
    auto other_size = other.get_digits_count();
    unsigned int max_size = get_max(this_size, other_size) + 1;

    int* result = new int[max_size];
    uint64_t extra_digit = 0;

    for (int i = 0; i < max_size; ++i)
    {
        uint64_t this_digit = (i < this_size) ? static_cast<uint64_t>((*this)[i]) : 0;
        uint64_t other_digit = (i < other_size) ? static_cast<uint64_t>(other[i]) : 0;

        uint64_t res_sum = this_digit + other_digit + extra_digit;
        extra_digit = res_sum >> 32;
        result[i] = static_cast<int>(static_cast<uint32_t>(res_sum));
    }    

    // for (int i = 0; i < max_size; ++i)
    // {
    //     std::cout << result[i] << " ";
    // }
    // std::cout << std::endl;

    from_array(result, max_size);
    delete[] result;
    return *this;
}

bigint bigint::operator+(
    bigint const& summand) const
{
    bigint result(*this);

    return result += summand;
}

bigint& bigint::operator++()
{
    if (get_sign() == 1)
    {
       return _raw_positive_increment();
    }
    return _raw_negative_increment();
}

bigint const bigint::operator++(int)
{
    bigint curr(*this);
    ++(*this);
    return curr;
}

bigint& bigint::operator-=(
    bigint const& minuend)&
{
    return *this += -minuend;
}

bigint bigint::operator-(
    bigint const& minuend) const
{
    bigint result(*this);

    return result -= minuend;
}

bigint& bigint::operator--()
{
    if (get_sign() == 1)
    {
        _raw_positive_decrement(); 
    }
    return _raw_negative_decrement(); 
}

bigint const bigint::operator--(
    int)
{
    bigint x = *this;
    --*this;
    return x;
}

bigint& bigint::operator*=(
    bigint const& other)&
{
    if (get_sign() == 0 || other.get_sign() == 0)
    {
        *this = 0;
        return *this;
    }

    if (other == 1) { return *this; }
    if (*this == 1) { return *this = other; }

    if (other == -1) { return this->negate(); }
    if (*this == -1) { return *this = -other; }

    if (get_sign() == -1 && other.get_sign() == -1)
    {
        return this->negate() *= -other;
    }

    if (get_sign() == -1)
    {
        return (this->negate() *= other).negate();
    }
    if (other.get_sign() == -1)
    {
        return (*this *= -other).negate();
    }

    unsigned int words_multiplication_result_digits[3] = { 0 };
    bigint result = 0;
    auto this_digits_count = get_digits_count();
    auto _other_digitscount = other.get_digits_count();

    for (size_t i = 0; i < this_digits_count; ++i)
    {
        auto this_digit = (*this)[i];
        auto this_digit_loword = get_loword(this_digit);
        auto this_digit_hiword = get_hiword(this_digit);

        for (size_t j = 0; j < _other_digitscount; ++j)
        {
            auto multiplier_digit = other[j];
            auto multiplier_digit_loword = get_loword(multiplier_digit);
            auto multiplier_digit_hiword = get_hiword(multiplier_digit);

            addition_for_multiplication(result, words_multiplication_result_digits, 
                this_digit_loword, multiplier_digit_loword, (i + j) << 1); //may be this shifts don't work )))

            addition_for_multiplication(result, words_multiplication_result_digits, 
                this_digit_loword, multiplier_digit_hiword, (i + j + 1) << 1);

            addition_for_multiplication(result, words_multiplication_result_digits, 
                this_digit_hiword, multiplier_digit_loword, (i + j + 1) << 1);

            addition_for_multiplication(result, words_multiplication_result_digits, 
                this_digit_hiword, multiplier_digit_hiword, (i + j + 2) << 1);
        }
    }

    return *this = std::move(result);
}

bigint bigint::operator*(
    bigint const& other) const
{
    bigint result = *this;
    return result *= other;
}

bigint& bigint::operator/=(
    bigint const& other)&
{
    return *this;
}

bigint bigint::operator/(
    bigint const& other) const
{
    bigint result = *this;
    return result /= other;
}

bigint& bigint::operator%=(
    bigint const& other)&
{
    return *this;
}

bigint bigint::operator%(
    bigint const& other) const
{
    bigint result = *this;
    return result %= other;
}


#pragma region Left and right shifts

bigint& bigint::operator<<=(
    size_t shift)&
{
    if (shift == 0 || get_sign() == 0) 
    {
        return *this;
    }

    unsigned int const bits_per_digit = sizeof(int) << 3;
    unsigned int const zeros_to_add_count = static_cast<int>(shift / bits_per_digit);
    //shift &= bits_per_digit - 1;
    shift = (shift - (zeros_to_add_count * bits_per_digit));

    unsigned int bits_from_previous_digit = 0;
    unsigned int bits_for_next_digit = 0;
    auto const digits_count = get_digits_count();
    auto const old_sign = get_sign();

    if (shift != 0) 
    {
        const int mask = (1 << shift) - 1;
        for (int i = 0; i < digits_count; ++i) 
        {
            bits_for_next_digit = (static_cast<bigint const*>(this)->operator[](i) >>
                (bits_per_digit - shift)) & mask; //Why we use mask? We won't get the same result without it?

            unsigned int result = (this->operator[](i) << static_cast<int>(shift)) |
                bits_from_previous_digit;

            this->operator[](i) = *reinterpret_cast<int*>(&result);

            bits_from_previous_digit = bits_for_next_digit;
        }
    }

    size_t numbers_to_add_after_shift_overflows = (bits_from_previous_digit != 0) ? 1 : 0;

    unsigned int oldest_digit_in_new_array = numbers_to_add_after_shift_overflows > 0 
        ? bits_from_previous_digit
        : _oldest_digit;

    size_t additional_oldest_zero = 0;
    if (old_sign >= 0 && _oldest_digit < 0) 
    {
        additional_oldest_zero = 1;
    }

    size_t new_array_size = 
        digits_count + zeros_to_add_count + 
        numbers_to_add_after_shift_overflows + additional_oldest_zero;

    if (new_array_size > digits_count) 
    {
        auto* new_array = new int[new_array_size];

        std::memcpy(new_array + zeros_to_add_count, _other_digits + 1, (digits_count - 1) * sizeof(int));

        new_array [new_array_size - 1 - numbers_to_add_after_shift_overflows - additional_oldest_zero] = _oldest_digit;

        if (numbers_to_add_after_shift_overflows == 1) 
        {
            new_array [new_array_size - 1 - additional_oldest_zero] =
                *reinterpret_cast<int*>(&bits_from_previous_digit);
        }
        if (additional_oldest_zero == 1) 
        {
            new_array[new_array_size - 1] = 0;
        }
        std::memset(new_array, 0, zeros_to_add_count * sizeof(int));

        from_array(new_array, new_array_size);

        delete[] new_array;
    }

    remove_leading_zeros();

    return *this;
}

bigint bigint::operator<<(
    size_t shift) const
{
    bigint result = *this;
    return result <<= shift;
}

bigint& bigint::operator>>=(
    size_t shift)&
{
    int const old_sign = get_sign();
    if (shift == 0 || old_sign == 0) 
    {
        return *this;
    }

    int const bits_per_digit = sizeof(int) << 3;
    int const digits_to_remove = static_cast<int>(shift / bits_per_digit);
    //shift &= bits_per_digit - 1;
    shift = (shift - (digits_to_remove * bits_per_digit));

    if (digits_to_remove >= get_digits_count()) 
    {
        *this = 0;
        return *this;
    }

    unsigned int bits_from_previous_digit = 0;
    unsigned int bits_for_next_digit = 0;
    auto const digits_count = get_digits_count();

    if (shift != 0) 
    {
        const unsigned int mask = (1 << shift) - 1;
        for (int i = digits_count - 1; i >= 0; --i) 
        {
            bits_for_next_digit = (static_cast<bigint const*>(this)->operator[](i) & mask)
                << (bits_per_digit - shift);

            unsigned int result = (static_cast<bigint const*>(this)->operator[](i) >> static_cast<int>(shift)) | 
                bits_from_previous_digit;

            bits_from_previous_digit = bits_for_next_digit;

            this->operator[](i) = *reinterpret_cast<int*>(&result);

            if (old_sign == -1) // Why we need it?
            {
                const int mask_2 = INT_MIN >> (shift - 1);
                this->operator[](i) |= mask_2;
            }
        }
    }

    size_t new_array_size = digits_count - digits_to_remove;
    if (new_array_size == 0) 
    {
        *this = 0;
        remove_leading_zeros();
        return *this;
    }

    if (digits_to_remove > 0) 
    {
        auto* new_array = new int[new_array_size];
        std::memcpy(new_array, _other_digits + 1 + digits_to_remove, (new_array_size - 1) * sizeof(int));
        new_array[new_array_size - 1] = _oldest_digit;

        from_array(new_array, new_array_size);
        delete[] new_array;
    }

    remove_leading_zeros();
    return *this;
}

bigint bigint::operator>>(
    size_t shift) const
{
    bigint result = *this;
    return result >>= shift;
}

#pragma endregion



#pragma region Compare functions (with Supporting)

int bigint::compare(bigint const &a, bigint const &b) const
{
    int sign = 1;
    size_t a_digits_count = a.get_digits_count();
    size_t b_digits_count = a.get_digits_count();

    if (a_digits_count > b_digits_count || a.get_sign() < b.get_sign())
        return 1;
    if (b_digits_count > a_digits_count || a.get_sign() > b.get_sign())
        return -1;
    if (a.get_sign() == 0 && b.get_sign() == 0)
        return 0;
    if (a.get_sign() == -1 && b.get_sign() == -1)
        return -1;

    for (int i = a_digits_count; i > 0; ++i)
    {
        if (a[i] > b[i])
            return 1 * sign;
        if (b[i] > a[i])
            return -1 * sign;
    }
    return 0;
}

bool bigint::operator==(
    bigint const& other) const
{
    return compare(*this, other) == 0;
}

bool bigint::operator!=(
    bigint const& other) const
{
    return compare(*this, other) != 0;
}

bool bigint::operator<(
    bigint const& other) const
{
    return compare(*this, other) < 0;
}

bool bigint::operator<=(
    bigint const& other) const
{
    return compare(*this, other) <= 0;
}

bool bigint::operator>(
    bigint const& other) const
{
    return compare(*this, other) > 0;
}

bool bigint::operator>=(
    bigint const& other) const
{
    return compare(*this, other) >= 0;
}

#pragma endregion 


#pragma region Bit operations (apart from shifts)

bigint bigint::operator~() const
{
    bigint result = *this;
    return result.invert();
}

bigint& bigint::operator&=(
    bigint const& other)&
{
    auto const first_size = get_digits_count();
    for (int i = 0; i < first_size; ++i) 
    {
        unsigned int const other_digit = other[i];
        auto const other_digit_int = *reinterpret_cast<int const*>(&other_digit);
        this->operator[](i) &= other_digit_int;
    }
    return *this;
}

bigint bigint::operator&(
    bigint const& other) const
{
    bigint result = *this;
    return result &= other;
}

bigint& bigint::operator|=(
    bigint const& other)&
{
    auto const first_size = get_digits_count();
    for (int i = 0; i < first_size; ++i)
    {
        unsigned int const other_digit = other[i];
        auto const other_digit_int = *reinterpret_cast<int const*>(&other_digit);
        this->operator[](i) |= other_digit_int;
    }
    return *this;
}

bigint bigint::operator|(
    bigint const& other) const
{
    bigint result = *this;
    return result |= other;
}

bigint& bigint::operator^=(
    bigint const& other)&
{
    auto const first_size = get_digits_count();
    for (int i = 0; i < first_size; ++i)
    {
        unsigned int const other_digit = other[i];
        auto const other_digit_int = *reinterpret_cast<int const*>(&other_digit);
        this->operator[](i) ^= other_digit_int;
    }
    return *this;
}

bigint bigint::operator^(
    bigint const& other) const
{
    bigint result = *this;
    return result ^= other;
}

#pragma endregion   

//TODO: write this region
#pragma region IO functions (with supporting)

std::ostream& operator<<(
    std::ostream& stream,
    bigint const& value)
{
    bool BITSET = false;

    auto digits_count = value.get_digits_count();
    for (auto i = 0; i < digits_count; ++i)
    {
        if (BITSET) 
        {
            stream << std::bitset<32>(const_cast<bigint&>(value)[i]) << ' ';
        }
        else
        {
            stream << const_cast<bigint&>(value)[i] << ' ';
        }
    }

    return stream;
}

std::istream& operator>>(
    std::istream& stream,
    bigint& value)
{


    return stream;
}

#pragma endregion

// unsigned int (*loword_hiword_function_pointers[])(unsigned int) =
// { get_loword, get_hiword };
// 
// int extra_multipier = (this_sign == -1 || other_sign == -1) ? - 1 : 1;
// 
// for (int i = 0; i < max_size; ++i) {
//     result[i] = 0;
// 
//     auto this_digit = static_cast<bigint const*>(this)->operator[](i);
//     auto other_digit = other[i];
// 
//     for (int j = 0; j < 2; ++j) {
//         auto this_half_digit = loword_hiword_function_pointers[j](this_digit);
//         auto other_half_digit = loword_hiword_function_pointers[j](other_digit);
// 
//         auto digits_sum = this_half_digit + other_half_digit + extra_digit;
//         extra_digit = (digits_sum >> SHIFT) * extra_multipier;
//         result[i] += static_cast<int>((digits_sum & MASK) << (j * SHIFT));
//     }
// }
// 