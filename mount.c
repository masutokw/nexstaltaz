#include "mount.h"
extern uint64_t Sys_Ticks;

//*parses nexstar received string ,fired by USB RX interrupt callback  function
void nexstar_poll(void)
{
    nexstar_init();
    nexstar_cmd(rx_str,rx_str_len);
    rx_str_len=0;
    cdcacm_print_buff(response,resp_size);
}

/*Fired by Systick timer 100ms computes horizontal coordinates from ecuatorial target
 and corrected speed for both axes by using Taki´s matrix transforms */

void track(void)
{
    st_target.timer_count=st_current.timer_count=( Sys_Ticks/1000.0);
    //compute correct actual alt-az mount values
    to_alt_az(&st_target);
    //translate counter values to current postition alt/az values
    st_current.az=(res_x*counter_x)/RAD_TO_ARCS;
    st_current.alt=(res_y*counter_y)/RAD_TO_ARCS;
    //compute ecuatorial current equatorial values to be send out from NEXSTAR protocol interface
    to_equatorial(&st_current);
    //compute next alt/az mount values  for target next lap second
    st_next.timer_count= st_target.timer_count+1.0;
    to_alt_az(&st_next);
    //compute delta values :next values from actual values for desired target coordinates
    d_az_r=RAD_TO_ARCS*(st_next.az)-res_x*counter_x;
    d_alt_r=RAD_TO_ARCS*(st_next.alt)-res_x*counter_y;
    //if trace print computed   stuff  to USB output stream
    if (tracedata) trace_frame();
    // Compute and set timer intervals for stepper  rates
    speed_x_a(d_az_r);
    speed_y_a(d_alt_r);

}

void trace_frame(void)
{
    gotoxy(0,4);
    c_double d_az= RAD_TO_ARCS*(st_next.az-st_target.az);
    c_double d_alt=RAD_TO_ARCS*(st_next.alt-st_target.alt);
    c_double temp=st_target.az*RAD_TO_DEG;
    c_double  temp1=st_target.alt*RAD_TO_DEG;
    printf("Time :%2.3fs  %2.4f  %2.4f  %2.4f %2.4f \r\n ",Sys_Ticks/1000.0,temp,temp1, d_az,d_alt );
    printf("Delta %+4.2f %+4.2f \r\n",temp*3600.0-res_x*counter_x, temp1*3600.0-res_y*counter_y );
    printf("Freq %2.4f %2.4f\r\n",d_az/res_x,d_alt/res_y);
    printf("Freq %2.4f %2.4f %f\r\n",d_az_r/res_x,d_alt_r/res_y,speed_x);
    printf("%f\r\n",rtc_get_counter_val()-Sys_Ticks/1000.0);
    ClockShow();
}

void ClockShow(void)
{

    time_t now = rtc_get_counter_val();
    struct tm * Time = localtime(&now);
    printf("%i-%02i-%02i %02i:%02i:%02i\r\n", 1900 + Time->tm_year, 1+Time->tm_mon, Time->tm_mday, Time->tm_hour, Time->tm_min,Time->tm_sec);
  //   printf("%i-%02i-%02i %02i:%02i:%02i\r\n",  Time->tm_hour, Time->tm_min,Time->tm_sec, 1+Time->tm_mon, Time->tm_mday,Time->tm_year,);

}

void SetTime(int y,int m,int d,int h,int min,int sec)

{
    int new;
    struct tm  TimeSet;
    TimeSet.tm_year=y;
    TimeSet.tm_mon=m;
    TimeSet.tm_mday=d;
    TimeSet.tm_hour=h;
    TimeSet.tm_min=min;
    TimeSet.tm_sec=sec;
    new= mktime(&TimeSet);
    if(new > -1)rtc_set_counter_val(new);


}
