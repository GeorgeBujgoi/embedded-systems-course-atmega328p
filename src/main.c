#include "drivers/timer/timer0.h"
#include "bsp/nano.h"
#include "drivers/usart/usart.h"
#include "drivers/timer/timer0.h"
#include "utils/delay.h"

uint8_t pump_state = 0;
void pump_on(void)
{
    GPIO_Write(GPIO_PORTD, 7, GPIO_HIGH); // releu ON
    pump_state = 1;
}

void pump_off(void)
{
    GPIO_Write(GPIO_PORTD, 7, GPIO_LOW); // releu OFF
    pump_state = 0;
}

int main(void)
{
    Timer0_Init();
    ADC_Init();
    USART_Init_Default();

    GPIO_Init(GPIO_PORTD, 7, GPIO_OUTPUT);

    uint32_t last_time = 0;
    uint16_t adc_value = 0;
    char text[60];

    pump_off();

    while (1)
    {
        if (Millis() - last_time >= 500)
        {
            last_time = Millis();

            adc_value = ADC_Read(0);

            if (adc_value > 580)
                pump_on();

            if (adc_value < 550)
                pump_off();

    if (pump_state == 0)
    {
    sprintf(text, "ADC=%u Pump=%u\r\n", adc_value, pump_state);
    USART_Transmit(text, strlen(text));
    }

    }
    }
}
