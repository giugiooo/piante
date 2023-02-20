/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//****************************************************************************
//
// main.c - MSP-EXP432P401R + Educational Boosterpack MkII - Temperature
//
//          Displays temperature measured by the TMP006 Infrared Thermopile
//          Contactless Temperature Sensor. The MSP432 communicates
//          with the sensor through I2C.
//
//****************************************************************************

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "modules/led/led.h"
#include "modules/light/light.h"
#include <stdio.h>
#include "modules/lcd/lcd.h"
#include "modules/temperature/temperature.h"

#define TIMER_PERIOD 11718



const Timer_A_UpModeConfig mainLoopTimerUpConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,         // SMCLK/1 = 3MHz
        TIMER_PERIOD,                           // every half second
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};


void _hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    _graphicsInit();
    _ledInit();
    _lightSensorInit();
    _temperatureSensorInit();

}

/*
 * Main function
 */
void _enableInterrupts(){
    // Initialize main interrupt stuff
        Timer_A_configureUpMode(TIMER_A1_BASE, &mainLoopTimerUpConfig);
        //Interrupt_enableSleepOnIsrExit();
        Interrupt_enableInterrupt(INT_TA1_0);
        Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
        //Interrupt_enableMaster(); //enable master interrupts*/
}


int main(void)
{

    _hwInit();

    _enableInterrupts();

    while (1)
    {
        PCM_gotoLPM0();
    }

}


//main timer handler
int temperature = 20;
int water_level;
const int SAMPLE_DELAY = 10; //ogni quanto fare il sampling
const int CHANGE_DISPLAY_DELAY = 45; //ogni quanto cambia la schermata
int timer = 0;
int timer2 = CHANGE_DISPLAY_DELAY+1;
int STATE = 0; //1 for temperature and brightness, 2 for terrain humidity and water level
char buffer[6];
void TA1_0_IRQHandler(void)
{

    if (timer2 > CHANGE_DISPLAY_DELAY){
        STATE = (STATE == 0) ? 1 : 0; //cambia lo stato
        timer2 = 0;

        Graphics_clearDisplay(&g_sContext);
        if (STATE == 0){
            GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) "Temperature:", AUTO_STRING_LENGTH, 64, 12, OPAQUE_TEXT);
            GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) "Brightness", AUTO_STRING_LENGTH, 64, 72, OPAQUE_TEXT);
        }
        if (STATE == 1){
            GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) "Water level:", AUTO_STRING_LENGTH, 64, 12, OPAQUE_TEXT);
            GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) "Terrain", AUTO_STRING_LENGTH, 64, 62, OPAQUE_TEXT);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) "humidity", AUTO_STRING_LENGTH, 64, 72, OPAQUE_TEXT);
        }

        timer = SAMPLE_DELAY +1; //aggiornamento forzato
        GrContextFontSet(&g_sContext, &g_sFontCmss36b); //font grande in modo che le successive volte che scrivi qualcosa vengono fuori i numeri grandi

    }

    if (timer > SAMPLE_DELAY){

        if (STATE == 0){
            lux = _lightGetLuxValue();
            temperature -= (temperature-_temperatureGetTemperature())/3;
            sprintf(buffer, "%d", (temperature-40)*5/9);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) buffer, AUTO_STRING_LENGTH, 64, 32, OPAQUE_TEXT);
            sprintf(buffer, "%d%", (int) lux/300);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) buffer, AUTO_STRING_LENGTH, 64, 92, OPAQUE_TEXT);

            timer = 0;

            //todo: aggiungere
        }
        if (STATE == 1){
            water_level = 70; //todo: get water level
            sprintf(buffer, "%d%", water_level);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) buffer, AUTO_STRING_LENGTH, 64, 32, OPAQUE_TEXT);
            timer = 0;

            if (water_level < 15){ //se il livello dell'acqua è sotto 15%
                _ledSetRGB(255, 0, 0);
            }
            else {
                _ledSetRGB(0, 0, 0);
            }

        }

    }

    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    timer++;
    timer2++;
}

