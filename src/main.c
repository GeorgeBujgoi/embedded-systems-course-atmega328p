#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "bsp/nano.h"

#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "drivers/adc/adc.h"
#include "drivers/usart/usart.h"
#include "drivers/lcd/lcd.h"

#include "pump.h"

int main(void)
{
    Timer0_Init();
    ADC_Init();
    USART_Init_Default();
    LCD_Init();

    Pump_Init();

    uint32_t last_time = 0;
    uint16_t adc_value = 0;

    char text[60];
    char line[17];

    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Smart Irrig");

    while (1)
    {
        if (Millis() - last_time >= 500)
        {
            last_time = Millis();

            adc_value = ADC_Read(0);

            if (adc_value > 580)
                Pump_On();

            LCD_SetCursor(0, 0);

            sprintf(line,"ADC:%u      ",adc_value);

            LCD_Print(line);

            LCD_SetCursor(1, 0);

            if (Pump_GetState())
                LCD_Print("Pump: ON    ");
            else
                LCD_Print("Pump: OFF   ");

            if (Pump_GetState() == 0)
            {
                sprintf(text,"ADC=%u Pump=%u\r\n",adc_value, Pump_GetState());

                USART_Transmit(text,strlen(text));
            }
        }
    }
}