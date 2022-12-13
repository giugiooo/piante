#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "led.h"

int main(void)
{
    volatile uint32_t i;

    // Stop watchdog timer
    WDT_A_holdTimer();

    initializeLeds();

    while(1){

        lightSingleLed(BLU));

        // Delay
        for(i=100000; i>0; i--);
    }
}
