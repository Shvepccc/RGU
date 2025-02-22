#ifndef _MATRIX_H
#define _MATRIX_H
#include <iostream>

class matrix
{
private:
	double** data;
	size_t _rows_count, _columns_count;

public:
	matrix(size_t rows_count, size_t columns_count, double initial_value);

	~matrix();
	matrix(const matrix& arg);
	matrix& operator = (matrix const& arg);

	void print() const;

	double det();
	matrix& transp();
	matrix& reverse();


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
};

#endif // !_MATRIX_H
