#ifndef TAKI_H_INCLUDED
#define TAKI_H_INCLUDED
#include <math.h>
#include <stdio.h>
#ifndef M_PI
#define M_PI acos(-1.0)
#endif
#define RAD_TO_DEG (180.0/M_PI)//  57.2958
#define SOLAR_DS (3600.0*24.0)
#define K_SID 1.00273790935
#define M_2PI   (2.0*M_PI)
#define KTIME ((K_SID*M_2PI)/(SOLAR_DS))
#define RAD_TO_ARCS (180.0*3600.0/M_PI)
typedef  double  c_double ;
typedef struct
{
    c_double	ra ;
    c_double	dec ;
    c_double	alt ;
    c_double 	az ;
    c_double    timer_count ;
} c_star ;

void reset_transforms(c_double az1,c_double az2,c_double az3);
void init_star(unsigned char index,const c_star *star);
void compute_trasform(void);
void to_alt_az(c_star *star);
void to_equatorial(c_star *star);
void printm(void);
void set_star(c_star *st,c_double ra,c_double dec,c_double az,c_double alt,c_double ticks);

#endif

