#include "../include/test_feistel.hpp"
#include "../include/test_des_algorithm.hpp"

int main()
{
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     ТЕСТИРОВАНИЕ КЛАССА FEISTEL_NETWORK                    ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    
    bool all_passed = run_feistel_network_tests();

    bool tests_ok = run_des_algorithm_tests();
    
    if (!tests_ok)
    {
        std::cerr << "Внимание: некоторые тесты DES не пройдены!" << std::endl;
    }
    
    std::cout << "\n";
    if (all_passed && tests_ok)
    {
        std::cout << "✅ ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!\n";
    }
    else
    {
        std::cout << "❌ НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ.\n";
    }
    std::cout << "\n";
    
    return all_passed ? 0 : 1;
}