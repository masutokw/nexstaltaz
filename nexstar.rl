#include <string.h>
#include <stdio.h>
#include "taki.h"
#include "nexstar.h"
#include <time.h>

#ifndef M_PI
#define M_PI acos(-1.0)
#endif
#define ADD_DIGIT(var,digit)  if  (digit>'9') var=(var<<4)+digit-'7'; else var=(var<<4) +digit-'0';
#define RAD_TO_CEL_P (16777216/(2.0*M_PI))
#define RAD_TO_CEL (65536/(2.0*M_PI))
double rad_to_cel;
char stcmd;
long deg=0;
long *px ,*py;
long x=3000;
long y=10000;
long ra=3000;
long dec=1000;
long alt=0;
long az=0;
int prec=0;
int sync_cmd;
int inv_dir=0;
long vrate=0;
long rate=0;
long frate=0;
double alt_frate=0;
double az_frate=0;
long axe=0;
char digit;
long var;
char trackmode=1;
char alt_axis,neg;
int cs;
char A=36;
char B=8;
char C=4;
char D=0;
char E=4;
char F=12;
char G=0;
char H=1;
char hour,min,sec,year,month,day,tzone,dst;
extern int tracedata;
/*extern double res_x,res_y;
extern long counter_x,counter_y;*/
%%{
	machine command;
	write data;
}%%


%%{
#Actions
    action getx { ADD_DIGIT(x,fc);}
    action gety {ADD_DIGIT(y,fc);}
    action storecmd {stcmd=fc;}

    action return_eq
    {
        ra=st_current.ra*rad_to_cel;
        dec=st_current.dec*rad_to_cel;
        if (st_current.dec<0.0) dec+=(2*M_PI)*rad_to_cel;
        if (prec)
            sprintf(response,"%06lX00,%06lX00#",ra,dec);

        else {
            sprintf(response,"%04lX,%04lX#",ra,dec);
        }
        prec=0;
    }

    action return_altaz {
        az=st_current.az*rad_to_cel;
        alt=st_current.alt*rad_to_cel;
        if (prec)
            sprintf(response,"%06lX00,%06lX00#",az,alt);
        else
            sprintf(response,"%04lX,%04lX#",az,alt);
        prec=0;
    }
    action goto_eq {
        px=&ra;
        py=&dec;
        sync_cmd=0;
    }
    action goto_altaz {
        px=&alt;
        py=&az;
    }
    action sync {
        px=&ra;
        py=&dec;
        sync_cmd=1;
    }

    action return_track_mode {
        sprintf(response,"%c#",trackmode);
         resp_size=2;

    }

    action set_track_mode{
        trackmode=fc;
        if (trackmode)
          {st_target.ra=st_current.ra;
          st_target.dec=st_current.dec;}
        sprintf(response,"#");


    }

    action precise {prec=1;rad_to_cel=RAD_TO_CEL_P;}
    #action getcoord {sprintf(response,"set_fixed_rate\r\n");}f
    action rate_high_byte {vrate+=fc*256;}

    action rate_low_byte {vrate+=fc;}

    action fixed_rate {frate=fc;}

    action inv {inv_dir=1; if ((fc==7)||(fc==37)) inv_dir=-1;}
    action alt_axis {alt_axis=(fc==17);}
    action set_frate {
        if (alt_axis) alt_frate=frate*frate*200.0*inv_dir ;else az_frate=frate*frate*200.0*-(inv_dir);
         trackmode=0;
       sprintf(response,"#");


    //printf("[set_fixed_rate]");
    }
    action set_vrate{sprintf(response,"#");
     if (alt_axis) alt_frate=vrate*inv_dir ;else az_frate=vrate*-(inv_dir);
    sprintf(response,"#");

    }

    action get_version
    {
        sprintf(response,"%c%c#",4,10);

    }

    action get_model {
        sprintf(response,"%c#",1);
         resp_size=2;

    }

    action is_aligned
    {
        sprintf(response,"%c#",is_aligned);
        resp_size=2;

    }

    action is_slewing {
        sprintf(response,"%c#",is_slewing);
        resp_size=2;
    }

    action set_cancel
    {
        sprintf(response,"#");

    }

    action get_dev_version{sprintf(response,"%c%c#",4,1);resp_size=3;}
    action execute{if (resp_size==0) resp_size=strlen(response) ;}
    action echo  {sprintf(response,"%c#",fc);}
	action connected  {sprintf(response,"#");}
    action save
    {
        *px=x;
        *py=y;
        sprintf(response,"#");
        st_target.ra=(ra)/rad_to_cel;
        st_target.dec=(dec)/rad_to_cel;
        st_next.ra=st_target.ra;
        st_next.dec=st_target.dec;
        if (sync_cmd)
        {
            to_alt_az(&st_target);
            counter_x=st_target.az*RAD_TO_ARCS/res_x;
            counter_y=st_target.alt*RAD_TO_ARCS/res_y;
            sync_cmd=0;
        }
    }
    action A {A=fc;}
    action B {B=fc;}
    action C {C=fc;}
    action D {D=fc;}
    action E {E=fc;}
    action F {F=fc;}
    action G {G=fc;}
    action H {H=fc;
                   Set_Coord();
                   sprintf(response,"#");
              }
    action Hour {hour=fc;}
    action Min {min=fc;}
    action Sec {sec=fc;}
    action Month {month=fc;}
    action Day{day=fc;}
    action Year{year=fc;}
    action Tzone{tzone=fc;}
    action Dst{
        dst=fc;
         SetTime( 100+year, month-1,day,hour,min,sec);
         sprintf(response,"#");
        }

	action getcoord {Get_Coord();}
    action traceport {tracedata=!tracedata;}
    action SETTIME {// printf ("%d %d %d %d %d %d\r\n",year-100, month,day,hour,min,sec);
     //SetTime( year, month,day,hour,min,sec);
     // printf("%d %d %d %d %d %d %d %d %f#",A,B,C,D,E,F,G,H,longitude);
     sideral(longitude);
    }
    action gettime {
        time_t now = rtc_get_counter_val();
        struct tm * Time = localtime(&now);
        sprintf(response,"%c%c%c%c%c%c%c%c#",Time->tm_hour, Time->tm_min,Time->tm_sec,Time->tm_mon,Time->tm_mday,Time->tm_year,2,Time->tm_isdst);
        resp_size=9;
    }
    action printsideral{sideral(-4.20);

    }
   # action gettime {ClockShow();}
#definicion sintaxis Nextstar
    Xdigit=[0-9A-F];
#Syntax

    Coords=((Xdigit @getx){4} ',' (Xdigit @gety){4})@save ;
    Coords_p=((Xdigit @getx){6}Xdigit{2} ',' (Xdigit @gety){6}Xdigit{2})@save ;
# GET position commands
    Eq = ('E' | '?E'	|'e' @precise) @return_eq;
    Altaz=('Z' | 'z' @precise) @return_altaz;
    init= '?'@connected;
    Get= (Eq | Altaz);



#GOTO commands

    Goto_Sync_pre=(('r' @goto_eq ) | ('b'  @goto_altaz ) |  ('s' @sync ) ) @precise Coords_p  ;
    Goto_Sync_s=(('R' @goto_eq ) | ('B' @goto_altaz ) |  ('S' @sync ) ) Coords ;
    Goto_Sync =Goto_Sync_pre |Goto_Sync_s;


#TRACKING commands
    GetTrack = 't' @return_track_mode;
    SetTrack = 'T' (0..3)@set_track_mode ;
    Track= GetTrack | SetTrack;

# SLEWING variable and fixed rate commands
    var_rate =(extend @rate_high_byte) (extend@rate_low_byte) ;
    fix_rate =extend @fixed_rate;
    Vr = (3|4) (((16 | 17)@alt_axis) ((( 6  |  7)@inv)  var_rate ) ) 0 0 @set_vrate    ;
    Fr = 2 (((16 | 17)@alt_axis ) (((36 | 37)@inv)  fix_rate ) ) 0 0 0 @set_frate;
    Slew ='P' (Vr | Fr) ;

#TIME / LOCATION commands
    Get_Loc='w'@getcoord;
    Set_Loc='W' (0..90)@A (0..59)@B(0..59)@C(0..1)@D(extend)@E (0..59)@F(0..59)@G(0..1)@H;
    Get_Time='h'@gettime;
    Set_Time='H' (0..23)@Hour(0..59)@Min(0..59)@Sec(1..12)@Month(1..31)@Day(extend)@Year(extend)@Tzone(0..1)@Dst;
    Loc=Get_Loc | Set_Loc | Get_Time | Set_Time;

#Misc commands.
    Get_version=('V'|'v')@get_version;
    Get_model='m'@get_model;
    azmotor=16;
    altmotor=17;
    gps=0xB0;
    rtc=0xB2;
    dev=( azmotor | altmotor | gps | rtc);
    echo= 'K' extend @echo;
    align_completed='J'@is_aligned;
    goto_progress='L' @is_slewing ;
    cancel_goto="M"@set_cancel;
    Get_dev_version='P' (1|2) (dev) (0xFE) (0x0{3}) 0x02 @get_dev_version;
    Misc=Get_version | Get_model | Get_dev_version | echo |align_completed | goto_progress |cancel_goto ;
    Trace='G'@traceport;
    Time= 'X'@SETTIME;
    Sideral='S'@printsideral;
	main :=  (( Goto_Sync | Get | Track | Slew | Loc  | Misc|Trace|Time|Sideral)%execute) ;



}%%



void nexstar_init(void)
{
    %%write init;
}
void nexstar_cmd( char *str,int len )
{
    x=y=0;
    resp_size=0;rad_to_cel=RAD_TO_CEL;
    char *p = str, *pe = str + len;
    char *eof=pe;
    %%write exec;

};

void checkfsm(void)
{
    if ( cs < command_first_final )
        printf("FAIL");
    else
        printf("ACCEPT");
}



inline void Get_Coord (void)
{
    long temp =fabs(latitude*3600.0);
    D=H=1;
    if (latitude<0.0) D=0;
    A=temp/3600;
    temp=temp%3600;
    B=temp/60;
    C=temp%60;
    temp =fabs(longitude*3600);
    if (longitude>0.0) H=0;
    E=temp/3600;
    temp=temp%3600;
    F=temp/60;
    G=temp%60;
    sprintf(response,"%c%c%c%c%c%c%c%c#",A,B,C,D,E,F,G,H);
    resp_size=9;
}
inline void Set_Coord(void)
{
    longitude=(E*3600.0+F*60.0+G)/3600.0;//RAD_TO_ARCS ;
    if (H) longitude=-longitude;
    latitude=(A*3600.0+B*60.0+C)/3600.0;//RAD_TO_ARCS ;
    if (!D) latitude=-latitude;
}
