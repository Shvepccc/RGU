#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>
#include "../../libs/error.h"
#include <math.h>


int convert_TO_decimal(char* n, int base, int* ans) {
	int len = 0, j = 0, sign = 0;

	if (n == NULL || ans == NULL) {
		return NULL_POINTER;
	}

	if (base < 2 || base > 36) {
		return INCORRECT_BASE;
	}
	if (n[0] == '0') {
		*ans = 0;
		return 0;
	}

	len = strlen(n);

	if (n[0] == '-') { sign = 1; }

	for (int i = sign; i < len; i++) {
		if (isdigit(n[i])) {
			if ((int)((int)n[i] - '0') > base) { *ans = 0; return INCORRECT_ARG; }
			(*ans) += (int)((int)n[i] - '0') * pow((double)base, len - i - 1);
		}
		else if (isalpha(n[i])) {
			if ((int)((int)toupper(n[i]) - 'A' + 10) > base) { *ans = 0; return INCORRECT_ARG; }
			(*ans) += (int)((int)toupper(n[i]) - 'A' + 10) * pow((double)base, len - i - 1);
		}
	}
	if (sign == 1) { (*ans) *= -1; }
	return 0;
}

int convert_FROM_decimal(int n, int base, char** ans) {
	char* res;
	int len = 0, j = 0, sign = 0;

	if (ans == NULL) {
		return NULL_POINTER;
	}
	if (base < 2 || base > 36) {
		return INCORRECT_BASE;
	}
	if (n == 0) {
		*ans = (char*)malloc(2 * sizeof(char));
		if (*ans == NULL) { return MEMORY_ALLOCATE_ERROR; }
		(*ans)[0] = '0';
		(*ans)[1] = '\0';
		return 0;
	}

	len = (log(abs(n)) / log(base)) + 1;

	if (n < 0) { sign = 1; len++; n = -n; }

	*ans = (char*)malloc((len + 1) * sizeof(char));
	if (*ans == NULL) { return MEMORY_ALLOCATE_ERROR; }
	res = *ans;

	if (sign) { *res = '-'; res++; }

	res += len - sign;
	*res-- = 0;
	
	while (n > 0) {
		j = (int)n % base;
		if (j < 10) {
			*res = j + '0';
		}
		else {
			*res = j + 'a' - 10;
		}

		n /= base;
		res--;
	}

	return 0;
}