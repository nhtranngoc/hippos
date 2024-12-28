#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#if defined(__is_libk)
#include <kernel/serial.h>
#endif

#define SERIAL 0

static bool print(uint8_t output, const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++){
		#if defined(__is_libk)
		if(output == SERIAL) {
			char cbuf[2];
			cbuf[0] = bytes[i];
			cbuf[1] = '\0';
			serial_write(cbuf, SERIAL_COM1);
		}
		#endif
		if(output != SERIAL) {
			if (putchar(bytes[i]) == EOF)
				return false;
		}
	}
	return true;
}


// For now, printf is including an output pipe, 0 for serial, other wise to terminal. 
// @TODO: Will need to change this to something more elegant.
int printf(uint8_t output, const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(output, format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(output, &c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(output, str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			int num = va_arg(parameters, int);
			char buf[32];
			itoa(num, buf, 10);
			size_t len = strlen(buf);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(output, buf, len))
				return -1;
			written += len;
		} else if (*format == 'x') {
			format++;
			int64_t num = va_arg(parameters, int64_t);
			char buf[32];
			itoa(num, buf, 16);
			size_t len = strlen(buf);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(output, buf, len))
				return -1;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(output, format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
