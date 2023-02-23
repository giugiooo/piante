#include <modules/water/water.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

//Global Constants
//if TapWater there is no conductibility so values change 
const bool tapWater = true;
 
//Global variables
int HistoryValue = 0;
char printBuffer[45];

//Void Setup
void _waterSensorInit(){

            /* Configuring GPIOs (5.5 A0) */
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4,
            GPIO_TERTIARY_MODULE_FUNCTION);
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,
                        GPIO_TERTIARY_MODULE_FUNCTION);


            /* Enable ADC block*/
            ADC14_enableModule();

            //![Single Sample Mode Configure]
            /* Initializing ADC (MCLK/1/4) */
            ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4, ADC_MAPINTCH1);

            /* Configuring ADC Memory */
            ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
            ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                        ADC_INPUT_A0, false);
            ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A1, false);


            /* Configuring Sample Timer */
            ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

            /* Enabling/Toggling Conversion */
            ADC14_enableConversion();
            ADC14_toggleConversionTrigger();
            //![Single Sample Mode Configure]

            /* Enabling interrupts */
            ADC14_enableInterrupt(ADC_INT0);
            ADC14_enableInterrupt(ADC_INT1);
            Interrupt_enableInterrupt(INT_ADC14);
}

void ADC14_IRQHandler(void)
{
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if ((ADC_INT0 & status) || (ADC_INT1 & status))
    {
        /* should be berween 0 and 16384*/
        water_level = (int) ADC14_getResult(ADC_MEM0);
        humidity = ADC14_getResult(ADC_MEM1);

        /* next sample */
        ADC14_toggleConversionTrigger();
    }
}

const int MAX_HUMIDITY = 16000;
const int MIN_HUMIDITY = 11500;

int _humidityGetHumidity(){
    int m = (MIN_HUMIDITY-MAX_HUMIDITY)/100;
    int percentage = (humidity-MAX_HUMIDITY)/m;

    if (percentage > 100) percentage = 100;
    if (percentage < 0) percentage = 0;

    return percentage;
}
int _waterLevelGetValue(){

	if (water_level > 2750){
	    if (water_level > 3500) {
	        return 100;
	    }
	    return (water_level-2500)/10;
	}
	else {
	    if (water_level < 1500){
	        return 0;
	    }
	    return (water_level-1500)/50;
	}
}

