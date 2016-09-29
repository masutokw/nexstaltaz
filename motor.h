#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED
long counter_x,counter_y;
unsigned int ticks_x,ticks_y;
int dir_x,dir_y;
double speed_x,speed_y,res_x,res_y,current_speedx,accel_x,accel_y;



void motor_init(long x,long y,double rx,double ry,double ax ,double ay);
void speedx(float speed);
void speedy(float speed);
void speed_x_a(float target_speed);
void speed_y_a(float target_speed);
#endif
