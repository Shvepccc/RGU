#include <stdio.h>
#include <stdarg.h>

int main() {

}

int find_Kaprekara(int base, ...) {
	char* ptr;
	va_list args;
	va_start(args, base);

	while ((va_arg(args, char*)) != NULL) {
		
	}
	return 0;
}
