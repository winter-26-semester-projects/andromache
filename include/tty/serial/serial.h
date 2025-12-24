#include<stdint.h>

#define UART_PORT_COM1 0x3F8

/* Register Offsets */
#define UART_TX     0  // Transmit
#define UART_IER    1  // Interrupt Enable
#define UART_FCR    2  // FIFO Control
#define UART_LCR    3  // Line Control
#define UART_MCR    4  // Modem Control
#define UART_LSR    5  // Line Status

/* Bitmasks */
#define UART_LCR_DLAB 0x80 // Divisor Latch Access Bit
#define UART_LSR_THRE 0x20 // Transmit Holding Register Empty

void early_serial_init(uint16_t);
void serial_putc(uint16_t, char);
