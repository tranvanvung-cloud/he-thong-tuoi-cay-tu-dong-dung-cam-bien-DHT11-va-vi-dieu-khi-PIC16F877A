#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "I2C.h"
#include "LCD_I2C.h"

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Disable
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

#define _XTAL_FREQ 20000000  // T?n s? th?ch anh 20MHz

// DHT11
#define DHT11_PIN    RB0
#define DHT11_TRIS   TRISB0

// Pump and LED
#define PUMP_PIN     RC0
#define PUMP_TRIS    TRISC0
#define LED_PIN      RB1
#define LED_TRIS     TRISB1

// DHT11 functions
void DHT11_SetPinOutput(void) {
    DHT11_TRIS = 0;
}

void DHT11_SetPinInput(void) {
    DHT11_TRIS = 1;
}

void DHT11_Start(void) {
    DHT11_SetPinOutput();
    DHT11_PIN = 0;
    __delay_ms(20);
    DHT11_PIN = 1;
    __delay_us(30);
    DHT11_SetPinInput();
}

uint8_t DHT11_CheckResponse(void) {
    uint8_t response = 0;
    __delay_us(40);
    if (DHT11_PIN == 0) {
        __delay_us(80);
        if (DHT11_PIN == 1) response = 1;
        __delay_us(40);
    }
    return response;
}

uint8_t DHT11_ReadByte(void) {
    uint8_t i, byte = 0;
    for (i = 0; i < 8; i++) {
        while (!DHT11_PIN);
        __delay_us(30);
        if (DHT11_PIN) byte |= (1 << (7 - i));
        while (DHT11_PIN);
    }
    return byte;
}

uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity) {
    uint8_t humi_int, humi_dec, temp_int, temp_dec, checksum;
    
    DHT11_Start();
    if (DHT11_CheckResponse()) {
        humi_int = DHT11_ReadByte();
        humi_dec = DHT11_ReadByte();
        temp_int = DHT11_ReadByte();
        temp_dec = DHT11_ReadByte();
        checksum = DHT11_ReadByte();
        
        if ((humi_int + humi_dec + temp_int + temp_dec) == checksum) {
            *humidity = humi_int;
            *temperature = temp_int;
            return 1;   // Success
        }
    }
    return 0;   // Failed
}

// Main
void main(void) {
    uint8_t humidity, temperature;
    char buffer[17];

    // I2C and LCD init
    TRISC3 = 1;  // SCL input
    TRISC4 = 1;  // SDA input
    I2C_Master_Init(); // 100kHz
    LCD_Init();
    LCD_Clear();
    
    // Output pins init
    PUMP_TRIS = 0;
    PUMP_PIN = 0;
    LED_TRIS = 0;
    LED_PIN = 0;
    
    INTCON = 0;  // Disable interrupts

    LCD_Set_Cursor(1, 1);
    LCD_Write_String("DHT11 + LCD");
    
    __delay_ms(1000);  // Delay ?? hi?n th? dòng trên
    
    // ??t con tr? ? dòng 2, c?t 1
    LCD_Set_Cursor(2, 1);
    LCD_Write_String("Dang khoi dong...");
    __delay_ms(1000);  // Delay ?? hi?n th? dòng thông báo kh?i ??ng
    LCD_Clear();
    while (1) {
        // ??c d? li?u t? DHT11
        if (DHT11_ReadData(&humidity, &temperature)) {
            // Hi?n th? ?? ?m
            LCD_Set_Cursor(1, 1);  // ??t con tr? dòng 2
            sprintf(buffer, "do am: %d%%", humidity);
            LCD_Write_String(buffer);
            
            // ?i?u khi?n b?m theo ?? ?m
            if (humidity < 30) {
                // B?t b?m và LED
                PUMP_PIN = 1;
                LED_PIN = 1;
                
                // C?p nh?t tr?ng thái b?m trên LCD
                LCD_Set_Cursor(2, 1);  // ??t con tr? dòng 3
                LCD_Write_String("Pump: ON");
            } else if (humidity > 70) {
                // T?t b?m và LED
                PUMP_PIN = 0;
                LED_PIN = 0;
                
                // C?p nh?t tr?ng thái b?m trên LCD
                LCD_Set_Cursor(2, 1);  // ??t con tr? dòng 3
                LCD_Write_String("Pump: OFF");
            } else {
                // Gi? b?m ? tr?ng thái Idle (ngh?)
                LCD_Set_Cursor(2, 1);  // ??t con tr? dòng 3
                LCD_Write_String("Pump: Idle");
            }
        } else {
            // N?u có l?i trong vi?c ??c DHT11
            LCD_Set_Cursor(1, 1);
            LCD_Write_String("Error Reading DHT");
        }

        __delay_ms(1000);  // Delay gi?a các l?n ??c d? li?u
    }
}