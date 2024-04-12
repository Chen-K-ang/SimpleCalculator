#include "LCD1602.h"

//读取忙检测
static void LCD1602_read_busy_flag(void)
{
	unsigned char state;
	unsigned char i;
	LCD1602_DB = 0xFF;   //IO口复位，准备读取电平变化
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do {
		LCD1602_EN = 1;
		state = LCD1602_DB;   //读取总线数据
		LCD1602_EN = 0;
		i++;
		if (i > 50)           //防止忙检测一直进行导致程序卡死
			break;
	} while (state & 0x80);       //如果DB7为高电平
}

//写命令
void LCD1602_write_cmd(unsigned char cmd)
{
	LCD1602_read_busy_flag(); //查询状态，lcd是否空闲
	LCD1602_RS = 0;    //进行写命令
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_EN = 1;    //操作完成，复位
	LCD1602_EN = 0;
}

//写数据
void LCD1602_write_data(unsigned char dat)
{
	LCD1602_read_busy_flag();
	LCD1602_RS = 1;    //进行写数据
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_EN = 1;    //操作完成，复位
	LCD1602_EN = 0;
}

//lcd初始化
void LCD1602_init(void)
{
	LCD1602_write_cmd(LCD_MODE_PIN8);
	LCD1602_write_cmd(LCD_DIS_ON);
	LCD1602_write_cmd(LCD_CURSOR_RIGHT);
	LCD1602_write_cmd(LCD_SCREEN_CLR);
}

//设置lcd字符显示位置
//输入：x：0~15   y 0~1
//说明：x为lcd显示的列数，y输入0时为第一行，1为第二行
static void LCD1602_set_pos(unsigned char x, unsigned char y)
{
	if (y) {
		LCD1602_write_cmd(LCD_MODE_FIR | (LCD_MODE_SEC + x));
	} else {
		LCD1602_write_cmd(LCD_MODE_FIR | x);
	}
}

//字符串显示
void LCD1602_show_str(unsigned char x, unsigned char y, unsigned char *str)
{
	LCD1602_set_pos(x, y);
	while (*str != '\0') {               //查询指针指向是否为空，空则结束
		LCD1602_write_data(*str++);  //字符串数组指针自加
	}
}

//自定义字符存储并显示
//说明：自定义字符需要先进行构造并储存，lcd1602才可以辨识并输出
//      pos参数为存储CGRAM位置
void LCD1602_set_char(unsigned char x, unsigned char y, unsigned char pos, unsigned char *str)
{
	unsigned char i;
	//构造自定义字符  确定CGRAM位置
	for (i = 0; i < 8; i++) {
		LCD1602_write_cmd(LCD_SET_CGRAM_ADDR + pos >> 3 + i);
		LCD1602_write_data(*str++);
	}
	LCD1602_set_pos(x, y);
	LCD1602_write_data(0x00 + pos);
}

//整数转字符串
unsigned char LCD1602_integer_to_str(int dat, unsigned char *str)
{
	unsigned char i = 0;
	unsigned char len = 0;
	unsigned char buf[6];  //int类型为6位  -32767~32768
	if (dat < 0) {         //dat为小数，则在字符串第一位添加“负号”
		dat = -dat;
		*str++ = '-';  //指针自加，为下一个数据存储做准备
		len++;
	}
	do {
		buf[i++] = dat % 10 + '0';  // +'0'操作可以转成ASCII码
		dat /= 10;     //取余
	} while (dat > 0);     //最后一位取余时余数为0，储存完后跳出循环

	len += i;              //记录一共有几位
	while (i-- > 0) {      //将临时字符串数据存进str
		*str++ = buf[i];
	}
	*str = '\0';           //最后一位补字符串结束符
	
	return (len);          //输出整数长度，可配合调用参数进行校验、补零等
}

//整数转字符串
//p.s. 将字符0xA5转成字符串输出后，lcd显示A5
void LCD1602_char_to_str(unsigned char dat, unsigned char *str)
{
	unsigned char temp = 0;
	temp = dat >> 4;     //处理高四位
	if (temp <= 9) {
		*str++ = temp + '0';
	} else {
		*str++= temp + '7';  //在ASCII表中转成ABCDEF
	}
	
	temp = dat & 0x0F;  //处理低四位
	if (temp <= 9) {
		*str++ = temp + '0';
	} else {
		*str++ = temp + '7';
	}
	*str = '\0';        //最后一位补字符串结束符
}
