#include <string.h>
#include <stdio.h>
#include "taki.h"
#ifndef M_PI
#define M_PI 3.14159265359
//#define M_PI acos(-1.0)
#endif
#define ADD_DIGIT(var,digit)  if  (digit>'9') var=(var<<4)+digit-'7'; else var=(var<<4) +digit-'0';
#define RAD_TO_CEL_P (16777216/(2.0*M_PI))
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
extern int sync_cmd;
int inv_dir=0;
long vrate=0;
long rate=0;
long frate=10;
long axe=0;
char digit;
long var;
char trackmode=1;
char alt_axis,neg;
int cs;
char response[40];
int resp_size;
char A=36;
char B=12;
char C=4;
char D=0;
char E=4;
char F=12;
char G=0;
char H=1;
extern c_star st_target,st_next,st_current;
extern int tracedata;

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
       if (prec) {
       ra=st_current.ra*RAD_TO_CEL_P;
        if (st_current.dec>=0.0) dec=st_current.dec*RAD_TO_CEL_P;
       else dec= ((2.0*M_PI) +st_current.dec)*RAD_TO_CEL_P;
       sprintf(response,"%06lx00,%06lx00#",ra,dec);}
        else sprintf(response,"%04lx,%04lx#",ra,dec);
        prec=0;
    }

    action return_altaz {
        if (prec)
            sprintf(response,"%08lx,%08lx#",alt,az);
        else
            sprintf(response,"%04lx,%04lx#",alt,az);
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

    }

    action set_track_mode{
        trackmode=fc;
        sprintf(response,"#");

    }

    action precise {prec=1;}
    #action getcoord {sprintf(response,"set_fixed_rate\r\n");}f
    action rate_high_byte {vrate+=fc*256;}

    action rate_low_byte {vrate+=fc;}

    action fixed_rate {frate=fc;}

    action inv {inv_dir=1;}
    action alt_axis {alt_axis=1;}
    action set_frate {sprintf(response,"#");
    //printf("[set_fixed_rate]");
    }
    action set_vrate{sprintf(response,"#");

    }

    action get_version
    {
        sprintf(response,"%c%c#",4,10);
        //printf("get_version\r\n");
    }

    action get_model {
        sprintf(response,"%c#",1);
        //printf("getmodel\r\n");
    }

    action is_aligned
    {
        sprintf(response,"%c#",1);
      //  printf("is_aligned");
    }

    action is_slewing {sprintf(response,"is_slewing\r\n");}

    action set_cancel
    {
        sprintf(response,"#");
        //printf("Cancel_goto\r\n");
    }

    action get_dev_version{sprintf(response,"%c%c#",0,0);resp_size=3;}
    action execute{if (resp_size==0) resp_size=strlen(response) ;}
    action echo  {sprintf(response,"%c#",fc);}
	action connected  {sprintf(response,"#");}
    action save {*px=x;*py=y; sprintf(response,"#");

    st_target.ra=(ra)/RAD_TO_CEL_P;st_target.dec=(dec)/RAD_TO_CEL_P;
    st_next.ra=st_target.ra;st_next.dec=st_target.dec;

    }
	action getcoord {sprintf(response,"%c%c%c%c%c%c%c%c#",A,B,C,D,E,F,G,H);resp_size=9;}
    action traceport {tracedata=!tracedata;}

#definicion sintaxis Nextstar
    Xdigit=[0-9A-F];
#Syntax

    Coords=((Xdigit @getx){4} ',' (Xdigit @gety){4})@save ;
#| ((Xdigit @getx) {8} ',' (Xdigit @gety){8});
    Coords_p=((Xdigit @getx){6}Xdigit{2} ',' (Xdigit @gety){6}Xdigit{2})@save ;
#	Coords=(Xdigit @getx)+ ',' (Xdigit @gety)+;
# GET position commands
    Eq = ('E' | '?E'	|'e' @precise) @return_eq;
    Altaz=('Z' | 'z' @precise) @return_altaz;
    init= '?'@connected;
    Get= (Eq | Altaz);



#GOTO commands
#GotoEq = 	(('R'|('r' @precise) )@goto_eq )Coords ;
#GotoAltaz = (('B'Coords)|('b' @precise  Coords)) %goto_altaz;
#Goto   = (GotoEq |GotoAltaz);
#Goto_Sync=((('R'|('r' @precise) )@goto_eq ) | (('B'|('b' @precise) )@goto_altaz ) |  (('S'|('s' @precise) )@sync ) ) Coords ;
    Goto_Sync_pre=(('r' @goto_eq ) | ('b'  @goto_altaz ) |  ('s' @sync ) ) @precise Coords_p  ;
    Goto_Sync_s=(('R' @goto_eq ) | ('B' @goto_altaz ) |  ('S' @sync ) ) Coords ;
    Goto_Sync =Goto_Sync_pre |Goto_Sync_s;
#SYNC commands
#Sync   = (	'S'Coords  | ()'s'Coords_p @precise) %sync;

#TRACKING commands
    GetTrack = 't' @return_track_mode;
    SetTrack = 'T' (0..3)@set_track_mode ;
    Track= GetTrack | SetTrack;

# SLEWING variable and fixed rate commands
    var_rate =(extend @rate_high_byte) (extend@rate_low_byte) ;
    fix_rate =extend @fixed_rate;
    Vr = 1 ((16 | 17 @alt_axis) (( 6  |  7@inv)  var_rate ) ) 0 0 @set_vrate    ;
    Fr = 3 ((16 | 17@alt_axis ) ((36 | 37@inv)  fix_rate ) ) 0 0 0 @set_frate;
    Slew ='P' (Vr | Fr) ;

#TIME / LOCATION commands
    Get_Loc='w'@getcoord;
    Set_Loc='W' (0..90) (0..59)(0..59)(0..1)(extend) (0..59)(0..59)(0..1);
    Get_Time='h';
    Set_Time='H' (0..23)(0..59)(0..59)(1..12)(1..31)(extend)(extend)(0..1);
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
    Get_dev_version='P' 0x02 (dev) (0xFE) (0x0{3}) 0x02 @get_dev_version;
    Misc=Get_version | Get_model | Get_dev_version | echo |align_completed | goto_progress |cancel_goto ;
    Trace='G'@traceport;
	main :=  (( Goto_Sync | Get | Track | Slew | Loc  | Misc|Trace)%execute) ;



}%%

void nexstar_init(void)
{
    %%write init;
}
void nexstar_cmd( char *str,int len )
{x=y=0;
    resp_size=0;
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


