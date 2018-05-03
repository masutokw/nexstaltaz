#ifndef MOUNT_H_INCLUDED
#define MOUNT_H_INCLUDED
#define ALT_PORT GPIOC
#define AZ_PORT GPIOC
#define ALT_PIN GPIO14
#define ALT_DIR_PIN GPIO13
#define AZ_PIN GPIO15
#define AZ_DIR_PIN GPIO0
#define AZ_DIR_PIN_PORT GPIOA
#include "taki.h"
#include "usb_cdc.h"
#include "nexstar.h"
#include <time.h>
#include <libopencm3/stm32/rtc.h>
#include "mount.h"

typedef enum {ALTAZ,EQ,ALT_ALT} mount_t;

c_star st_now,st_target,st_next;
c_double res_az,res_alt,longitude,latitude;
int elevation,pressure;
long counter_az,counter_alt;
char trackmode,is_aligned,is_slewing,tracking;

c_double d_az_r,d_alt_r;

mount_t mount_mode;
struct tm * TimeSet;
void  tak_init(mount_t mount,c_double longd, c_double latd);
void track(void);
void nexstar_poll(void);
void track(void);
void trace_frame(void);
void ClockShow(void);
void SetTime(char y,char m,char d,char h,char min,char sec);
double sideral(double lon);

#endif // MOUNT_H_INCLUDED
