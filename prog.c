#ifndef F_CPU
#define F_CPU 8000000
#endif

#include "atmega8a.h"
#include <avr/io.h>
#include <util/delay.h>

#define MAXVALUE 100

void initPWM() {
  DDRB = DDRB | (1 << DDRB7) | (1 << DDRB6) | (1 << DDRB2) | (1 << DDRB1);
  PORTB = 0x00;

  ICR1 = MAXVALUE;

  TCCR1A = TCCR1A | (1 << COM1B1) | (1 << COM1A1);	/* non-inverting mode	*/
  // TCCR1A = TCCR1A | (1 << COM1A1) | (1 << COM1A0);	/* inverting mode	*/

  //TCCR1B = TCCR1B | (1 << WGM13) | (1 << WGM12);	/* fast PWM, TOP = ICR1	*/
  //TCCR1A = TCCR1A | (1 << WGM11);			/* fast PWM, TOP = ICR1	*/
  TCCR1B = TCCR1B | (1 << WGM13);	/* phase and freq correct, TOP = ICR1	*/

  //TCCR1B = TCCR1B | (1 << CS10);		/* CLKio / 1 - no prescalar	*/
  //TCCR1B = TCCR1B | (1 << CS11);			/* CLKio / 8		*/
  TCCR1B = TCCR1B | (1 << CS11) | (1 << CS10);		/* CLKio / 64		*/
  //TCCR1B = TCCR1B | (1 << CS12);			/* CLKio / 256		*/
  //TCCR1B = TCCR1B | (1 << CS12) | (1 << CS10);	/* CLKio / 1024		*/
}

void setPWM(signed char w, char brake) {
  if (brake) {			/* braking: short circuit motor pins		*/
    PORTB = PORTB & ~(1 << PB7);	/* first: switch off	*/
    PORTB = PORTB & ~(1 << PB6);

    OCR1A = MAXVALUE;		/* switch on		*/
    OCR1B = MAXVALUE;
  }
  else if (w < 0) {		/* forward		*/
    OCR1A = 0;			/* first: switch off	*/
    PORTB = PORTB & ~(1 << PB7);

    OCR1B = -w;			/* switch on		*/
    PORTB = PORTB | (1 << PB6);
  }
  else if (w > 0) {		/* reverse		*/
    OCR1B = 0;			/* first: switch off	*/
    PORTB = PORTB & ~(1 << PB6);

    OCR1A = w;			/* switch on		*/
    PORTB = PORTB | (1 << PB7);
  }
  else {			/* switch off		*/
    OCR1B = 0;
    OCR1A = 0;
    PORTB = PORTB & ~(1 << PB6);
    PORTB = PORTB & ~(1 << PB7);
  }
}

int main(void) {
  signed char i;
  char j;
  signed char step;

  initPWM();

/*
  while (1) {
    for (i = 50; i > 0; i --) {
      setPWM(50, 0);
      _delay_ms(30);
      setPWM(0, 1);
      _delay_ms(1000);
    }
  }
*/

  step = 10;
  i = 0;
  while (1) {
    setPWM(i, 0);

    _delay_ms(1000);

    if (i == MAXVALUE || i == -MAXVALUE) {
      i = 0;
      step = -step;
      setPWM(i, 1);
      _delay_ms(2000);
    }

    i = i + step;
  }

}
