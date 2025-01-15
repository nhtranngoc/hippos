#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stdbool.h>

/* All I/O ports are calculated relative to the data port. This is because
* all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
* order, but they start at different values.
*/
#define SERIAL_DATA_PORT(base)              base
#define SERIAL_FIFO_COMMAND_PORT(base)      (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)      (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)     (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)       (base + 5)

// Base address of COM ports
enum SERIAL_COM_PORTS {
    SERIAL_COM1 = 0x3F8,
    SERIAL_COM2 = 0x2F8,
    SERIAL_COM3 = 0x3E8,
    SERIAL_COM4 = 0x2E8,
    SERIAL_COM5 = 0x5F8,
    SERIAL_COM6 = 0x4F8,
    SERIAL_COM7 = 0x5F8,
    SERIAL_COM8 = 0x4E8
};

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
* Tells the serial port to expect first the highest 8 bits on the data port,
* then the lowest 8 bits will follow
*/
#define SERIAL_LINE_ENABLE_DLAB             0x80

/**
 * Default line settings for a COM port: 8 bit length, no parity, one stop bit, disabled break control
 */
/* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
* Content: | d | b | prty  | s | dl  |
* Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
*/
#define SERIAL_DEFAULT_LINE_SETUP 0x03

/**
 * Default fifo buffer settings for a COM port:
 * FIFO enabled, clear both receiver and transmission queue, queue size is 14 bytes
 * Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
 * Content: | lvl | bs | r | dma | clt | clr | e |
 *            1 1   0    0   0     1     1     1  = 0xC7
 */
#define SERIAL_DEFAULT_FIFO_SETUP 0xC7

/**
 * Default modem settings for a COM port:
 * Ready to transmit, and data terminal ready
 * Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
 * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
 *            0   0   0    0    0     0     1     1
 */
#define SERIAL_DEFAULT_MODEM_SETUP 0x03

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
void serial_configure_fifo(unsigned short com);
void serial_configure_modem(unsigned short com);
void serial_initialize(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned short com);
void serial_write(char *buf, unsigned short com);
bool is_serial_initialized(void);

#endif // _KERNEL_SERIAL_H