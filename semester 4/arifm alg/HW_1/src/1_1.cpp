#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

const double EPS = 1e-10;

void printPolynomial(const std::vector<double>& coeffs, double a = 0)
{
    int n = coeffs.size();
    if (n == 0)
    {
        std::cout << "0" << std::endl;
        return;
    }
    
    bool first = true;
    
    for (int i = 0; i < n; ++i)
    {
        double coef = coeffs[i];
        if (fabs(coef) < EPS) continue;
        
        if (!first)
        {
            std::cout << (coef > 0 ? " + " : " - ");
        }
        else if (coef < 0)
        {
            std::cout << "-";
        }
        
        double absCoef = fabs(coef);
        
        if (i == 0)
        {
            std::cout << absCoef;
        } 
        else
        {
            if (fabs(absCoef - 1) > EPS) std::cout << absCoef;
            
            if (a != 0)
            {
                std::cout << "(x " << (a >= 0 ? "- " : "+ ") << fabs(a) << ")";
                if (i > 1) std::cout << "^" << i;
            }
            else
            {
                std::cout << "x";
                if (i > 1) std::cout << "^" << i;
            }
        }
        
        first = false;
    }
    
    if (first) std::cout << "0";
    std::cout << std::endl;
}

bool readPolynomialsFromFile(const std::string& filename, 
                            std::vector<double>& f,
                            std::vector<std::vector<double>>& g)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    bool isFirstLine = true;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '/') continue;
        
        std::stringstream ss(line);
        double value;
        std::vector<double> row;
        
        while (ss >> value) {
            row.push_back(value);
        }
        
        if (isFirstLine) {
            f = row;
            isFirstLine = false;
        } else {
            g.push_back(row);
        }
    }
    
    file.close();
    return true;
}

void printMatrix(const std::vector<std::vector<double>>& matrix)
{
    for (const auto& row : matrix)
    {
        for (double val : row)
        {
            std::cout << std::setw(10) << std::setprecision(4) << val << " ";
        }
        std::cout << std::endl;
    }
}

bool gaussianElimination(std::vector<std::vector<double>>& a, std::vector<double>& solution)
{
    int n = a.size();
    int m = a[0].size() - 1;
    
    std::vector<int> where(m, -1);
    int row = 0;
    
    for (int col = 0; col < m && row < n; col++)
    {
        int sel = row;
        for (int i = row; i < n; i++)
        {
            if (fabs(a[i][col]) > fabs(a[sel][col]))
                sel = i;
        }
        
        if (fabs(a[sel][col]) < EPS)
            continue;
        
        for (int j = col; j <= m; j++)
            std::swap(a[sel][j], a[row][j]);
        
        where[col] = row;
        
        for (int j = m; j >= col; j--)
            a[row][j] /= a[row][col];
        
        for (int i = 0; i < n; i++)
        {
            if (i != row && fabs(a[i][col]) > EPS)
            {
                double factor = a[i][col];
                for (int j = col; j <= m; j++)
                    a[i][j] -= factor * a[row][j];
            }
        }
        
        row++;
    }
    
    for (int i = 0; i < n; i++)
    {
        bool allZero = true;
        for (int j = 0; j < m; j++)
        {
            if (fabs(a[i][j]) > EPS)
            {
                allZero = false;
                break;
            }
        }
        if (allZero && fabs(a[i][m]) > EPS)
            return false;
    }
    
    solution.assign(m, 0.0);
    for (int j = 0; j < m; j++)
    {
        if (where[j] != -1)
            solution[j] = a[where[j]][m];
    }
    
    return true;
}

int main(int argc, char* argv[])
{
    std::string filename;
    
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        filename = "task_1.txt";
    }
    
    std::vector<double> f;
    std::vector<std::vector<double>> g;

    if (!readPolynomialsFromFile(filename, f, g))
    {
        std::cout << "Error: can not read file " << filename << std::endl;
        return 1;
    }

    std::cout << "f(x): ";
    printPolynomial(f);
    
    std::cout << "\ng_i(x):" << std::endl;
    for (size_t i = 0; i < g.size(); ++i)
    {
        std::cout << "g_" << i+1 << "(x) = ";
        printPolynomial(g[i]);
    }

    int max_degree = f.size();
    for (const auto& poly : g)
    {
        max_degree = std::max(max_degree, (int)poly.size());
    }

    std::vector<std::vector<double>> matrix(max_degree, std::vector<double>(g.size() + 1, 0));
    
    for (int k = 0; k < g.size(); ++k)
    {
        for (int i = 0; i < g[k].size(); ++i)
        {
            if (i < max_degree)
            {
                matrix[i][k] = g[k][i];
            }
        }
    }
    
    for (int i = 0; i < f.size(); ++i)
    {
        if (i < max_degree)
        {
            matrix[i][g.size()] = f[i];
        }
    }

    std::cout << "\nMatrix system:" << std::endl;
    printMatrix(matrix);

    std::vector<double> solution;
    bool isConsistent = gaussianElimination(matrix, solution);

    std::cout << "\nResult:" << std::endl;
    if (isConsistent)
    {
        std::cout << "Polynomial f(x) belongs to the linear span." << std::endl;
        std::cout << "\nRepresentation f(x) with A_i:" << std::endl;
        for (size_t i = 0; i < solution.size(); ++i)
        {
            if (fabs(solution[i]) > EPS)
            {
                std::cout << "A_" << i+1 << " = " << solution[i] << std::endl;
            }
        }
        
        std::cout << "\nf(x) = ";
        bool first = true;
        for (size_t i = 0; i < solution.size(); ++i)
        {
            if (fabs(solution[i]) < EPS) continue;
            
            if (!first)
            {
                std::cout << " + ";
            }
            std::cout << solution[i] << " * g" << i+1 << "(x)";
            first = false;
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Polynomial f(x) does NOT belong to the linear span ";
        std::cout << "(system is inconsistent)." << std::endl;
    }
    
    return 0;
}