#include "utils.h"

/** ut_memory_copy
 *  
 *  Copy certain amount of memory from one location to another 
 *  @param source   Pointer to source memory address
 *  @param dest     Pointer to destination memory address
 *  @param nbytes   Number of bytes to copy
*/
void ut_memory_copy(char *source, char *dest, int nbytes) {
    if (source == 0 || dest == 0) {
        return;
    }

    if (nbytes < 0) {
        return;
    }

    for (int i = 0; i < nbytes; i++) {
        dest[i] = source[i];
    }
}
