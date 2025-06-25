#ifndef CVECTOR_H_
#define CVECTOR_H_

#include <iostream>
#include <stdexcept>

class cvector
{
private:
    double* data;
    int _size;
    int _capacity;

    void reallocate(int new_capacity)
    {
        double* new_data = new double[new_capacity];
        for (int i = 0; i < _size; ++i)
        {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        _capacity = new_capacity;
    }

public:
    cvector() : 
        data(nullptr), _size(0), _capacity(0) 
    {}

    ~cvector()
    {
        delete[] data;
    }

    void push_back(const double& value)
    {
        if (_size == _capacity)
        {
            int new_capacity = (_capacity == 0) ? 16 : _capacity * 2;
            reallocate(new_capacity);
        }
        data[_size++] = value;
    }

    void pop_back()
    {
        if (_size == 0)
            throw std::out_of_range("pop_back on empty vector");
        --_size;
    }

    double& operator[](int index)
    {
        if (index >= _size)
            throw std::out_of_range("index out of bounds");
        return data[index];
    }

    const double& operator[](int index) const
    {
        if (index >= _size)
            throw std::out_of_range("index out of bounds");
        return data[index];
    }

    int size() const
    {
        return _size;
    }

    bool empty() const
    {
        return _size == 0;
    }

    void clear()
    {
        _size = 0;
    }

    cvector(const cvector& other) :
        data(nullptr), _size(other._size), _capacity(other._size)
    {
        if (other._size > 0)
        {
            data = new double[other._size];
            for (int i = 0; i < other._size; ++i)
            {
                data[i] = other.data[i];
            }
        }
    }

#pragma region operators

    cvector& operator+=(const cvector& other)
    {
        if (_size != other._size)
        {
            throw std::invalid_argument("Vectors must be of same size for +=");
        }
        for (int i = 0; i < _size; ++i)
        {
            data[i] += other[i];
        }
        return *this;
    }

    cvector& operator-=(const cvector& other)
    {
        if (_size != other._size)
        {
            throw std::invalid_argument("Vectors must be of same size for -=");
        }
        for (int i = 0; i < _size; ++i)
        {
            data[i] -= other[i];
        }
        return *this;
    }

    double operator*=(const cvector& other) const
    {
        return scalar_product(*this, other);
    }

    cvector& operator*=(double scalar)
    {
        for (int i = 0; i < _size; ++i)
        {
            data[i] *= scalar;
        }
        return *this;
    }

    cvector& operator/=(double scalar)
    {
        if (scalar == 0) {
            throw std::invalid_argument("Division by zero");
        }
        for (int i = 0; i < _size; ++i)
        {
            data[i] /= scalar;
        }
        return *this;
    }


    friend cvector operator+(cvector a, const cvector& b) {
        a += b;
        return a;
    }

    friend cvector operator-(cvector a, const cvector& b) {
        a -= b;
        return a;
    }

    friend cvector operator*(cvector a, double scalar) {
        a *= scalar;
        return a;
    }

    friend cvector operator*(double scalar, cvector a) {
        a *= scalar;
        return a;
    }

    friend cvector operator/(cvector a, double scalar) {
        a /= scalar;
        return a;
    }


    friend std::ostream& operator<<(std::ostream& os, const cvector& vec) {
        os << "[";
        for (int i = 0; i < vec.size(); ++i) {
            os << vec[i];
            if (i != vec.size() - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

#pragma endregion


#pragma region labs_tasks

    friend double scalar_product(const cvector& a, const cvector& b)
    {
        if (a.size() != b.size())
        {
            throw std::invalid_argument("Vectors must be of the same size");
        }

        double result = 0;
        for (int i = 0; i < a.size(); ++i)
        {
            result += a[i] * b[i];
        }
        return result;
    }

    friend cvector cross_product_3d(const cvector& a, const cvector& b)
    {
        if (a.size() != 3 || b.size() != 3)
        {
            throw std::invalid_argument("Both vectors must be 3-dimensional");
        }

        cvector result;
        result.push_back(a[1] * b[2] - a[2] * b[1]);
        result.push_back(a[2] * b[0] - a[0] * b[2]);
        result.push_back(a[0] * b[1] - a[1] * b[0]);
        return result;
    }

    friend cvector cross_product_7d(const cvector& a, const cvector& b)
    {
        if (a.size() != 7 || b.size() != 7)
        {
            throw std::invalid_argument("Both vectors must be 7-dimensional");
        }

        cvector result;
        for (int i = 0; i < 7; ++i)
        {
            double sum = 0;

            for (int k = 1; k <= 3; ++k)
            {
                int j = (i + k) % 7;
                int m = (i + 7 - k) % 7;

                if (k % 2 == 1) {
                    sum += a[j] * b[m];
                }
                else {
                    sum -= a[j] * b[m];
                }
            }

            result.push_back(sum);
        }
        return result;
    }

    friend double mixed_product_3d(const cvector& a, const cvector& b, const cvector& c)
    {
        return scalar_product(a, cross_product_3d(b, c));
    }

    friend double mixed_product_7d(const cvector& a, const cvector& b, const cvector& c)
    {
        return scalar_product(a, cross_product_7d(b, c));
    }

    friend double module(const cvector& vec)
    {
        int res = 0;
        for (int i = 0; i < vec._size; ++i)
        {
            res += vec[i] * vec[i];
        }
        return sqrt(res);
    }

    friend cvector* gram_schmidt(cvector** vectors, int count)
    {
        if (count <= 0 || vectors == nullptr) {
            throw std::invalid_argument("Invalid input parameters");
        }

        int dimension = vectors[0]->size();

        for (int i = 0; i < count; ++i)
        {
            if (vectors[i]->size() != dimension)
            {
                throw std::invalid_argument("All vectors must have the same dimension");
            }
        }

        cvector* result = new cvector[count];

        for (int i = 0; i < count; ++i)
        {
            result[i] = *vectors[i];
        }

        for (int i = 0; i < count; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                double coef = scalar_product(result[i], result[j]) /
                    scalar_product(result[j], result[j]);

                result[i] -= result[j] * coef;
            }
        }

        return result;
    }


#pragma endregion

};

#endif