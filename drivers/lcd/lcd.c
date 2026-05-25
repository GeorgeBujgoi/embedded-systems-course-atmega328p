#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

#define LCD_ADDR 0x27

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RS        0x01

static void I2C_Init(void)
{
    TWSR = 0x00;
    TWBR = 72;
    TWCR = (1 << TWEN);
}

static void I2C_Start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

static void I2C_Stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

static void I2C_Write(unsigned char data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

static void LCD_WriteByte(unsigned char data)
{
    I2C_Start();
    I2C_Write(LCD_ADDR << 1);
    I2C_Write(data | LCD_BACKLIGHT);
    I2C_Stop();
}

static void LCD_Pulse(unsigned char data)
{
    LCD_WriteByte(data | LCD_ENABLE);
    _delay_us(1);
    LCD_WriteByte(data & ~LCD_ENABLE);
    _delay_us(50);
}

static void LCD_Send4Bits(unsigned char data)
{
    LCD_WriteByte(data);
    LCD_Pulse(data);
}

static void LCD_Send(unsigned char value, unsigned char mode)
{
    unsigned char high = value & 0xF0;
    unsigned char low = (value << 4) & 0xF0;

    LCD_Send4Bits(high | mode);
    LCD_Send4Bits(low | mode);
}

static void LCD_Command(unsigned char cmd)
{
    LCD_Send(cmd, 0);
    _delay_ms(2);
}

static void LCD_Data(unsigned char data)
{
    LCD_Send(data, LCD_RS);
}

void LCD_Init(void)
{
    I2C_Init();

    _delay_ms(50);

    LCD_Send4Bits(0x30);
    _delay_ms(5);

    LCD_Send4Bits(0x30);
    _delay_us(150);

    LCD_Send4Bits(0x30);
    LCD_Send4Bits(0x20);

    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);

    LCD_Clear();
}

void LCD_Clear(void)
{
    LCD_Command(0x01);
    _delay_ms(2);
}

void LCD_SetCursor(unsigned char row, unsigned char col)
{
    unsigned char address;

    if (row == 0)
        address = 0x00 + col;
    else
        address = 0x40 + col;

    LCD_Command(0x80 | address);
}

void LCD_Print(char *text)
{
    while (*text)
    {
        LCD_Data(*text);
        text++;
    }
}