
/* #line 1 "nexstar.rl" */
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

/* #line 2 "nexstar.c" */
static const char _command_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 7, 1, 
	8, 1, 10, 1, 11, 1, 12, 1, 
	13, 1, 14, 1, 15, 1, 16, 1, 
	17, 1, 18, 1, 19, 1, 20, 1, 
	21, 1, 22, 1, 23, 1, 24, 1, 
	25, 1, 26, 1, 27, 1, 28, 1, 
	29, 1, 30, 1, 31, 1, 32, 1, 
	33, 1, 34, 1, 35, 1, 36, 1, 
	37, 1, 38, 1, 39, 1, 40, 1, 
	41, 1, 42, 1, 43, 1, 44, 1, 
	45, 2, 1, 25, 2, 4, 9, 2, 
	5, 9, 2, 6, 9, 2, 6, 46, 
	2, 9, 2, 2, 9, 3
};

static const unsigned char _command_key_offsets[] = {
	0, 0, 27, 28, 32, 36, 40, 44, 
	45, 49, 53, 57, 61, 63, 65, 67, 
	69, 71, 71, 71, 73, 73, 77, 81, 
	82, 83, 84, 85, 86, 90, 93, 93, 
	94, 95, 96, 98, 100, 100, 100, 101, 
	102, 104, 106, 108, 110, 112, 112, 114, 
	116, 118, 122, 126, 130, 134, 138, 142, 
	146, 150, 151, 155, 159, 163, 167, 171, 
	175, 179, 183, 183
};

static const char _command_trans_keys[] = {
	63, 66, 69, 71, 72, 74, 75, 76, 
	77, 80, 82, 83, 84, 86, 87, 88, 
	90, 98, 101, 104, 109, 114, 115, 116, 
	118, 119, 122, 69, 48, 57, 65, 70, 
	48, 57, 65, 70, 48, 57, 65, 70, 
	48, 57, 65, 70, 44, 48, 57, 65, 
	70, 48, 57, 65, 70, 48, 57, 65, 
	70, 48, 57, 65, 70, 0, 23, 0, 
	59, 0, 59, 1, 12, 1, 31, 0, 
	1, 1, 2, 3, 4, -80, -78, 16, 
	17, -2, 0, 0, 0, 2, -80, -78, 
	16, 17, -2, 36, 37, 0, 0, 0, 
	16, 17, 6, 7, 0, 0, 0, 3, 
	0, 90, 0, 59, 0, 59, 0, 1, 
	0, 59, 0, 59, 0, 1, 48, 57, 
	65, 70, 48, 57, 65, 70, 48, 57, 
	65, 70, 48, 57, 65, 70, 48, 57, 
	65, 70, 48, 57, 65, 70, 48, 57, 
	65, 70, 48, 57, 65, 70, 44, 48, 
	57, 65, 70, 48, 57, 65, 70, 48, 
	57, 65, 70, 48, 57, 65, 70, 48, 
	57, 65, 70, 48, 57, 65, 70, 48, 
	57, 65, 70, 48, 57, 65, 70, 48, 
	57, 65, 70, 0
};

static const char _command_single_lengths[] = {
	0, 27, 1, 0, 0, 0, 0, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 2, 2, 1, 
	1, 1, 1, 1, 2, 1, 0, 1, 
	1, 1, 0, 0, 0, 0, 1, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0
};

static const char _command_range_lengths[] = {
	0, 0, 0, 2, 2, 2, 2, 0, 
	2, 2, 2, 2, 1, 1, 1, 1, 
	1, 0, 0, 1, 0, 1, 1, 0, 
	0, 0, 0, 0, 1, 1, 0, 0, 
	0, 0, 1, 1, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 0, 1, 1, 
	1, 2, 2, 2, 2, 2, 2, 2, 
	2, 0, 2, 2, 2, 2, 2, 2, 
	2, 2, 0, 2
};

static const unsigned char _command_index_offsets[] = {
	0, 0, 28, 30, 33, 36, 39, 42, 
	44, 47, 50, 53, 56, 58, 60, 62, 
	64, 66, 67, 68, 70, 71, 75, 79, 
	81, 83, 85, 87, 89, 93, 96, 97, 
	99, 101, 103, 105, 107, 108, 109, 111, 
	113, 115, 117, 119, 121, 123, 124, 126, 
	128, 130, 133, 136, 139, 142, 145, 148, 
	151, 154, 156, 159, 162, 165, 168, 171, 
	174, 177, 180, 181
};

static const char _command_trans_targs[] = {
	2, 3, 66, 66, 12, 66, 20, 66, 
	66, 21, 3, 67, 40, 66, 41, 66, 
	66, 49, 66, 66, 66, 49, 49, 66, 
	66, 66, 66, 0, 66, 0, 4, 4, 
	0, 5, 5, 0, 6, 6, 0, 7, 
	7, 0, 8, 0, 9, 9, 0, 10, 
	10, 0, 11, 11, 0, 66, 66, 0, 
	13, 0, 14, 0, 15, 0, 16, 0, 
	17, 0, 18, 19, 66, 0, 66, 22, 
	28, 34, 0, 23, 23, 23, 0, 24, 
	0, 25, 0, 26, 0, 27, 0, 66, 
	0, 23, 23, 29, 0, 24, 30, 0, 
	31, 32, 0, 33, 0, 66, 0, 35, 
	0, 36, 0, 37, 38, 39, 0, 66, 
	0, 66, 0, 42, 0, 43, 0, 44, 
	0, 45, 0, 46, 47, 0, 48, 0, 
	66, 0, 50, 50, 0, 51, 51, 0, 
	52, 52, 0, 53, 53, 0, 54, 54, 
	0, 55, 55, 0, 56, 56, 0, 57, 
	57, 0, 58, 0, 59, 59, 0, 60, 
	60, 0, 61, 61, 0, 62, 62, 0, 
	63, 63, 0, 64, 64, 0, 65, 65, 
	0, 66, 66, 0, 0, 4, 4, 0, 
	0
};

static const char _command_trans_actions[] = {
	0, 11, 5, 83, 0, 35, 0, 37, 
	39, 0, 9, 101, 0, 31, 0, 85, 
	7, 95, 104, 87, 33, 92, 98, 13, 
	31, 81, 107, 0, 5, 0, 1, 1, 
	0, 1, 1, 0, 1, 1, 0, 1, 
	1, 0, 0, 0, 3, 3, 0, 3, 
	3, 0, 3, 3, 0, 89, 89, 0, 
	65, 0, 67, 0, 69, 0, 71, 0, 
	73, 0, 75, 77, 79, 0, 45, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 41, 
	0, 0, 0, 25, 0, 0, 23, 0, 
	21, 0, 0, 0, 0, 27, 0, 25, 
	0, 23, 0, 17, 19, 0, 0, 29, 
	0, 15, 0, 49, 0, 51, 0, 53, 
	0, 55, 0, 57, 59, 0, 61, 0, 
	63, 0, 1, 1, 0, 1, 1, 0, 
	1, 1, 0, 1, 1, 0, 1, 1, 
	0, 1, 1, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 3, 3, 0, 3, 
	3, 0, 3, 3, 0, 3, 3, 0, 
	3, 3, 0, 3, 3, 0, 0, 0, 
	0, 47, 47, 0, 0, 1, 1, 0, 
	0
};

static const char _command_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 43, 43
};

static const int command_start = 1;
static const int command_first_final = 66;
static const int command_error = 0;

static const int command_en_main = 1;


/* #line 52 "nexstar.rl" */




/* #line 292 "nexstar.rl" */




void nexstar_init(void)
{
    
/* #line 166 "nexstar.c" */
	{
	cs = command_start;
	}

/* #line 299 "nexstar.rl" */
}
void nexstar_cmd( char *str,int len )
{
    x=y=0;
    resp_size=0;rad_to_cel=RAD_TO_CEL;
    char *p = str, *pe = str + len;
    char *eof=pe;
    
/* #line 169 "nexstar.c" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _command_trans_keys + _command_key_offsets[cs];
	_trans = _command_index_offsets[cs];

	_klen = _command_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _command_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _command_trans_targs[_trans];

	if ( _command_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _command_actions + _command_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 57 "nexstar.rl" */
	{ ADD_DIGIT(x,(*p));}
	break;
	case 1:
/* #line 58 "nexstar.rl" */
	{ADD_DIGIT(y,(*p));}
	break;
	case 2:
/* #line 62 "nexstar.rl" */
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
	break;
	case 3:
/* #line 75 "nexstar.rl" */
	{
        az=st_current.az*rad_to_cel;
        alt=st_current.alt*rad_to_cel;
        if (prec)
            sprintf(response,"%06lX00,%06lX00#",az,alt);
        else
            sprintf(response,"%04lX,%04lX#",az,alt);
        prec=0;
    }
	break;
	case 4:
/* #line 84 "nexstar.rl" */
	{
        px=&ra;
        py=&dec;
        sync_cmd=0;
    }
	break;
	case 5:
/* #line 89 "nexstar.rl" */
	{
        px=&alt;
        py=&az;
    }
	break;
	case 6:
/* #line 93 "nexstar.rl" */
	{
        px=&ra;
        py=&dec;
        sync_cmd=1;
    }
	break;
	case 7:
/* #line 99 "nexstar.rl" */
	{
        sprintf(response,"%c#",trackmode);
         resp_size=2;

    }
	break;
	case 8:
/* #line 105 "nexstar.rl" */
	{
        trackmode=(*p);
        if (trackmode)
          {st_target.ra=st_current.ra;
          st_target.dec=st_current.dec;}
        sprintf(response,"#");


    }
	break;
	case 9:
/* #line 115 "nexstar.rl" */
	{prec=1;rad_to_cel=RAD_TO_CEL_P;}
	break;
	case 10:
/* #line 117 "nexstar.rl" */
	{vrate+=(*p)*256;}
	break;
	case 11:
/* #line 119 "nexstar.rl" */
	{vrate+=(*p);}
	break;
	case 12:
/* #line 121 "nexstar.rl" */
	{frate=(*p);}
	break;
	case 13:
/* #line 123 "nexstar.rl" */
	{inv_dir=1; if (((*p)==7)||((*p)==37)) inv_dir=-1;}
	break;
	case 14:
/* #line 124 "nexstar.rl" */
	{alt_axis=((*p)==17);}
	break;
	case 15:
/* #line 125 "nexstar.rl" */
	{
        if (alt_axis) alt_frate=frate*frate*200.0*inv_dir ;else az_frate=frate*frate*200.0*-(inv_dir);
         trackmode=0;
       sprintf(response,"#");


    //printf("[set_fixed_rate]");
    }
	break;
	case 16:
/* #line 133 "nexstar.rl" */
	{sprintf(response,"#");
     if (alt_axis) alt_frate=vrate*inv_dir ;else az_frate=vrate*-(inv_dir);
    sprintf(response,"#");

    }
	break;
	case 17:
/* #line 140 "nexstar.rl" */
	{
        sprintf(response,"%c%c#",4,10);

    }
	break;
	case 18:
/* #line 145 "nexstar.rl" */
	{
        sprintf(response,"%c#",1);
         resp_size=2;

    }
	break;
	case 19:
/* #line 152 "nexstar.rl" */
	{
        sprintf(response,"%c#",is_aligned);
        resp_size=2;

    }
	break;
	case 20:
/* #line 158 "nexstar.rl" */
	{
        sprintf(response,"%c#",is_slewing);
        resp_size=2;
    }
	break;
	case 21:
/* #line 164 "nexstar.rl" */
	{
        sprintf(response,"#");

    }
	break;
	case 22:
/* #line 169 "nexstar.rl" */
	{sprintf(response,"%c%c#",4,1);resp_size=3;}
	break;
	case 24:
/* #line 171 "nexstar.rl" */
	{sprintf(response,"%c#",(*p));}
	break;
	case 25:
/* #line 174 "nexstar.rl" */
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
	break;
	case 26:
/* #line 190 "nexstar.rl" */
	{A=(*p);}
	break;
	case 27:
/* #line 191 "nexstar.rl" */
	{B=(*p);}
	break;
	case 28:
/* #line 192 "nexstar.rl" */
	{C=(*p);}
	break;
	case 29:
/* #line 193 "nexstar.rl" */
	{D=(*p);}
	break;
	case 30:
/* #line 194 "nexstar.rl" */
	{E=(*p);}
	break;
	case 31:
/* #line 195 "nexstar.rl" */
	{F=(*p);}
	break;
	case 32:
/* #line 196 "nexstar.rl" */
	{G=(*p);}
	break;
	case 33:
/* #line 197 "nexstar.rl" */
	{H=(*p);
                   Set_Coord();
                   sprintf(response,"#");
              }
	break;
	case 34:
/* #line 201 "nexstar.rl" */
	{hour=(*p);}
	break;
	case 35:
/* #line 202 "nexstar.rl" */
	{min=(*p);}
	break;
	case 36:
/* #line 203 "nexstar.rl" */
	{sec=(*p);}
	break;
	case 37:
/* #line 204 "nexstar.rl" */
	{month=(*p);}
	break;
	case 38:
/* #line 205 "nexstar.rl" */
	{day=(*p);}
	break;
	case 39:
/* #line 206 "nexstar.rl" */
	{year=(*p);}
	break;
	case 40:
/* #line 207 "nexstar.rl" */
	{tzone=(*p);}
	break;
	case 41:
/* #line 208 "nexstar.rl" */
	{
        dst=(*p);
         SetTime( 100+year, month-1,day,hour,min,sec);
         sprintf(response,"#");
        }
	break;
	case 42:
/* #line 214 "nexstar.rl" */
	{Get_Coord();}
	break;
	case 43:
/* #line 215 "nexstar.rl" */
	{tracedata=!tracedata;}
	break;
	case 44:
/* #line 216 "nexstar.rl" */
	{// printf ("%d %d %d %d %d %d\r\n",year-100, month,day,hour,min,sec);
     //SetTime( year, month,day,hour,min,sec);
     // printf("%d %d %d %d %d %d %d %d %f#",A,B,C,D,E,F,G,H,longitude);
     sideral(longitude);
    }
	break;
	case 45:
/* #line 221 "nexstar.rl" */
	{
        time_t now = rtc_get_counter_val();
        struct tm * Time = localtime(&now);
        sprintf(response,"%c%c%c%c%c%c%c%c#",Time->tm_hour, Time->tm_min,Time->tm_sec,Time->tm_mon,Time->tm_mday,Time->tm_year,2,Time->tm_isdst);
        resp_size=9;
    }
	break;
	case 46:
/* #line 227 "nexstar.rl" */
	{sideral(-4.20);

    }
	break;
/* #line 482 "nexstar.c" */
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _command_actions + _command_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 23:
/* #line 170 "nexstar.rl" */
	{if (resp_size==0) resp_size=strlen(response) ;}
	break;
/* #line 500 "nexstar.c" */
		}
	}
	}

	_out: {}
	}

/* #line 307 "nexstar.rl" */

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
