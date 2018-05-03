#include "mount.h"
extern uint64_t Sys_Ticks;
extern double alt_frate;
extern double  az_frate;

//*parses nexstar received string ,fired by USB RX interrupt callback  function
void nexstar_poll(void)
{
    nexstar_init();
    nexstar_cmd(rx_str,rx_str_len);
    rx_str_len=0;
    cdcacm_print_buff(response,resp_size);
}

void  tak_init(mount_t mount,c_double longd, c_double latd)
{
    longitude=longd; //-4.20;
    latitude=latd ;//36.72;
    reset_transforms(0.0, 0.0, 0.0);
    if (mount ==ALTAZ)
    {

        set_star(&st_target,sideral(longitude)+90.0, 0.0,90.0, 0.0, 0);
        init_star(1, &st_target);
        set_star(&st_target, sideral(longitude), latitude, 180.00, 89.99, 0);
        init_star(2, &st_target);

    }
    else if (mount==EQ)
    {
        double ra    ;
        set_star(&st_target, sideral(longitude), 0.0, 180.0, 0.0, 0);
        init_star(1, &st_target);
        ra=st_target.ra+M_PI/2.0;
        if (ra<0) ra+=M_2PI;
        if (latitude>=0.0)
            set_star(&st_target, ra*RAD_TO_DEG,45,90,45, 0);
        else
            set_star(&st_target, ra*RAD_TO_DEG,-45,270,45, 0);
        init_star(2, &st_target);


    }
    compute_trasform();
    set_star(&st_target, sideral(longitude), 10.0, 0.0, 0.0,0);
    to_alt_az(&st_target);
    is_aligned=0;
    is_slewing='0';
    counter_x=counter_y=0;

}


/*Fired by Systick timer 100ms computes horizontal coordinates from ecuatorial target
 and corrected speed for both axes by using Taki´s matrix transforms */

void track(void)
{
    st_target.timer_count=st_current.timer_count=( Sys_Ticks/1000.0);
    //compute correct actual alt-az mount values
    //translate counter values to current postition alt/az values
    st_current.az=(res_x*counter_x)/RAD_TO_ARCS;
    st_current.alt=(res_y*counter_y)/RAD_TO_ARCS;
    //compute ecuatorial current equatorial values to be send out from NEXSTAR protocol interface
    to_equatorial(&st_current);
    //if trace print computed   stuff  to USB output stream
    if (tracedata) trace_frame();
  //  if (trackmode)
  if (tracking)
    {
        //compute next alt/az mount values  for target next lap second
        st_target.timer_count+= 1.0;
        to_alt_az(&st_target);
        //compute delta values :next values from actual values for desired target coordinates
        d_az_r=RAD_TO_ARCS*(st_target.az)-res_x*counter_x;
        if(fabs(d_az_r)>(M_PI*RAD_TO_ARCS)) d_az_r-=M_2PI*RAD_TO_ARCS;
        d_alt_r=RAD_TO_ARCS*(st_target.alt)-res_x*counter_y;
        if(fabs(d_alt_r)>(M_PI*RAD_TO_ARCS)) d_alt_r-=M_2PI*RAD_TO_ARCS;

        // Compute and set timer intervals for stepper  rates
        speed_x_a(d_az_r);
        speed_y_a(d_alt_r);
       // is_slewing='0';
    }
    else
    {   speed_x_a(az_frate);
        speed_y_a(alt_frate);
        if (trackmode==1) {
                tracking=(speed_y==0.0)&&(speed_x==0.0);
                  st_target.ra=st_current.ra;
                  st_target.dec=st_current.dec;}



    }

}
void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
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


void ClockShowf(void)
{
    time_t now =rtc_get_counter_val();
    struct tm * Time = localtime(&now);
    printf("%i-%02i-%02i %02i:%02i:%02i\r\n", 1900 + Time->tm_year, Time->tm_mon, Time->tm_mday, Time->tm_hour, Time->tm_min,Time->tm_sec);

}

void SetTime(char y,char m,char d,char h,char min,char sec)

{
    int new;
    struct tm  TimeSet;

    TimeSet.tm_year=y;
    TimeSet.tm_mon=m;
    TimeSet.tm_mday=d;
    TimeSet.tm_hour=h;
    TimeSet.tm_min=min;
    TimeSet.tm_sec=sec;
    TimeSet.tm_isdst=1;
    new= mktime(&TimeSet);
    if(new > -1)rtc_set_counter_val(new);
    is_aligned=1;
}



double sideral(double lon)
{

    time_t obstime;
    //  char obstimestr[40];
    // struct tm obstimestruct;
    double d, t, GMST_s, LMST_s;
    int hour, min, sec;
    //  lon = lon*180.0/M_PI;// atof(argv[1]);
    // obstime = time(NULL);   /* seconds since Unix epoch */
    obstime=  rtc_get_counter_val()-7200.0;

    /* add JD to get to the UNIX epoch, then subtract to get the days since
     * 2000 Jan 01, 12h UT1 */
    d = (obstime / 86400.0)+ 2440587.5 - 2451545.0 ;
    t = d / 36525;

    GMST_s = 24110.54841 + 8640184.812866 * t + 0.093104 *t*t - 0.0000062 * t*t*t;  /* convert from UT1=0 */
    GMST_s += obstime;
    GMST_s = GMST_s - 86400.0 * floor(GMST_s / 86400.0);

    /* adjust to LMST */
    LMST_s = GMST_s + 3600*lon/15.;

    if (LMST_s <= 0)    /* LMST is between 0 and 24h */
    {
        LMST_s += 86400.0;
    }
    d=LMST_s;
    hour = floor(LMST_s / 3600.);
    LMST_s -= 3600. * floor(LMST_s / 3600);
    min = floor(LMST_s / 60.);
    sec = LMST_s - 60 * floor(LMST_s / 60.);

    printf("%02d:%02d:%02d   %d  %f\r\n", hour, min, sec, obstime,lon);

    return d*15.0/3600.0 ;
}

// 1475610691

