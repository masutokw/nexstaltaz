#ifndef MOUNT_H_INCLUDED
#define MOUNT_H_INCLUDED
#define ALT_PORT GPIOC
#define AZ_PORT GPIOC
#define ALT_PIN GPIO14
#define AZ_PIN GPIO13
#include "taki.h"
#include "usb_cdc.h"
#include "nexstar.h"
#include <time.h>

c_star st_now,st_target,st_next;
c_double res_az,res_alt,longitude,latitude;
int elevation,pressure;
long counter_az,counter_alt;
char trackmode,is_aligned,is_slewing;
c_double d_az_r,d_alt_r;

struct tm * TimeSet;

void track(void);
void nexstar_poll(void);
void track(void);
void trace_frame(void);
void ClockShow(void);
void SetTime(int y,int m,int d,int h,int min,int sec);

#endif // MOUNT_H_INCLUDED
