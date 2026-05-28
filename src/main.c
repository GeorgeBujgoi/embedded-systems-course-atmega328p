#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <avr/io.h>

#include "bsp/nano.h"

#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "drivers/usart/usart.h"
#include "drivers/lcd/lcd.h"

#include "pump.h"
#include "buzzer.h"
#include "sensors.h"

#define BAUD 57600

uint8_t auto_mode = 1;
uint8_t auto_watering = 0;
uint8_t pause_after_watering = 0;

uint8_t last_mode_button = 1;

uint32_t last_button_time = 0;
uint32_t pump_start_time = 0;
uint32_t pause_start_time = 0;
uint32_t last_time = 0;

int main(void)
{
    Timer0_Init();
    USART_Init(F_CPU, BAUD);
    LCD_Init();

    Pump_Init();
    Buzzer_Init();
    Sensors_Init();

    GPIO_Init(GPIO_PORTD, 4, GPIO_INPUT); // manual pump
    GPIO_Init(GPIO_PORTD, 5, GPIO_INPUT); // auto/manual switch

    PORTD |= (1 << PD4);
    PORTD |= (1 << PD5);

    uint16_t soil_value = 0;
    uint16_t water_value = 0;

    char buffer[64];
    char line[17];

    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Smart Irrig.");
    LCD_SetCursor(1, 0);
    LCD_Print("AUTO MODE    ");

    USART_Transmit("SYSTEM START\r\n", 14);

    while (1)
    {
        if (auto_watering == 1)
        {
            if (Millis() - pump_start_time >= 2000)
            {
                Pump_Off();
                auto_watering = 0;

                pause_after_watering = 1;
                pause_start_time = Millis();

                LCD_SetCursor(0, 0);
                LCD_Print("Pump stopped  ");
                LCD_SetCursor(1, 0);
                LCD_Print("Pause 10 sec  ");
            }

            continue;
        }

    if (pause_after_watering == 1) {
        water_value = Sensors_ReadWater();

        if (water_value < 300) {
            Pump_Off();
            Buzzer_On();

            LCD_SetCursor(0, 0);
            LCD_Print("NO WATER      ");

            LCD_SetCursor(1, 0);
            sprintf(line, "Water:%u    ", water_value);
            LCD_Print(line);

            pause_after_watering = 0;
            continue;
        }

        if (Millis() - pause_start_time >= 10000) {
            pause_after_watering = 0;

            LCD_SetCursor(0, 0);
            LCD_Print("Checking...   ");
        }

        continue;
    }

        if (Pump_GetState() == 0)
        {
            uint8_t mode_button = GPIO_Read(GPIO_PORTD, 5);

            if (last_mode_button == GPIO_HIGH && mode_button == GPIO_LOW)
            {
                if (Millis() - last_button_time > 500)
                {
                    last_button_time = Millis();
                    auto_mode = !auto_mode;

                    Pump_Off();

                    if (auto_mode)
                    {
                        USART_Transmit("AUTO MODE\r\n", 11);

                        LCD_SetCursor(0, 0);
                        LCD_Print("AUTO MODE     ");
                        LCD_SetCursor(1, 0);
                        LCD_Print("System active ");
                    }
                    else
                    {
                        USART_Transmit("MANUAL MODE\r\n", 13);

                        LCD_SetCursor(0, 0);
                        LCD_Print("MANUAL MODE   ");
                        LCD_SetCursor(1, 0);
                        LCD_Print("Hold D4       ");
                    }
                }
            }

            last_mode_button = mode_button;
        }

        if (auto_mode == 0)
        {
            Buzzer_Off();

            if (GPIO_Read(GPIO_PORTD, 4) == GPIO_LOW)
            {
                Pump_On();
                continue;
            }
            else
            {
                if (Pump_GetState() == 1)
                {
                    Pump_Off();

                    LCD_SetCursor(0, 0);
                    LCD_Print("MANUAL MODE   ");
                    LCD_SetCursor(1, 0);
                    LCD_Print("Pump: OFF     ");
                }
            }

            continue;
        }

        if (Millis() - last_time >= 500)
        {
            last_time = Millis();

            soil_value = Sensors_ReadSoil();
            water_value = Sensors_ReadWater();

            if (water_value < 300)
            {
                Pump_Off();
                Buzzer_On();

                LCD_SetCursor(0, 0);
                LCD_Print("NO WATER      ");

                LCD_SetCursor(1, 0);
                sprintf(line, "Water:%u    ", water_value);
                LCD_Print(line);

                sprintf(buffer, "NO WATER %u\r\n", water_value);
                USART_Transmit(buffer, strlen(buffer));
            }
            else
            {
                Buzzer_Off();

                LCD_SetCursor(0, 0);
                sprintf(line, "S:%u W:%u ", soil_value, water_value);
                LCD_Print(line);

                LCD_SetCursor(1, 0);
                LCD_Print("AUTO MODE     ");

sprintf(buffer,
        "Soil=%u Water=%u Temp=%u Pump=%u Mode=%s\r\n",
        soil_value,
        water_value,
        temp_value,
        Pump_GetState(),
        auto_mode ? "AUTO" : "MANUAL");

                USART_Transmit(buffer, strlen(buffer));

                if (soil_value > 850)
                {
                    Pump_On();
                    auto_watering = 1;
                    pump_start_time = Millis();
                }
            }
        }
    }
}