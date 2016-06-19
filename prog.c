#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lib/atmel.h"

#define PWMFORWARD B,2		/* PWM forward pin		*/
#define PINFORWARD D,7		/* forward pin			*/
#define OCRFORWARD OCR1B	/* forward OCR			*/

#define PWMREVERSE B,1		/* PWM reverse pin		*/
#define PINREVERSE B,0		/* reverse pin			*/
#define OCRREVERSE OCR1A	/* forward OCR			*/

#define MAXPWM   100				/* max PWM value		*/
#define MINVALUE 3				/* min value			*/
#define MAXVALUE 8				/* max value			*/

void initPWM() {
//  DDRB = DDRB | (1 << DDRB7) | (1 << DDRB6) | (1 << DDRB2) | (1 << DDRB1);
  OUTPUT(PWMFORWARD);
  OUTPUT(PINFORWARD);
  OUTPUT(PWMREVERSE);
  OUTPUT(PINREVERSE);

  PORTB = 0x00;

  ICR1 = MAXPWM;

  TCCR1A = TCCR1A | (1 << COM1B1) | (1 << COM1A1);	/* non-inverting mode	*/
  // TCCR1A = TCCR1A | (1 << COM1A1) | (1 << COM1A0);	/* inverting mode	*/

  //TCCR1B = TCCR1B | (1 << WGM13) | (1 << WGM12);	/* fast PWM, TOP = ICR1	*/
  //TCCR1A = TCCR1A | (1 << WGM11);			/* fast PWM, TOP = ICR1	*/
  TCCR1B = TCCR1B | (1 << WGM13);	/* phase and freq correct, TOP = ICR1	*/

  TCCR1B = TCCR1B | (1 << CS10);		/* CLKio / 1 - no prescalar	*/
  //TCCR1B = TCCR1B | (1 << CS11);			/* CLKio / 8		*/
  //TCCR1B = TCCR1B | (1 << CS11) | (1 << CS10);	/* CLKio / 64		*/
  //TCCR1B = TCCR1B | (1 << CS12);			/* CLKio / 256		*/
  //TCCR1B = TCCR1B | (1 << CS12) | (1 << CS10);	/* CLKio / 1024		*/
}

void setPWM(signed char w, char brake) {
  if (brake) {			/* braking: short circuit motor pins		*/
    LOW(PINFORWARD);		/* first: switch off	*/
    LOW(PINREVERSE);

    OCRFORWARD = MAXPWM;	/* switch on		*/
    OCRREVERSE = MAXPWM;
  }
  else if (w < 0) {		/* forward		*/
    LOW(PINREVERSE);		/* first: switch off	*/
    OCRREVERSE = 0;

    HIGH(PINFORWARD);		/* switch on		*/
    OCRFORWARD = -w;
  }
  else if (w > 0) {		/* reverse		*/
    LOW(PINFORWARD);		/* first: switch off	*/
    OCRFORWARD = 0;

    HIGH(PINREVERSE);		/* switch on		*/
    OCRREVERSE = w;
  }
  else {			/* switch off, no braking	*/
    LOW(PINREVERSE);
    LOW(PINFORWARD);
    OCRREVERSE = 0;
    OCRFORWARD = 0;
  }
}

int main(void) {
  signed short int i;
  signed char j;
  char k;

  initPWM();
  initLCD();
  switchOnLCD();
  sendStringLCD("Proba");

  j = 1;
  while (1) {
    for (i = MAXVALUE; i > MINVALUE; i --) {
      setPWM(j * i, 0);
      _delay_ms(100);
    }

    _delay_ms(4000);
/*
    k = 0;
    for (i = 0; i < 1000; i ++) {
      setPWM(j * (MINVALUE + k), 0);
      if (k) {
        _delay_ms(20);
      }
      else {
        _delay_ms(20);
      }
      k = ! k;
    }
*/

    setPWM(0, 0);
    _delay_ms(1000);

    j = - j;
//    if (i == MAXVALUE || i == -MAXVALUE) {
//      i = 0;
//      step = -step;
//      _delay_ms(2000);
//      setPWM(i, j = ! j);
//      _delay_ms(4000);
//    }
//
//    i = i + step;
    // if (j > 3) {
    //   j = 0;
    // }
  }

}
