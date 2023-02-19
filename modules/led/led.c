#include "led.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

/* Timer_A Compare Configuration Parameter  (PWM) */
Timer_A_CompareModeConfig compareConfig_PWM = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output but
        50                                        // 50% Duty Cycle
        };

/* Timer_A Up Configuration Parameter */
const Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,                      // SMCLK = 3 MhZ
        TIMER_A_CLOCKSOURCE_DIVIDER_12,         // SMCLK/12 = 250 KhZ
        1000,                                  // 40 ms tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
        };


void _ledSetRGB(int r, int g, int b){
    Timer_A_setCompareValue(TIMER_A0_BASE,  TIMER_A_CAPTURECOMPARE_REGISTER_3, r);
    Timer_A_setCompareValue(TIMER_A0_BASE,  TIMER_A_CAPTURECOMPARE_REGISTER_1, g);
    Timer_A_setCompareValue(TIMER_A2_BASE,  TIMER_A_CAPTURECOMPARE_REGISTER_1, b);
}

void _ledInit()
{
    /* Configures P2.4 to PM_TA0.1 for using Timer PWM to control LED */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
    GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configures P2.6 to PM_TA0.3 for using Timer PWM to control LED */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6,
    GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configures P5.6 to PM_TA0.4 for using Timer PWM to control LED */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6,
    GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring Timer_A0 for Up Mode and starting */
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

    /* Initialize compare registers to generate PWM */

    //red (Port 2.4)
    compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
    compareConfig_PWM.compareValue = 5;
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);

    // green (Port 2.6)
    compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    compareConfig_PWM.compareValue = 5;
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);

    // blue (Port 5.6)
    // For Port 5.6
    compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    compareConfig_PWM.compareValue = 5;
    Timer_A_initCompare(TIMER_A2_BASE, &compareConfig_PWM);

    _ledSetRGB(0, 0, 0);

}













