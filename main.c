void uart_init (void);
void uart_putc (char c);

void main()
{
    uart_init();

    while (1) {
        // Find a better way ;)
        // For now it will work though
        uart_putc('H');
        uart_putc('e');
        uart_putc('l');
        uart_putc('l');
        uart_putc('o');
        uart_putc(' ');
        uart_putc('W');
        uart_putc('o');
        uart_putc('r');
        uart_putc('l');
        uart_putc('d');
        uart_putc('!');
        uart_putc('\r');
        uart_putc('\n');
    }
}
