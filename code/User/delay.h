#ifndef PUBLIC_DELAY_H
#define PUBLIC_DELAY_H

#include <reg52.h>

//#define MAIN_Fosc        11059200UL //定义主时钟HZ 
#define MAIN_Fosc        12000000UL //定义主时钟HZ

void delay_ms(unsigned int times);

#endif


