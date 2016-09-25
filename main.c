
/*

 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/scb.h>
#include "taki.h"
#include "motor.h"

extern char rx_str[128];
extern int rx_str_len;
extern char response[40];
extern int resp_size;
extern void nexstar_cmd();
extern void nexstar_init();
extern void cdcacm_init();
extern void cdcacm_print_buff();
extern unsigned int ticks_x,ticks_y;
extern long counter_x,counter_y;
extern float res_x,res_y;
extern int dir_x,dir_y;
int sync_cmd=0;
uint16_t compare_time,compare_time3;
uint16_t new_time,new_time3;
uint16_t frequency,frequency2;
int tracedata = 0;
c_star st_target,
       st_next,
       st_current;
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

    /* Set GPIO12 (in GPIO port C) to 'output push-pull'. */
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

    /* Period (36kHz). */
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
    /* Enable TIM2 clock. */
    rcc_periph_clock_enable(RCC_TIM3);

    /* Enable TIM2 interrupt. */
    nvic_enable_irq(NVIC_TIM3_IRQ);

    /* Reset TIM2 peripheral. */
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

    /* Enable preload. */
    timer_disable_preload(TIM3);

    /* Continous mode. */
    timer_continuous_mode(TIM3);

    /* Period (36kHz). */
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

    /* Start counting. */
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
        compare_time = timer_get_counter(TIM2);

        /* Calculate and set the next compare value. */
        new_time = compare_time + ticks_x;
        timer_set_oc_value(TIM2, TIM_OC1, new_time);
        counter_x+=dir_x;
        /* Toggle LED to indicate compare event. */
        gpio_toggle(GPIOC, GPIO14);
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
        /* Toggle LED to indicate compare event. */
        gpio_toggle(GPIOC, GPIO13);

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

/*parses nexstar received string ,fired by USB RX interrupt callback  function*/
void nexstar_poll(void)
{
    nexstar_init();
    nexstar_cmd(rx_str,rx_str_len);
    rx_str_len=0;
    cdcacm_print_buff(response,resp_size);
}

/* fired by Systick timer 100ms computes horizontal coordinates from ecuatorial target
 and corrected speed for both axes by using Taki´s matrix transforms*/

void track(void)
{
    st_target.timer_count=st_current.timer_count=( Sys_Ticks/1000.0);
    //compute correct actual alt-az mount values
    to_alt_az(&st_target);
    //if received sync command from parser set current counter values  to new position
    if (sync_cmd)
    {
        counter_x=st_target.az*RAD_TO_ARCS/res_x;
        counter_y=st_target.alt*RAD_TO_ARCS/res_y;
        sync_cmd=0;
    }
    //translate counter values to current postition alt/az values
    st_current.az=(res_x*counter_x)/RAD_TO_ARCS;
    st_current.alt=(res_y*counter_y)/RAD_TO_ARCS;


    // st_r.az=st_target.az;
    // st_r.alt=st_target.alt;
    //compute ecuatorial current equatorial values to be send out from NEXSTAR protocol interface
    to_equatorial(&st_current);
    //compute next alt/az mount values  for target next lap second
    st_next.timer_count= st_target.timer_count+1.0;
    to_alt_az(&st_next);

    c_double d_az= RAD_TO_ARCS*(st_next.az-st_target.az);
    c_double d_alt=RAD_TO_ARCS*(st_next.alt-st_target.alt);

    float temp=st_target.az*RAD_TO_DEG;
    float temp1=st_target.alt*RAD_TO_DEG;

    //compute delta values :next values from actual values for desired target coordinates
    c_double d_az_r=RAD_TO_ARCS*(st_next.az)-res_x*counter_x;
    c_double d_alt_r=RAD_TO_ARCS*(st_next.alt)-res_x*counter_y;
    //if trace print computed   stuff  to USB output stream
    if (tracedata)
    {
        gotoxy(0,4);
        printf("Time :%2.3fs  %2.4f  %2.4f  %2.4f %2.4f \r\n ",Sys_Ticks/1000.0,temp,temp1, d_az,d_alt );
        printf("Delta %+4.2f %+4.2f \r\n",temp*3600.0-res_x*counter_x, temp1*3600.0-res_y*counter_y );
        printf("Freq %2.4f %2.4f\r\n",d_az/res_x,d_alt/res_y);
        //to_equatorial(&st_r);
        printf("Freq %2.4f %2.4f\r\n",d_az/res_x,d_alt/res_y);
    };

    // Computes and timer intervals for stepper  rates
    speedy(d_alt_r);
    speedx(d_az_r);

}


//}

int main(void)
{
    int i;

    //usbd_device *usbd_dev;

    SCB_VTOR = (uint32_t) 0x08002000;
    clock_setup();
    // rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set(GPIOB, GPIO9);

    for ( i = 0; i < 0x10000; i++)
        __asm__("nop");

    cdcacm_init();
    gpio_setup();
    systick_setup();
    tim_setup();
    tim_setup3();
    takset();
    motor_init(0,0,0.3,0.3);
    st_target.timer_count=( Sys_Ticks/1000.0);
    to_alt_az(&st_target);
    counter_x=st_target.az*RAD_TO_ARCS/res_x;
    counter_y=st_target.alt*180.0*3600.0/(M_PI*res_y);
    while (1)
    {

        if (rx_str_len) nexstar_poll();
        if ((Sys_Ticks%100)==0) track();

    }
    return 0;
}
