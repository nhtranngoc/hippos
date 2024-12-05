#include "serialport.h"
#include "io.h"

/** sp_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void sp_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb(SP_LINE_COMMAND_PORT(com), SP_LINE_ENABLE_DLAB);
    outb(SP_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SP_DATA_PORT(com), divisor & 0x00FF);
}

/** sp_configure_line
 *  Set the line of the COM port using default setting.
 * 
 *  @param com      The COM port to configure
 */
void sp_configure_line(unsigned short com) {
    outb(SP_LINE_COMMAND_PORT(com), SP_DEFAULT_LINE_SETUP);
}

/** sp_configure_fifo
 * Set the fifo buffer of the COM port using default setting.
 * 
 * @param com       The COM port to configure
 */
void sp_configure_fifo(unsigned short com) {
    outb(SP_FIFO_COMMAND_PORT(com), SP_DEFAULT_FIFO_SETUP);
}

/** sp_configure_modem
 * Set the modem of the COM port using default setting.
 * 
 * @param com       The COM port to configure
 */
void sp_configure_modem(unsigned short com) {
    outb(SP_MODEM_COMMAND_PORT(com), SP_DEFAULT_MODEM_SETUP);
}

/** sp_setup
 *  Setup serial on a COM port using default settings
 * 
 *  @param com      COM port
 */
void sp_setup(unsigned short com) {
    if (com < 1) {
        return;
    }

    sp_configure_baud_rate(com, 8);
    sp_configure_fifo(com);
    sp_configure_line(com);
    sp_configure_modem(com);
}

/** sp_is_transmit_fifo_empty
 *  Check if the TX FIFO pipe empty - otherwise we can't send messages
 *  Line status I/O uses bit 6 to know if all previous writes have been completed
 *                       bit 5 to know if it's empty -> 00100 0000 -> 0x20
 *  
 *  @param com  COM port
 *  @return     0 if the FIFO queue is not empty
 *              1 if the FIFO queue is empty
 */
int sp_is_transmit_fifo_empty(unsigned short com) {
    /* 0x20 = 0010 0000 */
    return inb(SP_LINE_STATUS_PORT(com)) & 0x20;
}

/** sp_write
 *  Write a string buffer to the serial port COM
 * 
 *  @param *buf     String buffer to be written
 *  @param com      COM port
 */
void sp_write(char *buf, unsigned short com) {
    int i = 0;
    while (buf[i] != 0) {
        // While transmit FIFO queue isn't empty, wait.
        while (sp_is_transmit_fifo_empty(com) == 0) {
            // Do nothing
        }
        outb(com, buf[i]);
        i++;
    }
}