#include "buzzer.h"
#include "drivers/gpio/gpio.h"

void Buzzer_Init(void)
{
    GPIO_Init(GPIO_PORTD, 6, GPIO_OUTPUT);
    Buzzer_Off();
}

void Buzzer_On(void)
{
    GPIO_Write(GPIO_PORTD, 6, GPIO_HIGH);
}

void Buzzer_Off(void)
{
    GPIO_Write(GPIO_PORTD, 6, GPIO_LOW);
}