#include "sensors.h"
#include "drivers/adc/adc.h"

void Sensors_Init(void)
{
    ADC_Init();
}

uint16_t Sensors_ReadSoil(void)
{
    return ADC_Read(0);
}

uint16_t Sensors_ReadWater(void)
{
    return ADC_Read(1);
}