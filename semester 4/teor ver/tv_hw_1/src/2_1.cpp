// make run TASK=2_1 ARGS="26 10000"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "../include/help.hpp"

bool parse_arguments(int argc, char* argv[], int& N, int& C)
{
    if (argc != 3)
    {
        std::cerr << "ОШИБКА: Неверное количество аргументов." << std::endl;
        std::cerr << "Используйте: <мощность алфавита N> <количество экспериментов C>" << std::endl;
        return false;
    }
    
    try
    {
        N = std::stoi(argv[1]);
        C = std::stoi(argv[2]);
    }
    catch (const std::exception& e)
    {
        std::cerr << "ОШИБКА: Аргументы должны быть целыми числами." << std::endl;
        return false;
    }
    
    if (N <= 1)
    {
        std::cerr << "ОШИБКА: Мощность алфавита N должна быть больше 1." << std::endl;
        return false;
    }
    
    if (C <= 0)
    {
        std::cerr << "ОШИБКА: Количество экспериментов C должно быть положительным." << std::endl;
        return false;
    }
    
    return true;
}

int count_even_numbers(int N)
{
    return N / 2;
}

double analytical_probability_first(int N)
{
    int even_count = count_even_numbers(N);
    return static_cast<double>(even_count) / N;
}

double analytical_probability_second_without_reorder(int N)
{
    int even_count = count_even_numbers(N);
    
    if (even_count == 0) return 0.0;

    double prob = ((N - even_count) * even_count) / (static_cast<double>(N) * (N - 1))
                    + (even_count * (even_count - 1)) / (static_cast<double>(N) * (N - 1));

    return prob;
}

//TODO: it seems bad
// double analytical_probability_second_with_reorder(int N)
// {
//     long long count = 0;
//     for (int second = 1; second <= N; ++second)
//     {
//         if (second % 2 == 0)  
//         {
//             count += (N - second);  // first > second
//         }
//         else
//         {
//             count += (second - 1);  // first < second
//         }
//     }
//     // Общее число исходов: N * (N-1)
//     return static_cast<double>(count) / (N * (N - 1));
// }

double analytical_probability_second_with_reorder(int N)
{
    long long even_count = N / 2;
    long long odd_count = (N + 1) / 2;
    
    long long total = static_cast<long long>(N) * (N - 1);
    long long favorable = even_count * (odd_count - 1) + (odd_count - 1) * even_count;
    
    return static_cast<double>(favorable) / total;
}

double analytical_probability_both_without_reorder(int N)
{
    int even_count = count_even_numbers(N);
    
    if (even_count < 2) return 0.0;
    
    return (static_cast<double>(even_count) / N) * 
           (static_cast<double>(even_count - 1) / (N - 1));
}

double analytical_probability_both_with_reorder(int N)
{
    int even_count = count_even_numbers(N);
    int odd_count = (N + 1) / 2;
    
    if (even_count == 0 || odd_count <= 1) return 0.0;
    
    long long count = static_cast<long long>(even_count) * (odd_count - 1);
    
    return static_cast<double>(count) / (N * (N - 1));
}

void run_experiments(int N, int C, 
                    double& empirical_first, 
                    double& empirical_second_without_reorder,
                    double& empirical_second_with_reorder,
                    double& empirical_both_without_reorder,
                    double& empirical_both_with_reorder)
{
    int count_first = 0;
    int count_second_without_reorder = 0;
    int count_second_with_reorder = 0;
    int count_both_without_reorder = 0;
    int count_both_with_reorder = 0;
    
    for (int exp = 0; exp < C; ++exp)
    {
        { // without reorder
            int first = getRandomNumber(1, N);
            if (first % 2 == 0)
            {
                count_first++;
            }
            
            int second_without_reorder = getRandomNumber(1, N-1);
            if (second_without_reorder >= first)
            {
                second_without_reorder++;
            }
            
            if (second_without_reorder % 2 == 0)
            {
                count_second_without_reorder++;
            }
            if (first % 2 == 0 && second_without_reorder % 2 == 0)
            {
                count_both_without_reorder++;
            }
        }
        
        { // with reorder
            int first = getRandomNumber(1, N);

            std::vector<int> remaining;
            for (int i = 1; i <= N; ++i)
            {
                if (i != first)
                {
                    remaining.push_back(i);
                }
            }
            
            int second_index = getRandomNumber(0, N-2);
            
            if ((second_index + 1) % 2 == 0)
            {
                count_second_with_reorder++;
            }
            if (first % 2 == 0 && (second_index + 1) % 2 == 0)
            {
                count_both_with_reorder++;
            }
        }
    }
    
    empirical_first = static_cast<double>(count_first) / C;
    empirical_second_without_reorder = static_cast<double>(count_second_without_reorder) / C;
    empirical_second_with_reorder = static_cast<double>(count_second_with_reorder) / C;
    empirical_both_without_reorder = static_cast<double>(count_both_without_reorder) / C;
    empirical_both_with_reorder = static_cast<double>(count_both_with_reorder) / C;
}

int main(int argc, char* argv[])
{
    int N, C;
    
    if (!parse_arguments(argc, argv, N, C))
    {
        return 1;
    }
    
    std::cout << "Параметры эксперимента:" << std::endl;
    std::cout << "  Мощность алфавита N = " << N << std::endl;
    std::cout << "  Количество экспериментов C = " << C << std::endl;
    std::cout << std::endl;
    
    double anal_first = analytical_probability_first(N);
    double anal_second_without_reorder = analytical_probability_second_without_reorder(N);
    double anal_second_with_reorder = analytical_probability_second_with_reorder(N);
    double anal_both_without_reorder = analytical_probability_both_without_reorder(N);
    double anal_both_with_reorder = analytical_probability_both_with_reorder(N);
    
    std::cout << "Аналитические вероятности:" << std::endl;
    std::cout << "  а) В первый раз: " << std::fixed << std::setprecision(6) << anal_first << std::endl;
    std::cout << "  б) Во второй раз (без изменения индексов): " << anal_second_without_reorder << std::endl;
    std::cout << "  б) Во второй раз (с изменением индексов): " << anal_second_with_reorder << std::endl;
    std::cout << "  в) Оба раза (без изменения индексов): " << anal_both_without_reorder << std::endl;
    std::cout << "  в) Оба раза (с изменением индексов): " << anal_both_with_reorder << std::endl;
    std::cout << std::endl;
    
    double emp_first, 
        emp_second_without_reorder, 
        emp_second_with_reorder, 
        emp_both_without_reorder, 
        emp_both_with_reorder;
        
    run_experiments(N, C, emp_first, emp_second_without_reorder, emp_second_with_reorder, 
                   emp_both_without_reorder, emp_both_with_reorder);
    
    std::cout << "Эмпирические вероятности (после " << C << " экспериментов):" << std::endl;
    std::cout << "  а) В первый раз: " << std::fixed << std::setprecision(6) << emp_first << std::endl;
    std::cout << "  б) Во второй раз (без изменения индексов): " << emp_second_without_reorder << std::endl;
    std::cout << "  б) Во второй раз (с изменением индексов): " << emp_second_with_reorder << std::endl;
    std::cout << "  в) Оба раза (без изменения индексов): " << emp_both_without_reorder << std::endl;
    std::cout << "  в) Оба раза (с изменением индексов): " << emp_both_with_reorder << std::endl;
    std::cout << std::endl;
    
    std::cout << "Сравнение (аналитическая vs эмпирическая):" << std::endl;
    std::cout << "  а) В первый раз: разница = " << std::abs(anal_first - emp_first) << std::endl;
    std::cout << "  б) Во второй раз (без изменения): разница = " << std::abs(anal_second_without_reorder - emp_second_without_reorder) << std::endl;
    std::cout << "  б) Во второй раз (с изменением): разница = " << std::abs(anal_second_with_reorder - emp_second_with_reorder) << std::endl;
    std::cout << "  в) Оба раза (без изменения): разница = " << std::abs(anal_both_without_reorder - emp_both_without_reorder) << std::endl;
    std::cout << "  в) Оба раза (с изменением): разница = " << std::abs(anal_both_with_reorder - emp_both_with_reorder) << std::endl;
    
    return 0;
}