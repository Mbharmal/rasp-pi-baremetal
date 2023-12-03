#define PERIPHERAL_BASE (0xFE000000)

// IO Read/write
unsigned int io_read (long address) {
    return *((volatile unsigned int *) address);
}

void io_write (long address, unsigned int value) {
    *(volatile unsigned int *) address = value;
}

// GPIO

#define GPFSEL0         (PERIPHERAL_BASE + 0x200000)
#define GPSET0          (PERIPHERAL_BASE + 0x20001C)
#define GPCLR0          (PERIPHERAL_BASE + 0x200028)
#define GPPUPPDN0       (PERIPHERAL_BASE + 0x2000E4)

#define GPIO_MAX_PIN    53

unsigned int gpio_config (unsigned int pin_number, unsigned int value, unsigned int base,
                          unsigned int field_size, unsigned int field_max) {
    unsigned int field_mask = (1 << field_size) - 1;
  
    if (pin_number > field_max) return 0;
    if (value > field_mask) return 0; 

    unsigned int num_fields = 32 / field_size;
    unsigned int reg = base + ((pin_number / num_fields) * 4);
    unsigned int shift = (pin_number % num_fields) * field_size;

    unsigned int curval = io_read(reg);
    curval &= ~(field_mask << shift);
    curval |= value << shift;
    io_write(reg, curval);

    return 1;
}

unsigned int gpio_set (unsigned int pin_number, unsigned int value) {
    return gpio_config(pin_number, value, GPSET0, 1, GPIO_MAX_PIN);
}

unsigned int gpio_clear (unsigned int pin_number, unsigned int value) {
    return gpio_config(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN);
}

unsigned int gpio_pull (unsigned int pin_number, unsigned int value) {
    return gpio_config(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN);
}

unsigned int gpio_function (unsigned int pin_number, unsigned int value) {
    return gpio_config(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN);
}

// UART

#define AUX_BASE       (PERIPHERAL_BASE + 0x215000)
#define AUX_ENABLES    (AUX_BASE + 4)
#define AUX_MU_IO_REG  (AUX_BASE + 64)
#define AUX_MU_IER_REG (AUX_BASE + 68)
#define AUX_MU_IIR_REG (AUX_BASE + 72)
#define AUX_MU_LCR_REG (AUX_BASE + 76)
#define AUX_MU_MCR_REG (AUX_BASE + 80)
#define AUX_MU_LSR_REG (AUX_BASE + 84)
#define AUX_MU_CNTL_REG (AUX_BASE + 96)
#define AUX_MU_BAUD_REG (AUX_BASE + 104)

#define AUX_UART_CLOCK (500000000)
#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

void uart_init (void) {
    io_write(AUX_ENABLES, 1);
    io_write(AUX_MU_IER_REG, 0);
    io_write(AUX_MU_CNTL_REG, 0);
    io_write(AUX_MU_LCR_REG, 3);
    io_write(AUX_MU_MCR_REG, 0);
    io_write(AUX_MU_IER_REG, 0);
    io_write(AUX_MU_IIR_REG, 0xC6);
    io_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));

    // Configure GPIO pins 14 and 15
    gpio_pull(14, 0);
    gpio_function(14, 2);
    gpio_pull(15, 0);
    gpio_function(15, 2);

    io_write(AUX_MU_CNTL_REG, 3);
}

void uart_putc (char c) {
    // Wait while UART is not ready
    while (!(io_read(AUX_MU_LSR_REG) & 0x20));

    io_write(AUX_MU_IO_REG, (unsigned int) c);
}
