// make run TASK=2_2 ARGS="4"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>

bool get_number(const char* str, int& num)
{
    char* end;
    long val = std::strtol(str, &end, 10);
    if (*end != '\0' || val <= 0) return false;
    num = (int)val;
    return true;
}

void print_perm(const std::vector<int>& p)
{
    for (size_t i = 0; i < p.size(); i++)
    {
        std::cout << p[i];
        if (i < p.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Ошибка! Использование: " << argv[0] << " N" << std::endl;
        return 1;
    }
    
    int N;
    if (!get_number(argv[1], N) || N < 2)
    {
        std::cout << "Ошибка! N должно быть целым числом >= 2" << std::endl;
        return 1;
    }
    
    const int i = 1;
    const int j = 2;
    
    std::cout << "Проверка теоремы сложения вероятностей" << std::endl;
    std::cout << "N = " << N << ", i = " << i << ", j = " << j << std::endl;
    
    double p_i_analytic = 1.0 / N;
    double p_j_analytic = 1.0 / N;
    double p_ij_analytic = 1.0 / (N * (N - 1));
    double p_union_analytic = p_i_analytic + p_j_analytic - p_ij_analytic;
    
    std::cout << "1. Аналитический расчёт:" << std::endl;
    std::cout << "   P(A" << i << ") = 1/" << N << " = " << p_i_analytic << std::endl;
    std::cout << "   P(A" << j << ") = 1/" << N << " = " << p_j_analytic << std::endl;
    std::cout << "   P(A" << i << " * A" << j << ") = 1/(" << N << "*" << N-1 << ") = " << p_ij_analytic << std::endl;
    std::cout << "   P(A" << i << " + A" << j << ") = " << p_i_analytic << " + " << p_j_analytic << " - " << p_ij_analytic 
        << " = " << p_union_analytic << "\n" << std::endl;
    
    std::vector<int> perm(N);
    for (int k = 0; k < N; k++) perm[k] = k + 1;
    
    int count_i = 0, count_j = 0, count_ij = 0, count_union = 0;
    int total = 0;
    
    std::vector<std::vector<int>> all_union, all_ij;
    
    std::cout << "2. Эмпирический расчёт:" << std::endl;
    
    do
    {
        total++;
        
        bool in_i = (perm[i-1] == i);
        bool in_j = (perm[j-1] == j);
        
        if (in_i) count_i++;
        if (in_j) count_j++;
        if (in_i && in_j)
        {
            count_ij++;
            all_ij.push_back(perm);
        }
        if (in_i || in_j)
        {
            count_union++;
            all_union.push_back(perm);
        }
        
    } while (std::next_permutation(perm.begin(), perm.end()));
    
    double p_i_exp = (double)count_i / total;
    double p_j_exp = (double)count_j / total;
    double p_ij_exp = (double)count_ij / total;
    double p_union_exp = (double)count_union / total;
    
    std::cout << "   Всего перестановок: " << total << std::endl;
    std::cout << "   P(A" << i << ") = " << count_i << "/" << total << " = " << p_i_exp << std::endl;
    std::cout << "   P(A" << j << ") = " << count_j << "/" << total << " = " << p_j_exp << std::endl;
    std::cout << "   P(A" << i << " * A" << j << ") = " << count_ij << "/" << total << " = " << p_ij_exp << std::endl;
    std::cout << "   P(A" << i << " + A" << j << ") = " << count_union << "/" << total << " = " << p_union_exp << "\n" << std::endl;
    
    std::cout << "3. Сравнение результатов:" << std::endl;
    std::cout << "   Разница P(A" << i << "): " << std::abs(p_i_analytic - p_i_exp) << std::endl;
    std::cout << "   Разница P(A" << j << "): " << std::abs(p_j_analytic - p_j_exp) << std::endl;
    std::cout << "   Разница P(A" << i << "*A" << j << "): " << std::abs(p_ij_analytic - p_ij_exp) << std::endl;
    std::cout << "   Разница P(A" << i << "+A" << j << "): " << std::abs(p_union_analytic - p_union_exp) << std::endl;
    std::cout << std::endl;
    
    std::cout << "4. Проверка теоремы сложения:" << std::endl;
    std::cout << "   Формула: P(A+B) = P(A) + P(B) - P(A*B)" << std::endl;
    std::cout << std::endl;
    
    double left_emp = p_union_exp;
    double right_emp = p_i_exp + p_j_exp - p_ij_exp;
    double diff_emp = std::abs(left_emp - right_emp);
    
    std::cout << "   По эмпирическим данным:" << std::endl;
    std::cout << "   " << p_union_exp << " = " << p_i_exp << " + " << p_j_exp << " - " << p_ij_exp 
              << " = " << right_emp << std::endl;
    
    if (diff_emp < 0.0000001)
    {
        std::cout << "   Теорема выполняется (разница: " << diff_emp << ")" << std::endl;
    }
    else
    {
        std::cout << "   Теорема НЕ выполняется (разница: " << diff_emp << ")" << std::endl;
    }
    std::cout << std::endl;
    
    double left_analytic = p_union_analytic;
    double right_analytic = p_i_analytic + p_j_analytic - p_ij_analytic;
    double diff_analytic = std::abs(left_analytic - right_analytic);
    
    std::cout << "   По аналитическим формулам:" << std::endl;
    std::cout << "   " << p_union_analytic << " = " << p_i_analytic << " + " << p_j_analytic << " - " << p_ij_analytic 
              << " = " << right_analytic << std::endl;
    
    if (diff_analytic < 0.0000001)
    {
        std::cout << "   Теорема ВЫПОЛНЯЕТСЯ (разница: " << diff_analytic << ")" << std::endl;
    }
    else
    {
        std::cout << "   Теорема НЕ ВЫПОЛНЯЕТСЯ (разница: " << diff_analytic << ")" << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "5. Все благоприятные исходы:" << std::endl;
    
    std::cout << "   Событие A" << i << " + A" << j << std::endl;
    if (all_union.empty())
    {
        std::cout << "   Нет благоприятных исходов" << std::endl;
    }
    else
    {
        std::cout << "   Всего " << count_union << " исходов:" << std::endl;
        for (size_t k = 0; k < all_union.size(); k++)
        {
            std::cout << "     ";
            print_perm(all_union[k]);
        }
    }
    std::cout << std::endl;
    
    std::cout << "   Событие A" << i << " * A" << j << std::endl;
    if (all_ij.empty())
    {
        std::cout << "   Нет благоприятных исходов" << std::endl;
    }
    else
    {
        std::cout << "   Всего " << count_ij << " исходов:" << std::endl;
        for (size_t k = 0; k < all_ij.size(); k++)
        {
            std::cout << "     ";
            print_perm(all_ij[k]);
        }
    }
    
    return 0;
}