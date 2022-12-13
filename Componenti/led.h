

#ifndef COMPONENTI_LED_H_
#define COMPONENTI_LED_H_

typedef enum {
    ROSSO,
    BLU,
    VERDE,
} Luce;

void initializeLeds();
void lightSingleLed(Luce colore);


#endif /* COMPONENTI_LED_H_ */
