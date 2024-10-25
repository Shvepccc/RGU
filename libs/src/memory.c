#include <stdio.h>
#include <stdlib.h>

int cust_realloc(void** ptr, size_t size) {
	void* ptr2 = NULL;

	ptr2 = (void*)realloc(*ptr, size);
	if (ptr2 == NULL) {
		return 1;
	}
	*ptr = ptr2;
	return 0;
}