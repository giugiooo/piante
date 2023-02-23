# WATERING GREENHOUSE - Project for the course "Embedded Software for the Internet of Things" (group 19)

This project consists in a greenhouse connected to a display which provides informations about the environment inside the planthouse (temperature and brightness) and about the plants living inside of it (soil humidity). In addition to the monitoring process, the greenhouse is able to water the plants when needed and to notify the plant owner when the water tank needs to be refilled. 
Our aim? Keeping a plant alive! 

## FEATURES:
The features implemented and tested in the project are the following:

- checks the temperature inside the greenhouse;
- checks the brightness of the environment;
- checks if the soil is humid enough; 
- erogates water from a water tank when the plant needs it;
- notifies the plant owner when the water tank is empty;
- displays informations about temperature/brightness of the environment and level of water in the tank;
- if the tank is almost empty a red led lights up in the greenhouse. 

## REQUIREMENTS:

### Hardware:
#### MSP432P401R Board + Educational booster pack (temperature sensor, brightness sensor, display, button)
![7f2e25b1-6b96-4478-a5f6-b2deba43b8f4](https://user-images.githubusercontent.com/125813273/220143137-08dbf299-6d40-4c98-8e08-ed4ff01ec917.jpg)

For our project we used MSP432P401R Board a microcontroller from Texas Instruments based on a 32-bit ARM Cortex-M4 CPU. The Educational Booster Pack with its additional hardware peripherals is also required.
From the board we can gain informations about the temperature and the brightness of the environment. In addition to this, the board gives power to the irrigation system and the water level sensor and collects data from them. The data collected from the sensors on the board and from the water level sensor are displayed on the little screen in the greenhouse.

#### Water level sensor
![8576540a-a3fd-466c-9a5e-a5127a94cb02](https://user-images.githubusercontent.com/125813273/220143063-c5ae17d9-58fa-4e3f-a475-b6e7afbd0945.jpg)

This component works attached to the board through cables and needs a power supply of 3.3V to operate. It sends informations to the boardd about the level of water present in the tank.

#### Irrigation system
<img width="89" alt="Screenshot 2023-02-22 at 10 49 26" src="https://user-images.githubusercontent.com/125813273/220584504-597b0181-2cb6-44c6-892a-9d59dcc60fe0.png">

This system consists in a motor servo that pumps water into a tube directioned into the plant vase. To work it needs a power supply of 5V taken from the board.

#### Humidity sensor
![5e63f190-ab91-49c9-b75f-c24799924b42](https://user-images.githubusercontent.com/125813273/220603239-5898ac4f-d226-4670-aeb2-7822c008da19.jpg)

This sensor is connected to the MSP432 and requires a power supply of 5V.

#### ESP8266mod
![b46767ec-107c-45f6-a34b-3b3b8a16733a](https://user-images.githubusercontent.com/125813273/220142992-f34b25a0-e398-498a-a5af-964661af01b0.jpg)

In our project the ESP8266 is used as a Serial-to-WiFi bridge linked to a telegram bot ( [greenhouse_mgmt_bot](https://t.me/greenhouse_mgmt_bot) ). It gains its power from a microusb cable connected to a computer and it is linked to the msp430 board through a female-female cable. 
In order to connect it to wifi we need to write a code on it using the arduino ide. 

#### Breadboard and cables

Since more than one of the components needed a power supply from the board, we linked them to a breadboard in order to connect them all at once.

### Software requirements:
- [MSP Driver Library](https://www.ti.com/tool/MSPDRIVERLIB) 
- [MSP Graphics Library](https://www.ti.com/tool/MSP-GRLIB)
- [CCSTUDIO](https://www.ti.com/tool/CCSTUDIO)
- [Arduino ide](https://www.arduino.cc/en/software)
- [CH340G Driver](https://sparks.gogo.co.nz/ch340.html)
- [libraries for the arduino ide](https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fsuperedo.it%2Fwp-content%2Fuploads%2F2020%2F06%2F26537922438_9808e3a404_k.jpg&f=1&nofb=1&ipt=dd2b137720bbd854a709182a1514d4c9b1cc70f703b550fc87ca92d29decc15c&ipo=images)

## USER INTERACTION
The user, through this object can understand whether the plant is located in an environment that consents it to live properly. The plant owner can see the information on the display inside the greenhouse and adjust the parameters that are not proper (for example by moving the greenhouse in a different place). If the user is far from the greenhouse he can still check if the plant is in good condition from a telegram bot called [greenhouse_mgmt_bot](https://t.me/greenhouse_mgmt_bot).

## Presentation (video + slides)
A demonstration on how this project works can be viewed on [YouTube](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwibjbjdn6T9AhUxRfEDHa1FBm8QyCl6BAgZEAM&url=https%3A%2F%2Fwww.youtube.com%2Fwatch%3Fv%3DdQw4w9WgXcQ&usg=AOvVaw0aHtehaphMhOCAkCydRLZU).
For additional infos check the [project slides](https://docs.google.com/presentation/d/1NzTzK7F4OtdL91AdtIMIIC9X2LwWOCKmxzR6OWZkVuw/edit#slide=id.p).

## HOW DOES IT WORK?
- led : we toggle led for two different actions
    - the blue one is used when the water pump is active 
    - we toggle the red led when the water level is under 15% 
    - initialize PWM timer:
    ```C
    /* modules/led/led.c */
    Timer_A_CompareModeConfig compareConfig_PWM = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output but
        50                                        // 50% Duty Cycle
        };
        
    const Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,                      // SMCLK = 3 MhZ
        TIMER_A_CLOCKSOURCE_DIVIDER_12,         // SMCLK/12 = 250 KhZ
        100000,                                  // 40 ms tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
        };
    ```
    - configure and start timer; configure ports:
    ```C
    /* modules/led/led.c */
    /* Configures P2.4 (red led) to PM_TA0.1 for using Timer PWM to control LED */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
    GPIO_PRIMARY_MODULE_FUNCTION);
    ...
    /* Configuring Timer_A0 for Up Mode and starting */
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
    ...
    compareConfig_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
    compareConfig_PWM.compareValue = 5;
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);
    ```
    - main logic:
    ```C
    /* main.c */
    if (timer > SAMPLE_DELAY){
    ...
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
    ...
    }
    ```


- light & temperature : we use the sensors present in the boosterPack MKII to monitor temperature and brightness within the greenhouse environment.
    - for sampling light level values, we make use of HAL_OPT3001, a library given by the Texas Instruments
    - for measuring heat, we make use of HAL_TMP006, a library given by the Texas Instruments


- water & humidity & servo : external sensor that we connected by initialiting free analog pins of the BoosterPack (A0, A1, A2, A3) that on the MSP432 LaunchPad coincide with (P5.5, P5.4, P4.7, P4.5), in order to gather the sensors values.



## Group Members

- Dennis Orlando
- Giulia Modenese
- Carolina Sopranzetti
