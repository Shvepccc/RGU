#include "../include/test_feistel.hpp"
#include "../include/test_des_algorithm.hpp"
#include "../include/test_triple_des.hpp"

int main()
{
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║           ЗАПУСК ВСЕХ ТЕСТОВ                               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    
    bool feistel_passed = run_feistel_network_tests();
    std::cout << "\n";
    
    bool des_passed = run_des_algorithm_tests();
    std::cout << "\n";
    
    bool triple_des_passed = run_triple_des_tests();
    std::cout << "\n";
    
    if (feistel_passed && des_passed && triple_des_passed)
    {
        std::cout << "✅ ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!\n";
        return 0;
    }
    else
    {
        std::cout << "❌ НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛЕНЫ.\n";
        return 1;
    }
}