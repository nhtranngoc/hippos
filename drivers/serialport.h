#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include "io.h"

/* All I/O ports are calculated relative to the data port. This is because
* all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
* order, but they start at different values.
*/
#define SP_DATA_PORT(base)              base
#define SP_FIFO_COMMAND_PORT(base)      (base + 2)
#define SP_LINE_COMMAND_PORT(base)      (base + 3)
#define SP_MODEM_COMMAND_PORT(base)     (base + 4)
#define SP_LINE_STATUS_PORT(base)       (base + 5)

// Base address of COM ports
enum COM_PORTS {
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8,
    COM5 = 0x5F8,
    COM6 = 0x4F8,
    COM7 = 0x5F8,
    COM8 = 0x4E8
};

/* The I/O port commands */

/* SP_LINE_ENABLE_DLAB:
* Tells the serial port to expect first the highest 8 bits on the data port,
* then the lowest 8 bits will follow
*/
#define SP_LINE_ENABLE_DLAB             0x80

/**
 * Default line settings for a COM port: 8 bit length, no parity, one stop bit, disabled break control
 */
/* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
* Content: | d | b | prty  | s | dl  |
* Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
*/
#define SP_DEFAULT_LINE_SETUP 0x03

/**
 * Default fifo buffer settings for a COM port:
 * FIFO enabled, clear both receiver and transmission queue, queue size is 14 bytes
 * Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
 * Content: | lvl | bs | r | dma | clt | clr | e |
 *            1 1   0    0   0     1     1     1  = 0xC7
 */
#define SP_DEFAULT_FIFO_SETUP 0xC7

/**
 * Default modem settings for a COM port:
 * Ready to transmit, and data terminal ready
 * Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
 * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
 *            0   0   0    0    0     0     1     1
 */
#define SP_DEFAULT_MODEM_SETUP 0x03

void sp_configure_baud_rate(unsigned short com, unsigned short divisor);
void sp_configure_line(unsigned short com);
void sp_configure_fifo(unsigned short com);
void sp_configure_modem(unsigned short com);
void sp_setup(unsigned short com);
int sp_is_transmit_fifo_empty(unsigned short com);
void sp_write(char *buf, unsigned short com);

#endif // SERIAL_PORT_H