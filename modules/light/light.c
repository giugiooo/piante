#include "../i2c/HAL_I2C.h"
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "HAL_OPT3001.h"
#include <stdio.h>
#include "light.h"

//Variable for storing lux value returned from OPT3001

void _lightSensorInit()
{
    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();

    /* Initialize OPT3001 digital ambient light sensor */
    OPT3001_init();

}

//visualize on display brightness value
float _lightGetLuxValue()
{
    return OPT3001_getLux();
}
    


