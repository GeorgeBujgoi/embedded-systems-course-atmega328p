#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>

void Sensors_Init(void);
uint16_t Sensors_ReadSoil(void);
uint16_t Sensors_ReadWater(void);

#endif