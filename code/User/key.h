#ifndef _KEY_H_
#define _KEY_H_

#include <reg52.h>
#include "delay.h"

#define MATRIX_KEYBOARD_DATA P1            /* 矩阵键盘引脚 */
#define USE_PROTEUS_KEYPAD   1             /* 是否使用proteus KEYPAD键盘，1为使用 */ 

unsigned char matrix_keyboard_pro(void);
unsigned char matrix_keyboard_scan(void);  /* 矩阵键盘扫描函数 */

#endif
