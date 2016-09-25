#include "motor.h"
#include <math.h>
#define sign(a) ( ( (a) < 0 )  ?  -1   : ( (a) > 0 ) )
long counter_x,counter_y;
unsigned int ticks_x,ticks_y;
int dir_x,dir_y;
float speed_x,speed_y,res_x,res_y;
void motor_init(long x,long y,float rx,float ry)
{
    counter_x=x;
    counter_y=y;
    res_x=rx;
    res_y=ry ;



}
void speedx(float speed)
{  if (fabs(speed)>15000) speed=15000*sign(speed);
    ticks_x=round((100000.0*res_x)/fabs(speed));

    dir_x=sign(speed);

}

void speedy(float speed)
{if (fabs(speed)>15000) speed=15000*sign(speed);
    ticks_y=round((100000.0*res_y)/fabs(speed));
    dir_y=sign(speed);
}
