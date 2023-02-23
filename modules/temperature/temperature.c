#include "../i2c/HAL_I2C.h"
#include "HAL_TMP006.h"

/* Temperature Sensor initialization */

void _temperatureSensorInit()
{
    /* Initialize I2C communication */
    /* Initialize TMP006 temperature sensor */
    TMP006_init();

}

int _temperatureGetTemperature(){
    return (int) TMP006_getTemp();
}
