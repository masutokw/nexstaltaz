#include "motor.h"
#include <math.h>
#define TOP_SPD 15000
#define sign(a) ( ( (a) < 0 )  ?  -1   : ( (a) > 0 ) )
//double speed_x,speed_y,res_x,res_y,current_speedx,accel_x,accel_y;
void motor_init(long x,long y,double rx,double ry,double ax,double ay )
{
    counter_x=x;
    counter_y=y;
    res_x=rx;
    res_y=ry ;
    accel_x=ax;
    accel_y=ay;
}
void speedx(float speed)
{
    if (fabs(speed)>15000) speed=15000*sign(speed);
    ticks_x=round((100000.0*res_x)/fabs(speed));
    dir_x=sign(speed);
    speed_x=speed;
}

void speedy(float speed)
{
    if (fabs(speed)>15000) speed=15000*sign(speed);
    ticks_y=round((100000.0*res_y)/fabs(speed));
    dir_y=sign(speed);
    speed_y=speed;
}
void speed_x_a(float target_speed)
{
    if (fabs(speed_x)>TOP_SPD) target_speed=TOP_SPD*sign(speed_x);
    if (target_speed==speed_x) return;

    //speed_x=target_speed;
    if (speed_x<target_speed)
    {
        if ((speed_x+accel_x)>=target_speed) speed_x=target_speed;
        else
            speed_x=speed_x+accel_x;
    }

    else
    {
        if ((speed_x-accel_x)<=target_speed) speed_x=target_speed;
        else
            speed_x=speed_x-accel_x;
    }

    ticks_x=round((100000.0*res_x)/fabs(speed_x));
    dir_x=sign(speed_x);


}

void speed_y_a(float target_speed)
{
    if (target_speed==speed_y) return;
    if (fabs(speed_y)>TOP_SPD) target_speed=TOP_SPD0*sign(speed_y);
    //speed_y=target_speed;
    if (speed_y<target_speed)
    {
        if ((speed_y+accel_y)>target_speed) speed_y=target_speed;
        else
            speed_y=speed_y+accel_y;
    }

    else
    {
        if ((speed_y-accel_y)<target_speed) speed_y=target_speed;
        else
            speed_y=speed_y-accel_y;
    }

    ticks_y=round((100000.0*res_y)/fabs(speed_y));
    dir_y=sign(speed_y);

}

