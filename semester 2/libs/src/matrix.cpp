#include "../../libs/matrix.h"

matrix::matrix(size_t rows_count, size_t columns_count, double initial_value = 0.0)
{
	int i, j;
	data = new double* [rows_count];
	for (i = 0; i < columns_count; ++i)
	{
		data[i] = new double[columns_count];
		for (j = 0; j < columns_count; j++)
		{
			data[i][j] = initial_value;
		}
	}
	_rows_count = rows_count;
	_columns_count = columns_count;
}

matrix::~matrix()
{
	for (int i = 0; i < _columns_count; ++i)
	{
		delete[] data[i];
	}
	delete[] data;
	data = nullptr;
}

matrix::matrix(const matrix& arg)
{
	int i, j;
	data = new double* [arg._rows_count];
	for (i = 0; i < arg._columns_count; ++i)
	{
		data[i] = new double[arg._columns_count];
		for (j = 0; j < arg._columns_count; j++)
		{
			data[i][j] = arg.data[i][j];
		}
	}
	_rows_count = arg._rows_count;
	_columns_count = arg._columns_count;
}

matrix& matrix::operator = (matrix const& arg)
{
	int i, j;

	//check if arg it me
	if (this == &arg)
		return *this;

	//clear old data
	for (int i = 0; i < _columns_count; ++i)
	{
		delete[] data[i];
	}
	delete[] data;
	data = nullptr;

	//set new data
	data = new double* [arg._rows_count];
	for (i = 0; i < arg._columns_count; ++i)
	{
		data[i] = new double[arg._columns_count];
		for (j = 0; j < arg._columns_count; j++)
		{
			data[i][j] = arg.data[i][j];
		}
	}
	_rows_count = arg._rows_count;
	_columns_count = arg._columns_count;

	return *this;
}

void matrix::print() const
{
	int i, j;
	for (i = 0; i < _rows_count; ++i)
	{
		for (j = 0; j < _columns_count; ++j)
		{
			std::cout << data[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

// Operators -------------------------------------------------------
matrix& matrix::operator +=(matrix const& arg)
{
	if (this->_rows_count == arg._rows_count &&
		this->_columns_count == arg._columns_count)
	{
		int i, j;
		for (i = 0; i < this->_rows_count; ++i)
		{
			for (j = 0; j < this->_columns_count; ++j)
			{
				this->data[i][j] += arg.data[i][j];
			}
		}
	}
	else
	{
		throw std::invalid_argument("Matrix with different size in '+='");
	}

	return *this;
}

matrix& matrix::operator -=(matrix const& arg)
{
	if (this->_rows_count == arg._rows_count &&
		this->_columns_count == arg._columns_count)
	{
		int i, j;
		for (i = 0; i < this->_rows_count; ++i)
		{
			for (j = 0; j < this->_columns_count; ++j)
			{
				this->data[i][j] -= arg.data[i][j];
			}
		}
	}
	else
	{
		throw std::invalid_argument("Matrix with different size in '+='");
	}

	return *this;
}


matrix& matrix::operator *=(matrix const& arg)
{
	int i, j, k;
	matrix res(this->_rows_count, arg._columns_count);
	if (this->_columns_count == arg._rows_count)
	{
		for (i = 0; i < this->_rows_count; ++i)
		{
			for (j = 0; j < arg._columns_count; ++j)
			{
				for (k = 0; k < this->_columns_count; ++k)
				{
					res.data[i][j] += this->data[i][k] * arg.data[k][j];
				}
			}
		}
	}
	else
	{
		throw std::invalid_argument("Matrix with different size in '+='");
	}
	*this = res;
	return *this;
}

matrix& matrix::operator *=(double const& arg)
{
	int i, j;
	for (i = 0; i < this->_rows_count; ++i)
	{
		for (j = 0; j < this->_columns_count; ++j)
		{
			this->data[i][j] *= arg;
		}
	}

	return *this;
}

matrix matrix::operator +(matrix const& arg) const 
{
	matrix copy = *this;
	return copy += arg;
}

matrix matrix::operator -(matrix const& arg) const
{
	matrix copy = *this;
	return copy -= arg;
}

matrix matrix::operator *(matrix const& arg) const
{
	matrix copy = *this;
	return copy += arg;
}

matrix matrix::operator *(double const& arg) const
{
	matrix copy = *this;
	return copy *= arg;
}

matrix operator *(double const& val, matrix const& arg) 
{
	return arg * val;
}

void find_triangular_matrix(matrix& A, matrix& L, matrix& U, int size)
{
	int i, j, k;

	for (i = 0; i < size; i++) 
	{
		for (j = 0; j < size; j++) 
		{
			if (i == j)
				L[i][j] = 1.0;
			else
				L[i][j] = 0.0;

			U[i][j] = 0.0;
		}
	}

	for (i = 0; i < size; i++) 
	{

		for (j = i; j < size; j++) 
		{
			U[i][j] = A[i][j];
			for (k = 0; k < i; k++) 
			{
				U[i][j] -= L[i][k] * U[k][j];
			}
		}

		for (j = i + 1; j < size; j++) 
		{
			L[j][i] = A[j][i];
			for (k = 0; k < i; k++) 
			{
				L[j][i] -= L[j][k] * U[k][i];
			}
			L[j][i] /= U[i][i];
			if (U[i][i] == 0) 
			{
				throw std::overflow_error("Division by zero exception");
			}
		}

	}
}

double find_determinant(matrix& arg, int size)
{
	double res = 1.0;
	for (int i = 0; i < size; i++) {
		res *= arg[i][i];
	}
	return res;
}

double matrix::det() 
{
	if (this->_rows_count != this->_columns_count) 
	{
		throw std::invalid_argument("Determinant avialable only for square matrix");
	}
	else
	{
		matrix matrix_L(this->_columns_count, this->_columns_count);
		matrix matrix_U(this->_columns_count, this->_columns_count);
		try
		{
			find_triangular_matrix(*this, matrix_L, matrix_U, this->_columns_count);
		}
		catch (const std::exception&)
		{
			return 0.0;
		}
		return find_determinant(matrix_U, this->_columns_count);
	}
	return 0.0;
}

matrix& matrix::transp()
{
	int x, y;
	matrix res_matrix(this->_columns_count, this->_rows_count);

	for (x = 0; x < this->_columns_count; ++x) 
	{
		for (y = 0; y < this->_rows_count; ++y) 
		{
			res_matrix[y][x] = (*this)[x][y];
		}
	}
	*this = res_matrix;
	return *this;
}

matrix& matrix::reverse()
{
	if (this->_rows_count != this->_columns_count)
	{
		throw std::invalid_argument("Determinant avialable only for square matrix");
	}
	else
	{
		double support_element;
		int i, j, k = 0;
		int size = this->_columns_count;

		matrix inverse(size, size);
		
		for (i = 0; i < size; i++) 
		{
		    for (j = 0; j < size; j++) 
			{
		        if (i == j)
		            inverse[i][j] = 1.0;
		        else
		            inverse[i][j] = 0.0;
		
		    }
		}
		
		for (k = 0; k < size; k++) 
		{
		
		    //Check for divizion by zero
			if ((*this)[k][k] == 0) 
			{
				throw std::overflow_error("Division by zero exception");
		    }
		
		    //Make diagonal element (support_element) 1
		    support_element = (*this)[k][k];
		    for (j = 0; j < size; j++) 
			{
				(*this)[k][j] /= support_element;
		        inverse[k][j] /= support_element;
		    }
		
		    //Set zero under main diagonal
		    for (i = k + 1; i < size; i++) 
			{
		        support_element = (*this)[i][k];
		        for (j = 0; j < size; j++) 
				{
					(*this)[i][j] -= support_element * (*this)[k][j];
		            inverse[i][j] -= support_element * inverse[k][j];
		        }
		    }
		}
		
		//Set zero above main diagonal: "turn matrix upside down"
		for (k = size - 1; k >= 0; k--) 
		{
		    for (i = k - 1; i >= 0; i--) 
			{
		        support_element = (*this)[i][k];
		        for (j = 0; j < size; j++) 
				{
					(*this)[i][j] -= support_element * (*this)[k][j];
		            inverse[i][j] -= support_element * inverse[k][j];
		        }
		    }
		}
		*this = inverse;
		return *this;
	}
}