#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include <stdint.h>

/** outb:
    *  Sends the given data to the given I/O port. Defined in io.s
    *
    *  @param port The I/O port to send the data to
    *  @param val The data to send to the I/O port
*/
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

/** inb:
 *      Read a byte from a given I/O port
 *      @param port The address of the port
 *      @return the read byte
 */
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

#endif /* INCLUDE_IO_H */