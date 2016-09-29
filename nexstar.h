#ifndef NEXSTAR_H_INCLUDED
#define NEXSTAR_H_INCLUDED
#include "motor.h"
c_star st_target,st_next,st_current;
int tracedata;
char response[40];
int resp_size;
void nexstar_init();
void nexstar_cmd( char *str,int len );
void checkfsm(void);
#endif
