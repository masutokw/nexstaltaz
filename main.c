#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rtc.h>
#include <time.h>
//#include <libopencm3/stm32/pwr.h>
#include "taki.h"
#include "motor.h"
#include "nexstar.h"
#include "usb_cdc.h"
#include "mount.h"

/*extern char rx_str[128];
extern int rx_str_len;
extern char response[40];
extern int resp_size;*/
//extern void nexstar_cmd();
//extern void nexstar_init();
//extern void cdcacm_init();
//extern void cdcacm_print_buff();
//extern void speed_x_a(float target_speed);
//extern void speed_y_a(float target_speed);
/*extern unsigned int ticks_x,ticks_y;
extern long counter_x,counter_y;
extern double res_x,res_y,speed_x;
extern int dir_x,dir_y;*/
int sync_cmd=0;
uint16_t compare_time2,compare_time3;
uint16_t new_time2,new_time3;
uint16_t frequency,frequency2;
int tracedata = 0;
c_double now;
uint64_t Sys_Ticks;
static void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

static void gpio_setup(void)
{
    /* Enable GPIOC clock. */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Set GPIO13 GPIO14  (in GPIO port C) to 'output push-pull'. */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO14|GPIO13);

    gpio_clear(GPIOC, GPIO13);
}

static void tim_setup(void)
{
    /* Enable TIM2 clock. */
    rcc_periph_clock_enable(RCC_TIM2);

    /* Enable TIM2 interrupt. */
    nvic_enable_irq(NVIC_TIM2_IRQ);

    /* Reset TIM2 peripheral. */
    timer_reset(TIM2);

    /* Timer global mode:
     * - No divider
     * - Alignment edge
     * - Direction up
     */
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    /* Reset prescaler value. */
    timer_set_prescaler(TIM2, 720);

    /* Enable preload. */
    timer_disable_preload(TIM2);

    /* Continous mode. */
    timer_continuous_mode(TIM2);


    timer_set_period(TIM2, 65535);

    /* Disable outputs. */
    timer_disable_oc_output(TIM2, TIM_OC1);
    timer_disable_oc_output(TIM2, TIM_OC2);
    timer_disable_oc_output(TIM2, TIM_OC3);
    timer_disable_oc_output(TIM2, TIM_OC4);

    /* -- OC1 configuration -- */

    /* Configure global mode of line 1. */
    timer_disable_oc_clear(TIM2, TIM_OC1);
    timer_disable_oc_preload(TIM2, TIM_OC1);
    timer_set_oc_slow_mode(TIM2, TIM_OC1);
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_FROZEN);

    /* Set the capture compare value for OC1. */
    timer_set_oc_value(TIM2, TIM_OC1, 1000);

    /* ---- */

    /* ARR reload enable. */
    timer_disable_preload(TIM2);

    /* Counter enable. */
    timer_enable_counter(TIM2);

    /* Enable commutation interrupt. */
    timer_enable_irq(TIM2, TIM_DIER_CC1IE);
}

static void tim_setup3(void)
{
    rcc_periph_clock_enable(RCC_TIM3);
    nvic_enable_irq(NVIC_TIM3_IRQ);
    timer_reset(TIM3);

    /* Timer global mode:
     * - No divider
     * - Alignment edge
     * - Direction up
     */
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    /* Reset prescaler value. */
    timer_set_prescaler(TIM3, 720);
    timer_disable_preload(TIM3);
    timer_continuous_mode(TIM3);
    timer_set_period(TIM3, 65535);
    /* Disable outputs. */
    timer_disable_oc_output(TIM3, TIM_OC1);
    timer_disable_oc_output(TIM3, TIM_OC2);
    timer_disable_oc_output(TIM3, TIM_OC3);
    timer_disable_oc_output(TIM3, TIM_OC4);

    /* -- OC1 configuration -- */

    /* Configure global mode of line 1. */
    timer_disable_oc_clear(TIM3, TIM_OC1);
    timer_disable_oc_preload(TIM3, TIM_OC1);
    timer_set_oc_slow_mode(TIM3, TIM_OC1);
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_FROZEN);

    /* Set the capture compare value for OC1. */
    timer_set_oc_value(TIM3, TIM_OC1, 1000);

    /* ---- */

    /* ARR reload enable. */
    timer_disable_preload(TIM3);

    /* Counter enable. */
    timer_enable_counter(TIM3);

    /* Enable commutation interrupt. */
    timer_enable_irq(TIM3, TIM_DIER_CC1IE);
}
static void systick_setup(void)
{
    /* 72MHz / 8 => 9000000 counts per second. */

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(8999);

    systick_interrupt_enable();
    systick_counter_enable();
}

void sys_tick_handler(void)
{
    Sys_Ticks++;
}

void tim2_isr(void)
{
    if (timer_get_flag(TIM2, TIM_SR_CC1IF))
    {

        /* Clear compare interrupt flag. */
        timer_clear_flag(TIM2, TIM_SR_CC1IF);

        /*
         * Get current timer value to calculate next
         * compare register value.
         */
        compare_time2 = timer_get_counter(TIM2);

        /* Calculate and set the next compare value. */
        new_time2 = compare_time2 + ticks_x;
        timer_set_oc_value(TIM2, TIM_OC1, new_time2);
        counter_x+=dir_x;
        //do step
        gpio_toggle(AZ_PORT,AZ_PIN);
    }
}
void tim3_isr(void)
{
    if (timer_get_flag(TIM3, TIM_SR_CC1IF))
    {

        /* Clear compare interrupt flag. */
        timer_clear_flag(TIM3, TIM_SR_CC1IF);

        /*
         * Get current timer value to calculate next
         * compare register value.
         */
        compare_time3 = timer_get_counter(TIM3);

        /* Calculate and set the next compare value. */


        new_time3 = compare_time3 + ticks_y;

        timer_set_oc_value(TIM3, TIM_OC1, new_time3);
        counter_y+=dir_y;
        /* do_step */
        gpio_toggle(ALT_PORT,ALT_PIN);

    }
}

void takset(void)
{
    reset_transforms(0.0, 0.0, 0.0);
    set_star(&st_target, 0.0, 90.0, 0.0, 36.80, 0);
    init_star(1, &st_target);
    set_star(&st_target, 0.00, 36.80, 180.00, 90.00, 0);
    init_star(2, &st_target);
    compute_trasform();
    set_star(&st_target, 178.0, 10.0, 0.0, 0.0,0);
    set_star(&st_next,178.0,10.0, 0.0, 0.0,0);
    to_alt_az(&st_target);


}
void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}

/*Parse nexstar�s received string ,fired by USB RX interrupt callback  function
void nexstar_poll(void)
{
    nexstar_init();
    nexstar_cmd(rx_str,rx_str_len);
    rx_str_len=0;
    cdcacm_print_buff(response,resp_size);
}*/




//}

int main(void)
{
    int i;

    //usbd_device *usbd_dev;

    SCB_VTOR = (uint32_t) 0x08002000;
    clock_setup();
    // rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    //activate USB autodetect by setting  GPIO9
    gpio_set(GPIOB, GPIO9);

    for ( i = 0; i < 0x10000; i++)
        __asm__("nop");
    cdcacm_init();
    gpio_setup();
    systick_setup();
    tim_setup();
    tim_setup3();
    takset();
    motor_init(0,0,0.3,0.3,1000.0,1000.0);
    st_target.timer_count=( Sys_Ticks/1000.0);
    to_alt_az(&st_target);
    counter_x=st_target.az*RAD_TO_ARCS/res_x;
    counter_y=st_target.alt*180.0*3600.0/(M_PI*res_y);
    rtc_auto_awake(RCC_HSE, 0xf424);
    //
    while (1)
    {
        if (rx_str_len) nexstar_poll();//procces nexstar commnad if any
        if ((Sys_Ticks%100)==0) track();//track loop
    }
    return 0;
}
//@param[in] clock_source ::rcc_osc. RTC clock source. Only HSE, LSE    and LSI are permitted.
