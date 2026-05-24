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

int main(void) {
    
   
    }
