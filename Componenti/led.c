#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "led.h"


void initializeLeds(){
    GPIO_setAsOutputPin(GPIO_PORT_J4,GPIO_PIN37); //blu
    GPIO_setAsOutputPin(GPIO_PORT_J4,GPIO_PIN38); //green
    GPIO_setAsOutputPin(GPIO_PORT_J4,GPIO_PIN39); //red

}

void lightSingleLed(Luce colore){
    switch(colore){
    case ROSSO:
        GPIO_setOutputLowOnPin(GPIO_PORT_PJ,GPIO_PIN4);
        GPIO_setOutputLowOnPin(GPIO_PORT_J4,GPIO_PIN38);
        GPIO_setOutputHighOnPin(GPIO_PORT_J4,GPIO_PIN39);
        break;
    case VERDE:
        GPIO_setOutputHighOnPin(GPIO_PORT_J4,GPIO_PIN38);
        GPIO_setOutputLowOnPin(GPIO_PORT_J4,GPIO_PIN39);
        GPIO_setOutputLowOnPin(GPIO_PORT_J4,GPIO_PIN37);
        break;
    case BLU:
        GPIO_setOutputHighOnPin(GPIO_PORT_J4,GPIO_PIN37);
        GPIO_setOutputLowOnPin(GPIO_PORT_J4,GPIO_PIN39);
        GPIO_setOutputLowOnPin(GPIO_PORT_J4,GPIO_PIN38);
        break;
    }
}
