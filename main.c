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
#include "modules/temperature/HAL_I2C.h"
#include "modules/temperature/HAL_TMP006.h"
#include "modules/led/led.h"
#include <stdio.h>
#include "modules/lcd/lcd.h"

/* Graphic library context */
Graphics_Rectangle rectangle;

/* Variable for storing temperature value returned from TMP006 */
float temp;



void _hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

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
}

/*
 * Main function
 */
int main(void)
{

    _hwInit();



    rectangle.xMax = 100;
    rectangle.yMax = 100;
    rectangle.xMin = 0;
    rectangle.yMin = 0;

    //Graphics_fillRectangle(&g_sContext, &rectangle);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) "Temperature:", AUTO_STRING_LENGTH, 64, 12, OPAQUE_TEXT);
    GrContextFontSet(&g_sContext, &g_sFontCmss36b);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) "5C", AUTO_STRING_LENGTH, 64, 32, OPAQUE_TEXT);

    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) "Greenhouse", AUTO_STRING_LENGTH, 64, 62, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) "humidity", AUTO_STRING_LENGTH, 64, 72, OPAQUE_TEXT);
    GrContextFontSet(&g_sContext, &g_sFontCmss36b);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) "50%", AUTO_STRING_LENGTH, 64, 92, OPAQUE_TEXT);

    while (1) {

    }

}

