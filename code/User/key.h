#ifndef _KEY_H_
#define _KEY_H_

#include <reg52.h>
#include "delay.h"

#define MATRIX_KEYBOARD_DATA P1            /* ����������� */
#define USE_PROTEUS_KEYPAD   1             /* �Ƿ�ʹ��proteus KEYPAD���̣�1Ϊʹ�� */ 

unsigned char matrix_keyboard_pro(void);
unsigned char matrix_keyboard_scan(void);  /* �������ɨ�躯�� */

#endif
