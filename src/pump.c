#include "pump.h"
#include "drivers/gpio/gpio.h"

static uint8_t pump_state = 0;

void Pump_Init(void)
{
    GPIO_Init(GPIO_PORTD, 7, GPIO_OUTPUT);
    Pump_Off();
}

void Pump_On(void)
{
    GPIO_Write(GPIO_PORTD, 7, GPIO_HIGH);
    pump_state = 1;
}

void Pump_Off(void)
{
    GPIO_Write(GPIO_PORTD, 7, GPIO_LOW);
    pump_state = 0;
}

uint8_t Pump_GetState(void)
{
    return pump_state;
}