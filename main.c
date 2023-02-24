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

#include <modules/water/water.h>
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

    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);

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
    _waterSensorInit();

}

/*
 * Main function
 */
void _enableInterrupts(){
    // Initialize main interrupt stuff
        Timer_A_configureUpMode(TIMER_A1_BASE, &mainLoopTimerUpConfig);
        Interrupt_enableInterrupt(INT_TA1_0);
        Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
        //enable master interrupts
        Interrupt_enableMaster(); 
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

//logic to water the plants
//constants
const int SERVO_WATER_OPEN_TIME = 10;
const int SERVO_WATER_CLOSE_TIME = 60;
const int WATER_TRESHOLD = 100;
//variables
int WATER_NEEDED = 0;
int servo_timer = 0;

//main timer handler

const int SAMPLE_DELAY = 10; //how often to do sampling
const int CHANGE_DISPLAY_DELAY = 45; //timing of chaning display

int temperature = 20;
int timer = 0;
int timer2 = CHANGE_DISPLAY_DELAY + 1;
int STATE = 0; //1 for temperature and brightness, 2 for terrain humidity and water level
char buffer[6];

void TA1_0_IRQHandler(void)
{

    if (timer2 > CHANGE_DISPLAY_DELAY){
        STATE = (STATE == 0) ? 1 : 0; //state changing
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

        timer = SAMPLE_DELAY +1; //forced upgrade
        GrContextFontSet(&g_sContext, &g_sFontCmss36b); //font size

    }

    if (WATER_NEEDED && servo_timer < SERVO_WATER_OPEN_TIME){
        servo_timer++;
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
    }
    else if (WATER_NEEDED && servo_timer < SERVO_WATER_CLOSE_TIME+SERVO_WATER_OPEN_TIME){
        servo_timer++;
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
    }
    else {
        servo_timer = 0;
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
    }

    if (timer > SAMPLE_DELAY){

        lux = _lightGetLuxValue();
        temperature -= (temperature-_temperatureGetTemperature())/3;
        int water = _waterLevelGetValue();
        int humidity = _humidityGetHumidity();

        if (water < 15){ //if water level under 15%
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5); //send telegram message
        }
        else {
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
        }
        if (humidity < WATER_TRESHOLD && water > 5) {
            WATER_NEEDED = 1;
            _ledSetRGB(255, 0, 0);
        }
        if (humidity > WATER_TRESHOLD+5 || water < 5){
            WATER_NEEDED = 0;
            servo_timer = 0;
        }
        if (humidity < WATER_TRESHOLD && water > 15) {
            _ledSetRGB(0, 0, 255);
        }
        if (water < 15 && humidity < WATER_TRESHOLD){
            _ledSetRGB(255, 0, 0);
        }

        if (humidity > WATER_TRESHOLD && water < 15){
            _ledSetRGB(255, 0, 0);
        }
        if (humidity > WATER_TRESHOLD && water > 15){
            _ledSetRGB(0, 0, 0);
        }

        if (STATE == 0){
            sprintf(buffer, "%dC", (temperature-40)*5/9);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) buffer, AUTO_STRING_LENGTH, 64, 32, OPAQUE_TEXT);
            sprintf(buffer, "%d'/,", (int) lux/300);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) buffer, AUTO_STRING_LENGTH, 64, 92, OPAQUE_TEXT);

            timer = 0;
        }
        if (STATE == 1){
            sprintf(buffer, "%d'/,", water);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) buffer, AUTO_STRING_LENGTH, 64, 32, OPAQUE_TEXT);
            sprintf(buffer, "%d'/,", humidity);
            Graphics_drawStringCentered(&g_sContext, (int8_t *) buffer, AUTO_STRING_LENGTH, 64, 92, OPAQUE_TEXT);

            timer = 0;


        }
    }



    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    timer++;
    timer2++;
}

