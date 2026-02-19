#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <string>

// Перечисления для правил индексирования (имена в snake_case)
enum class bit_indexing_order
{
    lsb0,   // младший бит имеет индекс 0
    msb0    // старший бит имеет индекс 0
};

enum class bit_index_base
{
    base0,  // индексы начинаются с 0
    base1   // индексы начинаются с 1
};

// Класс, содержащий метод перестановки битов
class bit_permutation
{
public:
    // Выполняет перестановку битов в массиве байтов согласно правилу
    // Параметры:
    //   data          - входной массив байтов
    //   rule          - правило перестановки (массив индексов битов)
    //   order         - порядок индексирования битов внутри байта
    //   base          - начало отсчёта индексов (0 или 1)
    // Возвращает новый массив байтов с переставленными битами
    std::vector<uint8_t> permute(
        const std::vector<uint8_t>& data,
        const std::vector<int>& rule,
        bit_indexing_order order,
        bit_index_base base) const
    {
        // Количество битов во входных данных
        size_t total_bits = data.size() * 8;

        // Проверка: размер правила должен совпадать с количеством битов
        if (rule.size() != total_bits)
        {
            throw std::invalid_argument(
                "Rule size must equal number of bits in data");
        }

        // Если данных нет, возвращаем пустой результат
        if (data.empty())
        {
            return {};
        }

        // 1. Преобразуем входные байты в линейный массив битов
        //    в соответствии с выбранным порядком индексирования
        std::vector<uint8_t> input_bits(total_bits);
        for (size_t i = 0; i < total_bits; ++i)
        {
            size_t byte_idx = i / 8;
            size_t bit_in_byte;
            if (order == bit_indexing_order::lsb0)
            {
                bit_in_byte = i % 8;               // lsb0: бит 0 - младший
            }
            else
            {
                bit_in_byte = 7 - (i % 8);          // msb0: бит 0 - старший
            }
            input_bits[i] = (data[byte_idx] >> bit_in_byte) & 1;
        }

        // 2. Применяем правило перестановки
        std::vector<uint8_t> output_bits(total_bits);
        for (size_t j = 0; j < total_bits; ++j)
        {
            int src_index = rule[j];

            // Коррекция начала отсчёта
            if (base == bit_index_base::base1)
            {
                if (src_index < 1)
                {
                    throw std::out_of_range(
                        "Index in rule must be >= 1 when base = 1");
                }
                --src_index; // теперь 0-индексированный
            }

            // Проверка границ
            if (src_index < 0 || static_cast<size_t>(src_index) >= total_bits)
            {
                throw std::out_of_range("Index in rule is out of range");
            }

            output_bits[j] = input_bits[static_cast<size_t>(src_index)];
        }

        // 3. Упаковываем выходные биты обратно в байты
        std::vector<uint8_t> result(data.size(), 0);
        for (size_t j = 0; j < total_bits; ++j)
        {
            if (output_bits[j] == 0) continue;

            size_t byte_idx = j / 8;
            size_t bit_in_byte;
            if (order == bit_indexing_order::lsb0)
            {
                bit_in_byte = j % 8;
            }
            else
            {
                bit_in_byte = 7 - (j % 8);
            }
            result[byte_idx] |= (1 << bit_in_byte);
        }

        return result;
    }
};

// Вспомогательная функция для печати массива байтов в шестнадцатеричном виде
std::string bytes_to_hex(const std::vector<uint8_t>& bytes)
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (uint8_t b : bytes)
    {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

// Вспомогательная функция для печати массива байтов в двоичном виде
std::string bytes_to_binary(const std::vector<uint8_t>& bytes)
{
    std::ostringstream oss;
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        if (i > 0) oss << ' ';
        for (int b = 7; b >= 0; --b)
        {
            oss << ((bytes[i] >> b) & 1);
        }
    }
    return oss.str();
}

// Структура для описания тестового случая
struct test_case
{
    std::vector<uint8_t> data;
    std::vector<int> rule;
    bit_indexing_order order;
    bit_index_base base;
    std::vector<uint8_t> expected_result;  // пустой вектор, если ожидается исключение
    bool expect_exception;
    std::string description;
};

// Функция для выполнения модульных тестов
void run_unit_tests()
{
    bit_permutation perm;

    std::vector<test_case> tests =
    {
        // Успешные тесты
        {
            {0x01},
            {0,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {0x01},
            false,
            "LSB0, Base0, тождественная перестановка"
        },
        {
            {0x01},
            {7,6,5,4,3,2,1,0},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {0x80},
            false,
            "LSB0, Base0, реверс битов"
        },
        {
            {0x01},
            {0,1,2,3,4,5,6,7},
            bit_indexing_order::msb0,
            bit_index_base::base0,
            {0x01},
            false,
            "MSB0, Base0, тождественная перестановка"
        },
        {
            {0x01},
            {7,6,5,4,3,2,1,0},
            bit_indexing_order::msb0,
            bit_index_base::base0,
            {0x80},
            false,
            "MSB0, Base0, реверс битов"
        },
        {
            {0x01},
            {1,2,3,4,5,6,7,8},
            bit_indexing_order::lsb0,
            bit_index_base::base1,
            {0x01},
            false,
            "LSB0, Base1, тождественная (индексы 1..8)"
        },
        {
            {0x12, 0x34},
            {8,9,10,11,12,13,14,15, 0,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {0x34, 0x12},
            false,
            "LSB0, Base0, обмен байтов местами"
        },
        {
            {},  // пустые данные
            {},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {},
            false,
            "Пустые данные"
        },
        // Тесты с ожидаемыми исключениями
        {
            {0x01, 0x02},
            {0,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {},
            true,
            "Ошибка: размер правила не совпадает (нужно 16, дано 8)"
        },
        {
            {0x01},
            {8,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {},
            true,
            "Ошибка: индекс вне допустимого диапазона (8 > 7)"
        },
        {
            {0x01},
            {0,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base1,
            {},
            true,
            "Ошибка: base=1, но индекс 0 присутствует"
        }
    };

    std::cout << "\n========================================\n";
    std::cout << "ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ\n";
    std::cout << "========================================\n";

    size_t passed = 0;
    size_t failed = 0;

    for (size_t i = 0; i < tests.size(); ++i)
    {
        const auto& test = tests[i];
        std::cout << "\nТест " << i + 1 << ": " << test.description << "\n";
        std::cout << "Входные данные (hex): 0x" << bytes_to_hex(test.data) << "\n";
        std::cout << "Входные данные (bin): " << bytes_to_binary(test.data) << "\n";

        try
        {
            std::vector<uint8_t> result = perm.permute(
                test.data, test.rule, test.order, test.base);

            if (test.expect_exception)
            {
                std::cout << "!!! ОШИБКА: ожидалось исключение, но его не было\n";
                ++failed;
            }
            else
            {
                // Сравниваем результат с ожидаемым
                if (result == test.expected_result)
                {
                    std::cout << "РЕЗУЛЬТАТ: OK\n";
                    std::cout << "Выходные данные (hex): 0x" << bytes_to_hex(result) << "\n";
                    std::cout << "Выходные данные (bin): " << bytes_to_binary(result) << "\n";
                    ++passed;
                }
                else
                {
                    std::cout << "!!! ОШИБКА: результат не совпадает с ожидаемым\n";
                    std::cout << "Ожидалось (hex): 0x" << bytes_to_hex(test.expected_result) << "\n";
                    std::cout << "Получено  (hex): 0x" << bytes_to_hex(result) << "\n";
                    std::cout << "Ожидалось (bin): " << bytes_to_binary(test.expected_result) << "\n";
                    std::cout << "Получено  (bin): " << bytes_to_binary(result) << "\n";
                    ++failed;
                }
            }
        }
        catch (const std::exception& ex)
        {
            if (test.expect_exception)
            {
                std::cout << "РЕЗУЛЬТАТ: OK (поймано ожидаемое исключение: " << ex.what() << ")\n";
                ++passed;
            }
            else
            {
                std::cout << "!!! ОШИБКА: неожиданное исключение: " << ex.what() << "\n";
                ++failed;
            }
        }
    }

    std::cout << "\n========================================\n";
    std::cout << "ИТОГ: passed = " << passed << ", failed = " << failed << "\n";
    std::cout << "========================================\n";
}

int main()
{
    try
    {
        run_unit_tests();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Критическая ошибка: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}