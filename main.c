#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/cm3/cortex.h>
#include <math.h>

#include <printf.h>
#include <debug.h>
#include <queue.h>

static volatile uint32_t system_millis = 0;

/* SysTick interrupt handler: increments millisecond counter */
void sys_tick_handler(void)
{
    system_millis++;
}

void delay_ms(uint32_t ms){
    uint32_t prev_time = system_millis;
    while ((system_millis-prev_time)<ms);
}

int main(void)
{
    //setup clock with 8MHz crystal, 3.3v supply, and 170MHz core clock. Weirdly hard to find too
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_170MHZ]);
    
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(rcc_ahb_frequency / 1000 - 1);  // 1ms period
    systick_interrupt_enable();
    systick_counter_enable();

    init_hardware_debug_uart(115200);

    rcc_periph_clock_enable(RCC_GPIOC);

    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);

    while(1){
        gpio_toggle(GPIOC, GPIO6);
        delay_ms(1000);
        printf("Hello World! :3c");
    }

    return 0;
}
