#include <kernel/serial.h>
#include <kernel/io.h>

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

/** serial_configure_line
 *  Set the line of the COM port using default setting.
 * 
 *  @param com      The COM port to configure
 */
void serial_configure_line(unsigned short com) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_DEFAULT_LINE_SETUP);
}

/** serial_configure_fifo
 * Set the fifo buffer of the COM port using default setting.
 * 
 * @param com       The COM port to configure
 */
void serial_configure_fifo(unsigned short com) {
    outb(SERIAL_FIFO_COMMAND_PORT(com), SERIAL_DEFAULT_FIFO_SETUP);
}

/** serial_configure_modem
 * Set the modem of the COM port using default setting.
 * 
 * @param com       The COM port to configure
 */
void serial_configure_modem(unsigned short com) {
    outb(SERIAL_MODEM_COMMAND_PORT(com), SERIAL_DEFAULT_MODEM_SETUP);
}

/** serial_initialize
 *  Setup serial on a COM port using default settings
 * 
 *  @param com      COM port
 */
void serial_initialize(unsigned short com) {
    if (com < 1) {
        return;
    }

    serial_configure_baud_rate(com, 8);
    serial_configure_fifo(com);
    serial_configure_line(com);
    serial_configure_modem(com);
}

/** serial_is_transmit_fifo_empty
 *  Check if the TX FIFO pipe empty - otherwise we can't send messages
 *  Line status I/O uses bit 6 to know if all previous writes have been completed
 *                       bit 5 to know if it's empty -> 00100 0000 -> 0x20
 *  
 *  @param com  COM port
 *  @return     0 if the FIFO queue is not empty
 *              1 if the FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned short com) {
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/** serial_write
 *  Write a string buffer to the serial port COM
 * 
 *  @param *buf     String buffer to be written
 *  @param com      COM port
 */
void serial_write(char *buf, unsigned short com) {
    int i = 0;
    while (buf[i] != 0) {
        // While transmit FIFO queue isn't empty, wait.
        while (serial_is_transmit_fifo_empty(com) == 0) {
            // Do nothing
        }
        outb(com, buf[i]);
        i++;
    }
}