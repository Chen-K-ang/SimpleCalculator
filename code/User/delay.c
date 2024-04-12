#include "delay.h"

//延时n毫秒
void delay_ms(unsigned int times)
{
    unsigned int i=0,j=0;
    for(i=0;i<times;i++)
        for(j=0;j<123;j++);
}
