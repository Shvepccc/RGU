#ifndef _MATRIX_H
#define _MATRIX_H
#include <iostream>
#include "vector.h"

class matrix
{
private:
	double** data;
	size_t _rows_count, _columns_count;
	void clear_data(matrix& arg);
public:
	friend void deep_copy(matrix& arg_1, const matrix& arg_2);
	matrix(size_t rows_count, size_t columns_count, double initial_value = 0);

	virtual ~matrix();
	matrix(const matrix& arg);
	matrix& operator = (matrix const& arg);

	void print(std::ostream& os = std::cout) const;
	double det();
	matrix transp();
	matrix reverse();
	int rank() const;


	class RowProxy {
	private:
		double*& data_row;
		size_t col_count;
	public:
		RowProxy(double*& r, size_t n) : data_row(r), col_count(n) {}

		double& operator[](size_t col) {
			if (col >= col_count) {
				throw std::out_of_range("Column index out of range");
			}
			return data_row[col];
		}
	};

	RowProxy operator [](size_t row) {
		if (row >= _rows_count) {
			throw std::out_of_range("Row index out of range");
		}
		return RowProxy(data[row], _columns_count);
	}

	matrix& operator +=(matrix const& arg);
	matrix& operator -=(matrix const& arg);
	matrix& operator *=(matrix const& arg);
	matrix& operator *=(double const& arg);

	matrix operator +(matrix const& arg) const;
	matrix operator -(matrix const& arg) const;
	matrix operator *(matrix const& arg) const;
	matrix operator *(double const& arg) const;
	friend matrix operator *(double const& val, matrix const& arg);

	friend std::ostream& operator<<(std::ostream& os, const matrix& m);


	friend matrix solve_gauss(matrix& A, matrix& b);
	friend int size_of_linear_shell(cvector** vectors, int count);
	friend bool is_vector_in_span(const cvector& vec, cvector** basis_vectors, int basis_count);
};

matrix solve_gauss(matrix& a, matrix& b);
int size_of_linear_shell(cvector** vectors, int count);
bool is_vector_in_span(const cvector& vec, cvector** basis_vectors, int basis_count);

#endif // !_MATRIX_H
