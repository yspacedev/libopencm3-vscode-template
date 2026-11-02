#include "utility.h"
volatile uint32_t system_millis = 0;

/* SysTick interrupt handler: increments millisecond counter */
void sys_tick_handler(void)
{
    system_millis++;
}

void delay_ms(uint32_t ms){
    uint32_t prev_time = system_millis;
    while ((system_millis-prev_time)<ms);
}

uint32_t get_millis(){
    return system_millis;
}

void init_system(void){
    //setup clock with 8MHz crystal, 3.3v supply, and 170MHz core clock. Change for target microcontroller
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_170MHZ]);
    
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(rcc_ahb_frequency / 1000 - 1);  // 1ms period
    systick_interrupt_enable();
    systick_counter_enable();
}