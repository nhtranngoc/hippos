#include <stdio.h>

int puts(const char* string) {
	return printf(1, "%s\n", string);
}
