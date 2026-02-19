#include <iostream>
#include <vector>
#include <cstdlib>

std::pair<std::vector<int>, int> short_division(const std::vector<int>& u, int v, int b)
{
    std::vector<int> w(u.size(), 0);
    int r = 0;

    for (int j = u.size() - 1; j >= 0; --j)
    {
        long long t = static_cast<long long>(r) * b + u[j];
        w[j] = static_cast<int>(t / v);
        r = static_cast<int>(t % v);
    }

    return {w, r};
}

void print_result(const std::vector<int>& u, int v, const std::pair<std::vector<int>, int>& result)
{
    for (int i = u.size() - 1; i >= 0; --i)
    {
        std::cout << u[i];
    }
    std::cout << " / " << v << " = ";

    bool leading = true;
    for (int i = result.first.size() - 1; i >= 0; --i)
    {
        if (leading && result.first[i] == 0 && i > 0)
        {
            continue;
        }
        leading = false;
        std::cout << result.first[i];
    }
    std::cout << ", remainder " << result.second << std::endl;
}

int main()
{
    int base = 10;

    std::vector<int> u1 = {1, 2, 3};
    int v1 = 5;
    std::pair<std::vector<int>, int> result1 = short_division(u1, v1, base);
    print_result(u1, v1, result1);

    std::vector<int> u2 = {6, 5, 4};
    int v2 = 7;
    std::pair<std::vector<int>, int> result2 = short_division(u2, v2, base);
    print_result(u2, v2, result2);

    std::vector<int> u3 = {4, 3, 2, 1};
    int v3 = 9;
    std::pair<std::vector<int>, int> result3 = short_division(u3, v3, base);
    print_result(u3, v3, result3);

    std::vector<int> u4 = {9, 9, 9};
    int v4 = 3;
    std::pair<std::vector<int>, int> result4 = short_division(u4, v4, base);
    print_result(u4, v4, result4);

    std::vector<int> u5 = {0, 0, 0, 1};
    int v5 = 2;
    std::pair<std::vector<int>, int> result5 = short_division(u5, v5, base);
    print_result(u5, v5, result5);

    return 0;
}