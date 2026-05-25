#ifndef PUMP_H
#define PUMP_H

#include <stdint.h>

void Pump_Init(void);
void Pump_On(void);
void Pump_Off(void);
uint8_t Pump_GetState(void);

#endif