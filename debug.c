#include "debug.h"

#define DBTX_PORT GPIOC //Debug TX on PC4
#define DBTX_PIN GPIO4

Queue debug_msg;

//necessary for printf to interact with hardware
void _putchar(char character){
    while (debug_msg.size == debug_msg.capacity); //wait for queue to empty
    enqueue_char((uint8_t)character);
}

void usart1_isr(){ //triggered when byte transmission was finished
    if (USART_ISR(USART1) & USART_ISR_TXE){
        if (debug_msg.size==0){ //if queue depleted
            USART_CR1(USART1) &= ~USART_CR1_TXEIE; //disable interrupt
        } else { //send next byte
            usart_send(USART1, dequeue(&debug_msg));
        }
    }
}
void init_hardware_debug_uart(uint32_t baud){
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_GPIOC);

    init_queue(&debug_msg, 256);

    gpio_mode_setup(DBTX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, DBTX_PIN);
    gpio_set_output_options(DBTX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, DBTX_PIN);
    gpio_set_af(DBTX_PORT, GPIO_AF7, DBTX_PIN);

    usart_set_baudrate(USART1, baud);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART1, USART_MODE_TX);

    nvic_enable_irq(NVIC_USART1_IRQ);

    usart_enable(USART1);
}

void enqueue_char(uint8_t in){
    enqueue(&debug_msg, in);
    USART_CR1(USART1) |= USART_CR1_TXEIE; //enable TX empty interrupt
}

/*
//semihosting to be implemented later
//
//from https://bogdanthegeek.github.io/blog/insights/jlink-rtt-for-the-masses/
int SEMIHOST_SysCall(int reason, void *arg)
{
    int value;
    __asm volatile(
        "mov r0, %[rsn] \n" // place semihost operation code into R0
        "mov r1, %[arg] \n" // R1 points to the argument array
        "bkpt 0xAB      \n" // call debugger
        "mov %[val], r0 \n" // debugger has stored result code in R0

        : [val] "=r"(value)                 // outputs
        : [rsn] "r"(reason), [arg] "r"(arg) // inputs
        : "r0", "r1", "r2", "memory");
    return value; // return result code, stored in R0
}

int getchar(void)
{
    return SEMIHOST_SysCall(SYS_READC, NULL);
}

int _write(int fd, const char *buf, int size)
{
    if (fd == SEMIHOST_STDOUT || fd == SEMIHOST_STDERR)
    {
        int32_t args[3] = {fd, (int32_t)buf, size};
        return SEMIHOST_SysCall(SYS_WRITE, &args[0]);
    }
    return 0; // Unsupported file descriptor
}

*/