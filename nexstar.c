
/* #line 1 "nexstar.rl" */
#include <string.h>
#include <stdio.h>
#include "taki.h"
#include "nexstar.h"
//#include "motor.h"
#ifndef M_PI
#define M_PI 3.14159265359
//#define M_PI acos(-1.0)
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
//char response[40];
//int resp_size;
char A=36;
char B=12;
char C=4;
char D=0;
char E=4;
char F=12;
char G=0;
char H=1;
//extern c_star st_target,st_next,st_current;
//extern int tracedata;
/*extern double res_x,res_y;
extern long counter_x,counter_y;*/

/* #line 2 "nexstar.c" */
static const char _command_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 10, 1, 11, 1, 
	12, 1, 13, 1, 14, 1, 15, 1, 
	16, 1, 17, 1, 18, 1, 19, 1, 
	20, 1, 21, 1, 22, 1, 23, 1, 
	24, 1, 25, 1, 26, 1, 27, 2, 
	1, 25, 2, 4, 9, 2, 5, 9, 
	2, 6, 9, 2, 9, 2, 2, 9, 
	3
};

static const unsigned char _command_key_offsets[] = {
	0, 0, 26, 27, 31, 35, 39, 43, 
	44, 48, 52, 56, 60, 62, 64, 66, 
	68, 70, 70, 70, 72, 72, 75, 77, 
	79, 79, 79, 80, 81, 85, 86, 87, 
	88, 89, 90, 92, 94, 94, 95, 96, 
	97, 99, 101, 103, 105, 107, 107, 109, 
	111, 115, 119, 123, 127, 131, 135, 139, 
	143, 144, 148, 152, 156, 160, 164, 168, 
	172, 176
};

static const char _command_trans_keys[] = {
	63, 66, 69, 71, 72, 74, 75, 76, 
	77, 80, 82, 83, 84, 86, 87, 90, 
	98, 101, 104, 109, 114, 115, 116, 118, 
	119, 122, 69, 48, 57, 65, 70, 48, 
	57, 65, 70, 48, 57, 65, 70, 48, 
	57, 65, 70, 44, 48, 57, 65, 70, 
	48, 57, 65, 70, 48, 57, 65, 70, 
	48, 57, 65, 70, 0, 23, 0, 59, 
	0, 59, 1, 12, 1, 31, 0, 1, 
	1, 2, 3, 16, 17, 6, 7, 0, 
	0, -80, -78, 16, 17, -2, 0, 0, 
	0, 2, 16, 17, 36, 37, 0, 0, 
	0, 0, 3, 0, 90, 0, 59, 0, 
	59, 0, 1, 0, 59, 0, 59, 48, 
	57, 65, 70, 48, 57, 65, 70, 48, 
	57, 65, 70, 48, 57, 65, 70, 48, 
	57, 65, 70, 48, 57, 65, 70, 48, 
	57, 65, 70, 48, 57, 65, 70, 44, 
	48, 57, 65, 70, 48, 57, 65, 70, 
	48, 57, 65, 70, 48, 57, 65, 70, 
	48, 57, 65, 70, 48, 57, 65, 70, 
	48, 57, 65, 70, 48, 57, 65, 70, 
	0
};

static const char _command_single_lengths[] = {
	0, 26, 1, 0, 0, 0, 0, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 3, 2, 2, 
	0, 0, 1, 1, 2, 1, 1, 1, 
	1, 1, 2, 2, 0, 1, 1, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0
};

static const char _command_range_lengths[] = {
	0, 0, 0, 2, 2, 2, 2, 0, 
	2, 2, 2, 2, 1, 1, 1, 1, 
	1, 0, 0, 1, 0, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 0, 1, 1, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	0, 2, 2, 2, 2, 2, 2, 2, 
	2, 0
};

static const unsigned char _command_index_offsets[] = {
	0, 0, 27, 29, 32, 35, 38, 41, 
	43, 46, 49, 52, 55, 57, 59, 61, 
	63, 65, 66, 67, 69, 70, 74, 77, 
	80, 81, 82, 84, 86, 90, 92, 94, 
	96, 98, 100, 103, 106, 107, 109, 111, 
	113, 115, 117, 119, 121, 123, 124, 126, 
	128, 131, 134, 137, 140, 143, 146, 149, 
	152, 154, 157, 160, 163, 166, 169, 172, 
	175, 178
};

static const char _command_trans_targs[] = {
	2, 3, 65, 65, 12, 65, 20, 65, 
	65, 21, 3, 3, 40, 65, 41, 65, 
	48, 65, 65, 65, 48, 48, 65, 65, 
	65, 65, 0, 65, 0, 4, 4, 0, 
	5, 5, 0, 6, 6, 0, 7, 7, 
	0, 8, 0, 9, 9, 0, 10, 10, 
	0, 11, 11, 0, 65, 65, 0, 13, 
	0, 14, 0, 15, 0, 16, 0, 17, 
	0, 18, 19, 65, 0, 65, 22, 28, 
	34, 0, 23, 23, 0, 24, 24, 0, 
	25, 26, 27, 0, 65, 0, 29, 29, 
	29, 0, 30, 0, 31, 0, 32, 0, 
	33, 0, 65, 0, 35, 35, 0, 36, 
	36, 0, 37, 38, 0, 39, 0, 65, 
	0, 65, 0, 42, 0, 43, 0, 44, 
	0, 45, 0, 46, 47, 0, 19, 0, 
	49, 49, 0, 50, 50, 0, 51, 51, 
	0, 52, 52, 0, 53, 53, 0, 54, 
	54, 0, 55, 55, 0, 56, 56, 0, 
	57, 0, 58, 58, 0, 59, 59, 0, 
	60, 60, 0, 61, 61, 0, 62, 62, 
	0, 63, 63, 0, 64, 64, 0, 65, 
	65, 0, 0, 0
};

static const char _command_trans_actions[] = {
	0, 11, 5, 53, 0, 37, 0, 39, 
	41, 0, 9, 13, 0, 33, 0, 7, 
	61, 67, 0, 35, 58, 64, 15, 33, 
	51, 70, 0, 5, 0, 1, 1, 0, 
	1, 1, 0, 1, 1, 0, 1, 1, 
	0, 0, 0, 3, 3, 0, 3, 3, 
	0, 3, 3, 0, 55, 55, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 47, 0, 0, 
	0, 0, 0, 27, 0, 0, 25, 0, 
	19, 21, 0, 0, 31, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 43, 0, 0, 27, 0, 0, 
	25, 0, 23, 0, 0, 0, 0, 29, 
	0, 17, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 0, 1, 1, 0, 1, 1, 
	0, 1, 1, 0, 1, 1, 0, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 3, 3, 0, 3, 3, 0, 
	3, 3, 0, 3, 3, 0, 3, 3, 
	0, 3, 3, 0, 0, 0, 0, 49, 
	49, 0, 0, 0
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
	0, 45
};

static const int command_start = 1;
static const int command_first_final = 65;
static const int command_error = 0;

static const int command_en_main = 1;


/* #line 52 "nexstar.rl" */




/* #line 233 "nexstar.rl" */


void nexstar_init(void)
{
    
/* #line 159 "nexstar.c" */
	{
	cs = command_start;
	}

/* #line 238 "nexstar.rl" */
}
void nexstar_cmd( char *str,int len )
{
    x=y=0;
    resp_size=0;rad_to_cel=RAD_TO_CEL;
    char *p = str, *pe = str + len;
    char *eof=pe;
    
/* #line 162 "nexstar.c" */
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
        if (st_current.dec>=0.0) dec=st_current.dec*rad_to_cel;
        else dec= ((2.0*M_PI) +st_current.dec)*rad_to_cel;
        if (prec)
            sprintf(response,"%06lx00,%06lx00#",ra,dec);

        else {
            sprintf(response,"%04lx,%04lx#",ra,dec);
        }
        prec=0;
    }
	break;
	case 3:
/* #line 75 "nexstar.rl" */
	{
        if (prec)
            sprintf(response,"%08lx,%08lx#",az,alt);
        else
            sprintf(response,"%04lx,%04lx#",az,alt);
        prec=0;
    }
	break;
	case 4:
/* #line 82 "nexstar.rl" */
	{
        px=&ra;
        py=&dec;
        sync_cmd=0;
    }
	break;
	case 5:
/* #line 87 "nexstar.rl" */
	{
        px=&alt;
        py=&az;
    }
	break;
	case 6:
/* #line 91 "nexstar.rl" */
	{
        px=&ra;
        py=&dec;
        sync_cmd=1;
    }
	break;
	case 7:
/* #line 97 "nexstar.rl" */
	{
        sprintf(response,"%c#",trackmode);

    }
	break;
	case 8:
/* #line 102 "nexstar.rl" */
	{
        trackmode=(*p);
        sprintf(response,"#");

    }
	break;
	case 9:
/* #line 108 "nexstar.rl" */
	{prec=1;rad_to_cel=RAD_TO_CEL_P;}
	break;
	case 10:
/* #line 110 "nexstar.rl" */
	{vrate+=(*p)*256;}
	break;
	case 11:
/* #line 112 "nexstar.rl" */
	{vrate+=(*p);}
	break;
	case 12:
/* #line 114 "nexstar.rl" */
	{frate=(*p);}
	break;
	case 13:
/* #line 116 "nexstar.rl" */
	{inv_dir=1;}
	break;
	case 14:
/* #line 117 "nexstar.rl" */
	{alt_axis=1;}
	break;
	case 15:
/* #line 118 "nexstar.rl" */
	{sprintf(response,"#");
    //printf("[set_fixed_rate]");
    }
	break;
	case 16:
/* #line 121 "nexstar.rl" */
	{sprintf(response,"#");

    }
	break;
	case 17:
/* #line 126 "nexstar.rl" */
	{
        sprintf(response,"%c%c#",4,10);
        //printf("get_version\r\n");
    }
	break;
	case 18:
/* #line 131 "nexstar.rl" */
	{
        sprintf(response,"%c#",1);
        //printf("getmodel\r\n");
    }
	break;
	case 19:
/* #line 137 "nexstar.rl" */
	{
        sprintf(response,"%c#",1);
      //  printf("is_aligned");
    }
	break;
	case 20:
/* #line 142 "nexstar.rl" */
	{sprintf(response,"is_slewing\r\n");}
	break;
	case 21:
/* #line 145 "nexstar.rl" */
	{
        sprintf(response,"#");
        //printf("Cancel_goto\r\n");
    }
	break;
	case 22:
/* #line 150 "nexstar.rl" */
	{sprintf(response,"%c%c#",0,0);resp_size=3;}
	break;
	case 24:
/* #line 152 "nexstar.rl" */
	{sprintf(response,"%c#",(*p));}
	break;
	case 25:
/* #line 155 "nexstar.rl" */
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
/* #line 171 "nexstar.rl" */
	{sprintf(response,"%c%c%c%c%c%c%c%c#",A,B,C,D,E,F,G,H);resp_size=9;}
	break;
	case 27:
/* #line 172 "nexstar.rl" */
	{tracedata=!tracedata;}
	break;
/* #line 381 "nexstar.c" */
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
/* #line 151 "nexstar.rl" */
	{if (resp_size==0) resp_size=strlen(response) ;}
	break;
/* #line 399 "nexstar.c" */
		}
	}
	}

	_out: {}
	}

/* #line 246 "nexstar.rl" */

};

void checkfsm(void)
{
    if ( cs < command_first_final )
        printf("FAIL");
    else
        printf("ACCEPT");
}



