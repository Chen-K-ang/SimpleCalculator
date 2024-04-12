#include "key.h"

/****
*******键盘扫描函数，使用行列反转扫描法
*******返回值：键值
*****/
#if USE_PROTEUS_KEYPAD
unsigned char matrix_keyboard_pro(void)
{
	unsigned char cord_h, cord_l;          /* 行列值中间变量 */
	unsigned char temp = 0x00;

	MATRIX_KEYBOARD_DATA = 0x0f;           /* 列线输出全为0 */

	/* 先检测有无按键按下 */
	if ((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f) {
		delay_ms(10);  /* 去抖 */
		if ((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f) {
			cord_h = MATRIX_KEYBOARD_DATA & 0x0f;        /* 读入行线值 */
			
			//遍历，低4位逐位输出0，直到找到按下键的列值
			for(temp = 0x80; temp != 0x08; temp >>= 1) {
				MATRIX_KEYBOARD_DATA = ~temp;        /* 输出当前列线值 */
				cord_l = MATRIX_KEYBOARD_DATA & 0x0f;
				if (cord_l != 0x0f) {
					cord_l = 0xf0 & (~temp);     /* 读入列线值 */
					while((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f); /* 等待松开并输出 */
					return(cord_h + cord_l);     /* 返回组合码值 */
				}
			}
		}
	}
	return(0xff);  /* 没有按键按下，返回0xff */
}
#else
unsigned char matrix_keyboard_pro(void)
{
	unsigned char cord_h, cord_l;          /* 行列值中间变量 */
	MATRIX_KEYBOARD_DATA = 0x0f;           /* 列线输出全为0 */
//	cord_h = MATRIX_KEYBOARD_DATA & 0x0f;  /* 读入行线值 */

	/* 先检测有无按键按下 */
	if ((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f) {
		delay_ms(10);  /* 去抖 */
		if ((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f) {
			cord_h = MATRIX_KEYBOARD_DATA & 0x0f;     /* 读入行线值 */
			MATRIX_KEYBOARD_DATA = cord_h | 0xf0;     /* 输出当前列线值 */
			cord_l = MATRIX_KEYBOARD_DATA & 0xf0;     /* 读入列线值 */

			while((MATRIX_KEYBOARD_DATA & 0xf0) != 0xf0); /* 等待松开并输出 */
			return(cord_h + cord_l);                      /* 返回组合码值 */
		}
	}
	return(0xff);  /* 没有按键按下，返回0xff */
}
#endif

/****
*******矩阵键盘键值编码为数值函数
*******返回值：键值数值
*****/
unsigned char matrix_keyboard_scan(void)
{
	switch (matrix_keyboard_pro()) {

	case (0xee):return 0; break;  /* K1按下 */
	case (0xde):return 1; break;  /* K2按下 */
	case (0xbe):return 2; break;  /* K3按下 */
	case (0x7e):return 3; break;  /* K4按下 */
	case (0xed):return 4; break;  /* K5按下 */
	case (0xdd):return 5; break;  /* K6按下 */
	case (0xbd):return 6; break;  /* K7按下 */
	case (0x7d):return 7; break;  /* K8按下 */
	case (0xeb):return 8; break;  /* K9按下 */
	case (0xdb):return 9; break;  /* K10按下 */
	case (0xbb):return 10;break;  /* K11按下 */
	case (0x7b):return 11;break;  /* K12按下 */
	case (0xe7):return 12;break;  /* K13按下 */
	case (0xd7):return 13;break;  /* K14按下 */
	case (0xb7):return 14;break;  /* K15按下 */
	case (0x77):return 15;break;  /* K16按下 */

	default:return 0xff;break;   /* 没有按键按下 */
	}
}

