#include "LCD1602.h"
#include "key.h"
#include <stdlib.h>

#define LENGTH 16
unsigned char show_calcu_str[LENGTH] = {0};
unsigned char show_result_str[LENGTH] = {0};

bit NOC_flag = 0;      /* �����һ�μ���ʽ */
bit error_flag = 0;    /* �����־λ */
bit ready_flag = 0;    /* ��ʾ��� */
bit equal_flag = 0;    /* ���Լ��� */

unsigned char len = 0;
unsigned char equal_bit = 0;
unsigned char operate_bit = 0;
unsigned char cal_count = 0;  //�����������

unsigned char cal_mode = 0; /* ���㷽�� */
unsigned char num_id = 0;   /* ȷ���ǵ�һ�������ǵڶ����� */
float num_1 = 0, num_2 = 0; /* �������� */
float result = 0;           /* ������� */

void calculator(unsigned char val);

void main()
{
	unsigned char key_val = 0xff;

	LCD1602_init();
	delay_ms(20);

	//��ӭ���� welcome + student id
	LCD1602_show_str(0, 0, "    Welcome!");
	LCD1602_show_str(0, 1, "   0000000000");
	delay_ms(2000);
	LCD1602_write_cmd(LCD_SCREEN_CLR); //��Ļ����
	delay_ms(20);

	while (1) {
		//��ȡ��ֵ����ʾ
		key_val = matrix_keyboard_scan();
		if (key_val != 0xff) {
			calculator(key_val);
			LCD1602_show_str(0, 0, show_calcu_str);
		}

		//�����������0����ʾ����INF
		if (error_flag) {
			error_flag = 0;
			LCD1602_show_str(0, 1, "INF");
		}
		
		//������ʾ���
		if (ready_flag) {
			ready_flag = 0;
			NOC_flag = 1;			
			sprintf(show_result_str, "%.3f", result);
			LCD1602_show_str(0, 1, show_result_str);
		}

	}
}

void calculator(unsigned char val)
{
	unsigned char tmep_str_1[LENGTH] = {0}; //������
	unsigned char tmep_str_2[LENGTH] = {0}; //������
	unsigned char i = 0, j = 0; //�м�ѭ������

	//����������ģʽ�£����ٴ�����ʱ���������ʾ
	if (NOC_flag) {
		NOC_flag = 0;
		for (i = 0; i < LENGTH; i++) {
			show_calcu_str[i] = ' ';
		}
		len = 0;
		LCD1602_show_str(0, 0, "                ");
	}

	switch (val) {
	//���ְ�����ת��ΪASCII�봢������ʾ������
	case 1:show_calcu_str[len++] = 0 + '0';break;
	case 4:show_calcu_str[len++] = 1 + '0';break;
	case 5:show_calcu_str[len++] = 2 + '0';break;
	case 6:show_calcu_str[len++] = 3 + '0';break;
	case 8:show_calcu_str[len++] = 4 + '0';break;
	case 9:show_calcu_str[len++] = 5 + '0';break;
	case 10:show_calcu_str[len++] = 6 + '0';break;
	case 12:show_calcu_str[len++] = 7 + '0';break;
	case 13:show_calcu_str[len++] = 8 + '0';break;
	case 14:show_calcu_str[len++] = 9 + '0';break;
	
	//NOC����
	case 0:
		//��������������Σ�����Ļ���㣬���¼���
		if (show_calcu_str[len - 1] == '.') {
			NOC_flag = 1;
			cal_count = 0;
			len = 0;
			LCD1602_write_cmd(LCD_SCREEN_CLR);
			break;
		}
		show_calcu_str[len++] = '.';
		break;

	//'='����
	case 2:
		equal_flag = 1;
		equal_bit = len;
		show_calcu_str[len++] = '=';
		break;
	
	//'+'����
	case 3:
		cal_mode = 0;
		operate_bit = len;
		show_calcu_str[len++] = '+';
		break;

	//'-'����
	case 7:
		cal_mode = 1;
		operate_bit = len;
		show_calcu_str[len++] = '-';
		break;

	//'*'����
	case 11:
		cal_mode = 2;
		operate_bit = len;
		show_calcu_str[len++] = '*';
		break;
	
	//'/'����
	case 15:
		cal_mode = 3;
		operate_bit = len;
		show_calcu_str[len++] = '/';
		break;
	default:
		break;
	}

	//������
	if (equal_flag) {
		equal_flag = 0;
		len = 0; //��ո������泤��ֵ��Ϊ��һ�μ�����׼��

		//���ݴ���������������ݣ�����õ�����	
		if (cal_count != 0) {
			cal_count++;
			//��������ģʽ
			num_1 = result;
		} else {
			cal_count++; //����һ��
			for (i = 0; i < operate_bit; i++) {
				tmep_str_1[i] = show_calcu_str[i];
			}
			num_1 = atof(tmep_str_1);
		}

		j = 0;
		for (i = (operate_bit + 1); i < equal_bit; i++) {
			tmep_str_2[j++] = show_calcu_str[i];
		}
		num_2 = atof(tmep_str_2);
		
		//�������㷨��õ����
		switch (cal_mode) {
		case 0:
			result = num_1 + num_2;
			break;
		case 1:
			result = num_1 - num_2;
			break;
		case 2:
			result = num_1 * num_2;
			break;
		case 3:
			if (num_2 == 0.0f) {
				error_flag = 1;
				break;
			}
			result = num_1 / num_2;
			break;
		default:
			break;
		}

		//���û�д�����������ʾ���
		if (!error_flag) {
			ready_flag = 1;
		}
	}
}
