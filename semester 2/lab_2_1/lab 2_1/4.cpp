#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include "../libs_1/matrix.h"



int program_4_main(int argc, char* argv[])
{	
	try
	{
		matrix a(5, 5, 1);
		matrix b(5, 5, 2);
		matrix c(5, 5, 4);
		std::cout << "Matrix a: \n";
		a.print();
		std::cout << "Matrix b: \n";
		b.print();
		a += b;
		std::cout << "Matrix a += b: \n";
		a.print();
		a = c;
		std::cout << "Matrix a = c: \n";
		a.print();
		a -= b;
		std::cout << "Matrix a -= b: \n";
		a.print();
		a *= c;
		std::cout << "Matrix a *= c: \n";
		a.print();
		std::cout << "Matrix a = 2 * b: \n";
		a = 2 * b;
		a.print();

		std::cout << "Matrix a with chaged element by [0][0]: \n";
		a[0][0] = 888;
		a.print();
		
		std::cout << "a[0][0] = " << a[0][0] << "\n\n";

		matrix h(3, 3, 1);
		h[0][0] = 9;
		h[1][1] = 9;
		h[2][2] = 9;
		std::cout << "Matrix h: \n";
		h.print();
		std::cout << "Determinant of h = " << h.det() << "\n";


		std::cout << "Transponanted matrix a: " << "\n";
		a[3][2] = 8;
		a.print();
		a = a.transp();
		a.print();

		std::cout << "Reversed matrix h: " << "\n";
		h = h.reverse();
		h.print();
	}
	catch (const std::exception& exc)
	{
		std::cout << "Oh! You have some error... x_x : " << 
			exc.what() << std::endl;
	}

	_CrtDumpMemoryLeaks();

	return 0;
}