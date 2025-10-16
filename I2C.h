#ifndef I2C_H
#define I2C_H

#include <xc.h>

// C?u hình t?n s? th?ch anh
#define _XTAL_FREQ 20000000  // 20MHz, b?n ??i theo th?c t?

#define I2C_SPEED 100000     // 100kHz

void I2C_Master_Init(void);
void I2C_Master_Start(void);
void I2C_Master_RepeatedStart(void);
void I2C_Master_Stop(void);
void I2C_Master_Write(unsigned char data);
unsigned char I2C_Master_Read(unsigned char ack);

#endif
