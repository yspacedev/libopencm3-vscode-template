#ifndef UTILITY_H
#define UTILITY_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/cm3/cortex.h>

void delay_ms(uint32_t ms);
uint32_t get_millis();
void init_system(void);

#endif