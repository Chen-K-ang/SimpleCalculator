#include "LCD1602.h"

//��ȡæ���
static void LCD1602_read_busy_flag(void)
{
	unsigned char state;
	unsigned char i;
	LCD1602_DB = 0xFF;   //IO�ڸ�λ��׼����ȡ��ƽ�仯
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do {
		LCD1602_EN = 1;
		state = LCD1602_DB;   //��ȡ��������
		LCD1602_EN = 0;
		i++;
		if (i > 50)           //��ֹæ���һֱ���е��³�����
			break;
	} while (state & 0x80);       //���DB7Ϊ�ߵ�ƽ
}

//д����
void LCD1602_write_cmd(unsigned char cmd)
{
	LCD1602_read_busy_flag(); //��ѯ״̬��lcd�Ƿ����
	LCD1602_RS = 0;    //����д����
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_EN = 1;    //������ɣ���λ
	LCD1602_EN = 0;
}

//д����
void LCD1602_write_data(unsigned char dat)
{
	LCD1602_read_busy_flag();
	LCD1602_RS = 1;    //����д����
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_EN = 1;    //������ɣ���λ
	LCD1602_EN = 0;
}

//lcd��ʼ��
void LCD1602_init(void)
{
	LCD1602_write_cmd(LCD_MODE_PIN8);
	LCD1602_write_cmd(LCD_DIS_ON);
	LCD1602_write_cmd(LCD_CURSOR_RIGHT);
	LCD1602_write_cmd(LCD_SCREEN_CLR);
}

//����lcd�ַ���ʾλ��
//���룺x��0~15   y 0~1
//˵����xΪlcd��ʾ��������y����0ʱΪ��һ�У�1Ϊ�ڶ���
static void LCD1602_set_pos(unsigned char x, unsigned char y)
{
	if (y) {
		LCD1602_write_cmd(LCD_MODE_FIR | (LCD_MODE_SEC + x));
	} else {
		LCD1602_write_cmd(LCD_MODE_FIR | x);
	}
}

//�ַ�����ʾ
void LCD1602_show_str(unsigned char x, unsigned char y, unsigned char *str)
{
	LCD1602_set_pos(x, y);
	while (*str != '\0') {               //��ѯָ��ָ���Ƿ�Ϊ�գ��������
		LCD1602_write_data(*str++);  //�ַ�������ָ���Լ�
	}
}

//�Զ����ַ��洢����ʾ
//˵�����Զ����ַ���Ҫ�Ƚ��й��첢���棬lcd1602�ſ��Ա�ʶ�����
//      pos����Ϊ�洢CGRAMλ��
void LCD1602_set_char(unsigned char x, unsigned char y, unsigned char pos, unsigned char *str)
{
	unsigned char i;
	//�����Զ����ַ�  ȷ��CGRAMλ��
	for (i = 0; i < 8; i++) {
		LCD1602_write_cmd(LCD_SET_CGRAM_ADDR + pos >> 3 + i);
		LCD1602_write_data(*str++);
	}
	LCD1602_set_pos(x, y);
	LCD1602_write_data(0x00 + pos);
}

//����ת�ַ���
unsigned char LCD1602_integer_to_str(int dat, unsigned char *str)
{
	unsigned char i = 0;
	unsigned char len = 0;
	unsigned char buf[6];  //int����Ϊ6λ  -32767~32768
	if (dat < 0) {         //datΪС���������ַ�����һλ��ӡ����š�
		dat = -dat;
		*str++ = '-';  //ָ���Լӣ�Ϊ��һ�����ݴ洢��׼��
		len++;
	}
	do {
		buf[i++] = dat % 10 + '0';  // +'0'��������ת��ASCII��
		dat /= 10;     //ȡ��
	} while (dat > 0);     //���һλȡ��ʱ����Ϊ0�������������ѭ��

	len += i;              //��¼һ���м�λ
	while (i-- > 0) {      //����ʱ�ַ������ݴ��str
		*str++ = buf[i];
	}
	*str = '\0';           //���һλ���ַ���������
	
	return (len);          //����������ȣ�����ϵ��ò�������У�顢�����
}

//����ת�ַ���
//p.s. ���ַ�0xA5ת���ַ��������lcd��ʾA5
void LCD1602_char_to_str(unsigned char dat, unsigned char *str)
{
	unsigned char temp = 0;
	temp = dat >> 4;     //�������λ
	if (temp <= 9) {
		*str++ = temp + '0';
	} else {
		*str++= temp + '7';  //��ASCII����ת��ABCDEF
	}
	
	temp = dat & 0x0F;  //�������λ
	if (temp <= 9) {
		*str++ = temp + '0';
	} else {
		*str++ = temp + '7';
	}
	*str = '\0';        //���һλ���ַ���������
}
