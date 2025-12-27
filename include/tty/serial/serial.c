#include "machine/io.h"
#include "serial.h"

void early_serial_init(uint16_t port) 
{
    outb(port + UART_IER, 0x00);          // Disable interrupts
    outb(port + UART_LCR, UART_LCR_DLAB); // Enable DLAB
    outb(port + 0, 0x01);                 // 115200 baud (divisor 1)
    outb(port + 1, 0x00);
    outb(port + UART_LCR, 0x03);          // 8n1
    outb(port + UART_FCR, 0xC7);          // Enable FIFO
}

void serial_putc(uint16_t port, char c) 
{
    while (!(inb(port + UART_LSR) & UART_LSR_THRE));
    outb(port, c);
}

void serial_write(uint16_t port, const char* str) 
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') serial_putc(port, '\r');
        serial_putc(port, str[i]);
    }
}
