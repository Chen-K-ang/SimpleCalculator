#include "key.h"

/****
*******����ɨ�躯����ʹ�����з�תɨ�跨
*******����ֵ����ֵ
*****/
#if USE_PROTEUS_KEYPAD
unsigned char matrix_keyboard_pro(void)
{
	unsigned char cord_h, cord_l;          /* ����ֵ�м���� */
	unsigned char temp = 0x00;

	MATRIX_KEYBOARD_DATA = 0x0f;           /* �������ȫΪ0 */

	/* �ȼ�����ް������� */
	if ((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f) {
		delay_ms(10);  /* ȥ�� */
		if ((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f) {
			cord_h = MATRIX_KEYBOARD_DATA & 0x0f;        /* ��������ֵ */
			
			//��������4λ��λ���0��ֱ���ҵ����¼�����ֵ
			for(temp = 0x80; temp != 0x08; temp >>= 1) {
				MATRIX_KEYBOARD_DATA = ~temp;        /* �����ǰ����ֵ */
				cord_l = MATRIX_KEYBOARD_DATA & 0x0f;
				if (cord_l != 0x0f) {
					cord_l = 0xf0 & (~temp);     /* ��������ֵ */
					while((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f); /* �ȴ��ɿ������ */
					return(cord_h + cord_l);     /* ���������ֵ */
				}
			}
		}
	}
	return(0xff);  /* û�а������£�����0xff */
}
#else
unsigned char matrix_keyboard_pro(void)
{
	unsigned char cord_h, cord_l;          /* ����ֵ�м���� */
	MATRIX_KEYBOARD_DATA = 0x0f;           /* �������ȫΪ0 */
//	cord_h = MATRIX_KEYBOARD_DATA & 0x0f;  /* ��������ֵ */

	/* �ȼ�����ް������� */
	if ((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f) {
		delay_ms(10);  /* ȥ�� */
		if ((MATRIX_KEYBOARD_DATA & 0x0f) != 0x0f) {
			cord_h = MATRIX_KEYBOARD_DATA & 0x0f;     /* ��������ֵ */
			MATRIX_KEYBOARD_DATA = cord_h | 0xf0;     /* �����ǰ����ֵ */
			cord_l = MATRIX_KEYBOARD_DATA & 0xf0;     /* ��������ֵ */

			while((MATRIX_KEYBOARD_DATA & 0xf0) != 0xf0); /* �ȴ��ɿ������ */
			return(cord_h + cord_l);                      /* ���������ֵ */
		}
	}
	return(0xff);  /* û�а������£�����0xff */
}
#endif

/****
*******������̼�ֵ����Ϊ��ֵ����
*******����ֵ����ֵ��ֵ
*****/
unsigned char matrix_keyboard_scan(void)
{
	switch (matrix_keyboard_pro()) {

	case (0xee):return 0; break;  /* K1���� */
	case (0xde):return 1; break;  /* K2���� */
	case (0xbe):return 2; break;  /* K3���� */
	case (0x7e):return 3; break;  /* K4���� */
	case (0xed):return 4; break;  /* K5���� */
	case (0xdd):return 5; break;  /* K6���� */
	case (0xbd):return 6; break;  /* K7���� */
	case (0x7d):return 7; break;  /* K8���� */
	case (0xeb):return 8; break;  /* K9���� */
	case (0xdb):return 9; break;  /* K10���� */
	case (0xbb):return 10;break;  /* K11���� */
	case (0x7b):return 11;break;  /* K12���� */
	case (0xe7):return 12;break;  /* K13���� */
	case (0xd7):return 13;break;  /* K14���� */
	case (0xb7):return 14;break;  /* K15���� */
	case (0x77):return 15;break;  /* K16���� */

	default:return 0xff;break;   /* û�а������� */
	}
}

