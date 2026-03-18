#ifndef HW_DB_UART_H
#define HW_DB_UART_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <queue.h>
#include <printf.h>
#include <stdint.h>

extern Queue debug_msg;

void init_hardware_debug_uart(uint32_t baud); //initialize timer, interrupt, GPIO
void enqueue_char(uint8_t in); //send char to internal queue to be sent out

#endif