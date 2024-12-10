#include <stdio.h>

int puts(const char* string) {
	return printf(PIPE_TERMINAL, "%s\n", string);
}
