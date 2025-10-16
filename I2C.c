#include "I2C.h"

void I2C_Master_Init(void)
{
    SSPCON = 0b00101000; // Master mode, enable MSSP
    SSPCON2 = 0x00;
    SSPADD = (_XTAL_FREQ / (4 * I2C_SPEED)) - 1;
    SSPSTAT = 0x00;
    
    TRISC3 = 1; // SCL là input
    TRISC4 = 1; // SDA là input
}

void I2C_Master_Start(void)
{
    SEN = 1;          // Start condition
    while(SEN);
}

void I2C_Master_RepeatedStart(void)
{
    RSEN = 1;         // Repeated start
    while(RSEN);
}

void I2C_Master_Stop(void)
{
    PEN = 1;          // Stop condition
    while(PEN);
}

void I2C_Master_Write(unsigned char data)
{
    SSPBUF = data;    // Ghi data vào buffer
    while(BF);        // Ch? h?t truy?n
    while(SSPSTATbits.R_nW); // Ch? ghi xong
}

unsigned char I2C_Master_Read(unsigned char ack)
{
    unsigned char received_data;
    
    RCEN = 1;             // Enable receive
    while(!BF);           // Ch? nh?n ?? 8 bit
    received_data = SSPBUF;  // ??c buffer

    ACKDT = (ack)?0:1;    // ack=1 g?i ACK, ack=0 g?i NACK
    ACKEN = 1;            // G?i ACK/NACK
    while(ACKEN);

    return received_data;
}

