// make run TASK=2_3 ARGS="8"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "../include/help.hpp"

int flipCoin()
{
    return getRandomNumber(1, 2);
}

int runExperiment()
{
    int prev = flipCoin();
    int current = flipCoin();
    int tossCount = 2;
    
    while (prev != current)
    {
        prev = current;
        current = flipCoin();
        tossCount++;
    }
    
    return tossCount;
}

double analyticProbabilityBeforeK(int k)
{
    if (k <= 2) return 0.0;
    return 1.0 - pow(0.5, k - 2);
}

double analyticProbabilityEven()
{
    double firstTerm = 0.5; 
    double ratio = 0.25;
    
    double sum = firstTerm / (1.0 - ratio);
    
    return sum;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Ошибка: Необходимо указать параметр k" << std::endl;
        std::cerr << "Использование: " << argv[0] << " <k>" << std::endl;
        return 1;
    }
    
    int k = 0;
    try
    {
        k = std::stoi(argv[1]);
    }
    catch (const std::exception&)
    {
        std::cerr << "Ошибка: k должно быть целым числом" << std::endl;
        return 1;
    }
    
    if (k < 2)
    {
        std::cerr << "Ошибка: k должно быть >= 2" << std::endl;
        return 1;
    }
    
    std::cout << "\n1. Пространство элементарных событий:\n" << std::endl;

    std::string res_1 = "0";
    std::string res_2 = "1";
    
    for (int len = 2; len <= k; len++)
    {
        std::cout << res_1 + res_1.back() << std::endl;
        std::cout << res_2 + res_2.back() << std::endl;
        
        res_1 += (res_1.back() == '0') ? '1' : '0';
        res_2 += (res_2.back() == '1') ? '0' : '1';
    }
    
    std::cout << "\n2. Проведение экспериментов (k = " << k << ", N = " << NUM_EXPERIMENTS << " )" << std::endl;
    
    int countBeforeK = 0;
    int countEven = 0;
    
    for (int i = 0; i < NUM_EXPERIMENTS; i++)
    {
        int tosses = runExperiment();
        
        if (tosses < k) countBeforeK++;
        if (tosses % 2 == 0) countEven++;
    }
    
    double empiricalBeforeK = static_cast<double>(countBeforeK) / NUM_EXPERIMENTS;
    double empiricalEven = static_cast<double>(countEven) / NUM_EXPERIMENTS;
    
    double analyticBeforeK = analyticProbabilityBeforeK(k);
    double analyticEven = analyticProbabilityEven();
    
    std::cout << "\n3. Результаты:" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    
    std::cout << "\na)Вероятность окончания до " << k << "-го бросания:" << std::endl;
    std::cout << "  Эмпирическая:  " << empiricalBeforeK << std::endl;
    std::cout << "  Аналитическая: " << analyticBeforeK << std::endl;
    std::cout << "  Разница:       " << std::abs(empiricalBeforeK - analyticBeforeK) << std::endl;
    
    std::cout << "\nб) Вероятность окончания с четным числом бросков:" << std::endl;
    std::cout << "  Эмпирическая:  " << empiricalEven << std::endl;
    std::cout << "  Аналитическая: " << analyticEven << std::endl;
    std::cout << "  Разница:       " << std::abs(empiricalEven - analyticEven) << std::endl;
    
    return 0;
}