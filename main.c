#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/cm3/cortex.h>
#include <math.h>

#include "printf.h"
#include "debug.h"
#include "utility.h"

int main(void)
{
    init_system(); //to edit clock configurations and SysTick, edit "utility.c"
    init_hardware_debug_uart(115200); //initialize debug through UART

    //LED on C6 on WeAct STM32G474 Core board
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);

    while(1){
        gpio_toggle(GPIOC, GPIO6);
        delay_ms(500);
        printf("Hello World!");
    }

    return 0;
}
