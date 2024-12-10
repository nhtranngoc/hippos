#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A utility function to reverse a string
void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char *itoa ( int value, char * str, int base ) {
    int i = 0;
    bool isNegative = false;

    // Handle edge case, if 0 we quickly return
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (value < 0 && base == 10) {
        isNegative = true;
        value = -value;
    }

    while(value) {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    reverse(str, i);

    return str;
}