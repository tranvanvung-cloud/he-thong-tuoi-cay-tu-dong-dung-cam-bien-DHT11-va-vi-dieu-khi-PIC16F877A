#include "LCD_I2C.h"
#include <string.h>

void LCD_Send_Cmd(char cmd)
{
    char upper_nibble, lower_nibble;
    upper_nibble = cmd & 0xF0;
    lower_nibble = (cmd << 4) & 0xF0;

    // G?i 4-bit upper
    I2C_Master_Start();
    I2C_Master_Write(LCD_Address);
    I2C_Master_Write(upper_nibble | 0x0C);  // RS=0, EN=1
    I2C_Master_Write(upper_nibble | 0x08);  // EN=0
    I2C_Master_Stop();

    // G?i 4-bit lower
    I2C_Master_Start();
    I2C_Master_Write(LCD_Address);
    I2C_Master_Write(lower_nibble | 0x0C);
    I2C_Master_Write(lower_nibble | 0x08);
    I2C_Master_Stop();

    __delay_ms(2);
}

void LCD_Send_Char(char data)
{
    char upper_nibble, lower_nibble;
    upper_nibble = data & 0xF0;
    lower_nibble = (data << 4) & 0xF0;

    // G?i upper 4-bit
    I2C_Master_Start();
    I2C_Master_Write(LCD_Address);
    I2C_Master_Write(upper_nibble | 0x0D);  // RS=1, EN=1
    I2C_Master_Write(upper_nibble | 0x09);  // EN=0
    I2C_Master_Stop();

    // G?i lower 4-bit
    I2C_Master_Start();
    I2C_Master_Write(LCD_Address);
    I2C_Master_Write(lower_nibble | 0x0D);
    I2C_Master_Write(lower_nibble | 0x09);
    I2C_Master_Stop();

    __delay_ms(2);
}

void LCD_Write_String(char *str)
{
    while(*str)
    {
        LCD_Send_Char(*str++);
    }
}

void LCD_Set_Cursor(char row, char col)
{
    char pos;
    switch(row)
    {
        case 1:
            pos = 0x80 + (col - 1);
            break;
        case 2:
            pos = 0xC0 + (col - 1);
            break;
        default:
            pos = 0x80;
    }
    LCD_Send_Cmd(pos);
}

void LCD_Init(void)
{
    __delay_ms(50);   // LCD kh?i ??ng

    LCD_Send_Cmd(0x33); // Kh?i t?o 4-bit
    LCD_Send_Cmd(0x32); 
    LCD_Send_Cmd(0x28); // 4-bit, 2 dòng, 5x7 font
    LCD_Send_Cmd(0x0C); // B?t hi?n th?, t?t con tr?
    LCD_Send_Cmd(0x06); // T? ??ng d?ch con tr?
    LCD_Send_Cmd(0x01); // Xóa màn hình
    __delay_ms(2);
}
void LCD_Clear(void) {
    LCD_Send_Cmd(0x01);  // L?nh ?? xóa màn hình LCD
    __delay_ms(2);       // ??i cho l?nh ???c th?c hi?n
}
