#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "bsp/nano.h"

#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "drivers/usart/usart.h"
#include "drivers/lcd/lcd.h"

#include "pump.h"
#include "buzzer.h"
#include "sensors.h"

int main(void)
{
    Timer0_Init();
    USART_Init_Default();
    LCD_Init();

    Pump_Init();
    Buzzer_Init();
    Sensors_Init();

    uint32_t last_time = 0;

    uint16_t soil_value = 0;
    uint16_t water_value = 0;

    char text[64];
    char line[17];

    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Smart Irrig");

    while (1)
    {
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

                sprintf(text, "NO WATER %u\r\n", water_value);
                USART_Transmit(text, strlen(text));

                continue;
            }

            Buzzer_Off();

            if (soil_value > 580)
                Pump_On();

            LCD_SetCursor(0, 0);
            sprintf(line, "S:%u W:%u ", soil_value, water_value);
            LCD_Print(line);

            LCD_SetCursor(1, 0);

            if (Pump_GetState())
                LCD_Print("Pump: ON    ");
            else
                LCD_Print("Pump: OFF   ");

            if (Pump_GetState() == 0)
            {
                sprintf(text,
                        "Soil=%u Water=%u Pump=%u\r\n",
                        soil_value,
                        water_value,
                        Pump_GetState());

                USART_Transmit(text, strlen(text));
            }
        }
    }
}