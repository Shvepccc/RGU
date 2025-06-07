#include "../libs/bf_complex.h"

int program_5_main(int argc, char* argv[])
{
    bf_complex z1(bigfloat(1, 2), bigfloat(3, 4)); // 1/2 + 3/4i
    bf_complex z2(2.0, 1.0, -1.0, 2.0);            // 2 + (-1/2)i

    std::cout << "z1 = " << z1 << "\n";
    std::cout << "z2 = " << z2 << "\n\n";

    std::cout << "z1 + z2 = " << (z1 + z2) << "\n";
    std::cout << "z1 - z2 = " << (z1 - z2) << "\n";
    std::cout << "z1 * z2 = " << (z1 * z2) << "\n";
    std::cout << "z1 / z2 = " << (z1 / z2) << "\n\n";

    std::cout << "mode(z1) = " << z1.get_mod() << "\n";
    //std::cout << "arg(z1) = " << z1.get_arg(bigfloat(1, 1e3)) << std::endl;

    bf_complex read_test;
    std::cout << "Enter complex value: ";
    std::cin >> read_test;
    std::cout << "Reasult of read coplex value: " << read_test << "\n";

    return 0;
}