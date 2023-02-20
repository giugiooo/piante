#include "water_level.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

//function
/*String analogPinConverter(int value){
	String analogPinValue;
	
	switch(value){
		//pin D14 = A0
		case 14: analogPinValue = "A0"; break;
		//pin D15 = A1
		case 15: analogPinValue = "A1"; break;
		//pin D16 = A2
		case 16: analogPinValue = "A2"; break;
		//pin D17 = A3
		case 17: analogPinValue = "A3"; break;
		//pin D18 = A4
		case 18: analogPinValue = "A4"; break;
		//pin D19 = A5
		case 19: analogPinValue = "A5"; break;
		default :_ analogPinValue = "Pin Error"; break;
	} 
	return analogPinValue; 
}*/


//Global Constants
//const int sensorPin = GPIO_PORT_P;
//if TapWater there is no conductibility so values change 
const bool tapWater = true;
 
//Global variables
int HistoryValue = 0;
char printBuffer[45];
//String analogPin;


//Void Setup
void _waterSensorInit(){
            GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN1);

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
            //ADC14_configureSingleSampleMode(ADC_MEM0, true);
            ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
            //ADC14_configureSingleSampleMode(ADC_MEM1, true);
            //ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS,
            //ADC_INPUT_A1, false);
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
            //Interrupt_enableMaster();
}

int asd;
void ADC14_IRQHandler(void)
{
    asd = 1;
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if ((ADC_INT0 & status) || (ADC_INT1 & status))
    {
        /* should be berween 0 and 16384*/
        water_level = (int) ADC14_getResult(ADC_MEM0);
        asd = ADC14_getResult(ADC_MEM1);
        //asd = (int) ADC14_getResult(ADC_MEM1);

        if(asd > 2500) {
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
        }
        else
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);

        /* next sample */
        ADC14_toggleConversionTrigger();
    }
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

