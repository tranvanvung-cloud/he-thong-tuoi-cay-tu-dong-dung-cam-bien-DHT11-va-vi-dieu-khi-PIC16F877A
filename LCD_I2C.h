#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <xc.h>
#include "I2C.h"

#define LCD_Address 0x4E  // ??a ch? PCF8574 g?n v?i LCD. (0x4E = 0x27 << 1)

void LCD_Init(void);
void LCD_Send_Cmd(char cmd);
void LCD_Send_Char(char data);
void LCD_Write_String(char *str);
void LCD_Set_Cursor(char row, char col);
void LCD_Clear(void);
#endif
