# WATERING GREENHOUSE - Project for the course "Embedded Software for the Internet of Things" (group 19)

This project consists in a greenhouse connected to a display which provides informations about the environment inside the planthouse (temperature and brightness) and about the plants living inside of it (soil humidity). In addition to the monitoring process, the greenhouse is able to water the plants when needed and to notify the plant owner when compares any issue (eg. water tank needs to be refilled). 
Our aim? Keeping a plant alive! 

## FEATURES:
The features implemented and tested in the project are the following:

- checks the temperature inside the greenhouse;
- checks the brightness of the environment;
- checks if the soil is humid enough; 
- erogates water from a water tank when the plant needs it;
- notifies the plant owner when the water tank is empty;
- displays informations about temperature/brightness of the environment and level of water in the tank
- if the tank is almost empty a red led lights up in the greenhouse  

## REQUIREMENTS:

### Hardware:
#### Launchpad (temperature sensor, brightness sensor, display, button)
![81GuSUkudzL](https://user-images.githubusercontent.com/125813273/220139784-b5718920-77d2-40c6-93ea-872a60726643.jpg)
 
#### water level sensor
![waterlevelsensor](https://user-images.githubusercontent.com/125813273/220141817-7cab88d9-e783-4cad-9ce1-f1c89f1a9d8f.jpg)

#### irrigation system
#### esp8266mod
![Unknown](https://user-images.githubusercontent.com/125813273/220141856-1f3300bc-05b2-4262-a33d-b47dd46d8edc.jpeg)

#### humidity sensor
#### breadboard and cables

### Software requirements:
- [MSP Driver Library](https://www.ti.com/tool/MSPDRIVERLIB) 
- [MSP Graphics Library](https://www.ti.com/tool/MSP-GRLIB)
- [CCSTUDIO](https://www.ti.com/tool/CCSTUDIO)

## USER INTERACTION
The user, through this object can understand whether the plant is located in an environment that consents it to live properly. The plant owner can see the information on the display inside the greenhouse and adjust the parameters that are not proper (for example by moving the greenhouse in a different place). If the user is far from the greenhouse he can still check if the plant is in good condition from a telegram bot called [greenhouse_mgmt_bot](https://t.me/greenhouse_mgmt_bot).

## Presentation (video + slides)
A demonstration on how this project works can be viewed on [YouTube](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwibjbjdn6T9AhUxRfEDHa1FBm8QyCl6BAgZEAM&url=https%3A%2F%2Fwww.youtube.com%2Fwatch%3Fv%3DdQw4w9WgXcQ&usg=AOvVaw0aHtehaphMhOCAkCydRLZU).
For additional infos check the [project slides](https://docs.google.com/presentation/d/1NzTzK7F4OtdL91AdtIMIIC9X2LwWOCKmxzR6OWZkVuw/edit#slide=id.p).

## Group Members
- Dennis Orlando
- Giulia Modenese
- Carolina Sopranzetti

##### Glossario:
- "l'amico": MSP432P401R Board
- "amici dell'amico": elementi, connessi all'amico, che interagiranno con esso

### Todo:
- [ ] Definire quali elementi andranno ad interagire con l'amico
- [ ] Implementare il codice per interagire con l'amico
- [ ] Connettere effettivamente l'amico ai suoi amici

