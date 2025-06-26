#include "../matrix.h"
#include "../vector.h"
#include <iomanip>

void create_2D_double_arr(double**& arr, 
	size_t rows_count, size_t columns_count, 
	double initial_value = 0.0)
{
	int i, j;
	arr = new double* [rows_count];
	for (i = 0; i < rows_count; ++i)
	{
		arr[i] = new double[columns_count];
		for (j = 0; j < columns_count; ++j)
		{
			arr[i][j] = initial_value;
		}
	}
}

matrix::matrix(size_t rows_count, size_t columns_count, double initial_value)
{
	create_2D_double_arr(data, rows_count, columns_count, initial_value);
	_rows_count = rows_count;
	_columns_count = columns_count;
}

void matrix::clear_data(matrix& arg)
{
	for (int i = 0; i < arg._rows_count; ++i)
	{
		delete[] arg.data[i];
	}
	delete[] arg.data;
	arg.data = nullptr;
}

matrix::~matrix()
{
	clear_data(*this);
}

void deep_copy(matrix& arg_1, const matrix& arg_2) 
{
	int i, j;
	arg_1.data = new double* [arg_2._rows_count];
	for (i = 0; i < arg_2._rows_count; ++i)
	{
		arg_1.data[i] = new double[arg_2._columns_count];
		for (j = 0; j < arg_2._columns_count; j++)
		{
			arg_1.data[i][j] = arg_2.data[i][j];
		}
	}
	arg_1._rows_count = arg_2._rows_count;
	arg_1._columns_count = arg_2._columns_count;
}

matrix::matrix(const matrix& arg)
{
	deep_copy(*this, arg);
}

matrix& matrix::operator = (matrix const& arg)
{
	int i, j;

	//check if arg it me
	if (this == &arg)
		return *this;

	//clear old data
	clear_data(*this);

	//set new data
	deep_copy(*this, arg);

	return *this;
}

void matrix::print(std::ostream& os) const
{
	int i, j;
	for (i = 0; i < _rows_count; ++i)
	{
		for (j = 0; j < _columns_count; ++j)
		{
			std::cout << std::setw(2) << data[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& os, const matrix& m)
{
	m.print(os);
	return os;
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

	// create temporary array
	double** res = nullptr;
	create_2D_double_arr(res, this->_rows_count, arg._columns_count);

	// calculate data in temp arr
	if (this->_columns_count == arg._rows_count)
	{
		for (i = 0; i < this->_rows_count; ++i)
		{
			for (j = 0; j < arg._columns_count; ++j)
			{
				for (k = 0; k < this->_columns_count; ++k)
				{
					res[i][j] += this->data[i][k] * arg.data[k][j];
				}
			}
		}
		clear_data(*this);
		this->data = res;
		this->_columns_count = arg._columns_count;
	}
	else
	{
		throw std::invalid_argument("Matrix with different size in '+='");
	}
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
	return copy *= arg;
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

void set_identity_matrix(matrix& arg, int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (i == j)
				arg[i][j] = 1.0;
			else
				arg[i][j] = 0.0;

		}
	}
}

void find_triangular_matrix(matrix& A, matrix& L, matrix& U, int size)
{
	int i, j, k;
	set_identity_matrix(L, size);

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

matrix matrix::transp()
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
	return res_matrix;
}

matrix matrix::reverse()
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
		set_identity_matrix(inverse, size);
		
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
		return inverse;
	}
}

void swap_double(double* a, double* b)
{
	if (!a || !b) return;

	double temp = *a;
	*a = *b;
	*b = temp;
}

matrix solve_gauss(matrix& a, matrix& b)
{
	if (a._rows_count != a._columns_count 
		|| a._rows_count != b._rows_count 
		|| b._columns_count != 1) {
		throw std::invalid_argument("Invalid matrix dimensions for Gauss method");
	}

	int n = a._rows_count;
	matrix ab(n, n + 1);

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			ab[i][j] = a[i][j];
		}
		ab[i][n] = b[i][0];
	}

	for (int curr_row = 0; curr_row < n; ++curr_row)
	{
		// Find main element in column
		int max_row = curr_row;
		for (int i = curr_row + 1; i < n; ++i)
		{
			if (abs(ab[i][curr_row]) > abs(ab[max_row][curr_row]))
			{
				max_row = i;
			}
		}

		// change rows
		if (max_row != curr_row)
		{
			for (int j = curr_row; j <= n; ++j)
			{
				swap_double(&ab[curr_row][j], &ab[max_row][j]);
			}
		}

		if (abs(ab[curr_row][curr_row]) < 1e-10)
		{
			throw std::runtime_error("Matrix is singular or nearly singular");
		}

		// Normalize current row
		double pivot = ab[curr_row][curr_row];
		for (int j = curr_row; j <= n; ++j)
		{
			ab[curr_row][j] /= pivot;
		}

		// Excluding a variable from other strings
		for (int i = 0; i < n; ++i)
		{
			if (i != curr_row)
			{
				double factor = ab[i][curr_row];
				for (int j = curr_row; j <= n; ++j)
				{
					ab[i][j] -= factor * ab[curr_row][j];
				}
			}
		}
	}

	matrix x(n, 1);
	for (int i = n - 1; i >= 0; --i)
	{
		x[i][0] = ab[i][n];
		for (int j = i + 1; j < n; ++j)
		{
			x[i][0] -= ab[i][j] * x[j][0];
		}
	}

	return x;
}

int matrix::rank() const
{
    matrix temp(*this);
    int rank = std::min(_rows_count, _columns_count);
    
    for (int row = 0; row < rank; row++) {
        if (temp[row][row] == 0) {
            bool found = false;
            
            // find row with non-zero element and swap rows
            for (int i = row + 1; i < _rows_count; i++) {
                if (temp[i][row] != 0) {
                    for (int j = 0; j < _columns_count; j++) {
                        swap_double(&temp[row][j], &temp[i][j]);
                    }
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                rank--;
                if (row < rank) {
                    // swap columns
                    for (int i = 0; i < _rows_count; i++) {
                        swap_double(&temp[i][row], &temp[i][rank]);
                    }
                }
                row--;
                continue;
            }
        }

        // triangular matrix
        for (int i = row + 1; i < _rows_count; i++) {
            double factor = temp[i][row] / temp[row][row];
            for (int j = row; j < _columns_count; j++) {
                temp[i][j] -= factor * temp[row][j];
            }
        }
    }
    
    return rank;
}

int size_of_linear_shell(cvector** vectors, int count)
{
	if (count == 0 || vectors == nullptr) return 0;

	// check size of all vectors
	int dimension = vectors[0]->size();
	for (int i = 1; i < count; ++i)
	{
		if (vectors[i]->size() != dimension)
		{
			throw std::invalid_argument("All vectors must have the same dimension");
		}
	}

	// create matrix of vectors
	matrix mat(count, dimension);
	for (int i = 0; i < count; ++i)
	{
		for (int j = 0; j < dimension; ++j)
		{
			mat[i][j] = (*vectors[i])[j];
		}
	}

	// find triangular matrix
	matrix L(count, count);
	matrix U(count, dimension);
	find_triangular_matrix(mat, L, U, count);

	// calculate size of linear shell
	int rank = 0;
	const double epsilon = 1e-10;
	for (int i = 0; i < count; ++i)
	{
		bool non_zero_row = false;
		for (int j = 0; j < dimension; ++j)
		{
			if (std::abs(U[i][j]) > epsilon)
			{
				non_zero_row = true;
				break;
			}
		}
		if (non_zero_row)
		{
			rank++;
		}
		else
		{
			break;
		}
	}

	return rank;
}

bool is_vector_in_span(const cvector& vec, cvector** basis_vectors, int basis_count)
{
	if (basis_count == 0)
	{
		return vec.empty();
	}

	int dimension = basis_vectors[0]->size();
	if (vec.size() != dimension)
	{
		return false;
	}

	matrix a(basis_count, dimension);
	matrix b(basis_count, 1);

	// create matrix a from vectors
	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < basis_count; ++j)
		{
			a[j][i] = (*basis_vectors[i])[j];
		}
	}

	// create matrix b from discovering vector
	for (int i = 0; i < dimension; ++i)
	{
		b[i][0] = vec[i];
	}

	try {
		matrix x = solve_gauss(a, b);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}