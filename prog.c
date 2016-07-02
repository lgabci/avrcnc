#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lib/atmel.h"
#include "lib/lcd_hd44780.h"

#define PWMPORT B,1		/* PWM port			*/
#define OCRPORT OCR1A		/* OCR			*/

#define PINFORWARD D,7		/* forward pin			*/
#define PINREVERSE B,0		/* reverse pin			*/

#define MAXPWM   255				/* max PWM value		*/

/* initialize PWM
*/
void initPWM() {
  OUTPUT(PWMPORT);
  OUTPUT(PINFORWARD);
  OUTPUT(PINREVERSE);

  PORTB = 0x00;

  ICR1 = MAXPWM;

  TCCR1A = TCCR1A | (1 << COM1B1) | (1 << COM1A1);	/* non-inverting mode	*/
  // TCCR1A = TCCR1A | (1 << COM1A1) | (1 << COM1A0);	/* inverting mode	*/

  TCCR1B = TCCR1B | (1 << WGM13) | (1 << WGM12);	/* fast PWM, TOP = ICR1	*/
  TCCR1A = TCCR1A | (1 << WGM11);			/* fast PWM, TOP = ICR1	*/
  // TCCR1B = TCCR1B | (1 << WGM12) | (1 << WGM10);	/* fast PWM, 8 bit	*/
  // TCCR1B = TCCR1B | (1 << WGM13);	/* phase and freq correct, TOP = ICR1	*/

  TCCR1B = TCCR1B | (1 << CS10);		/* CLKio / 1 = no prescalar	*/
  //TCCR1B = TCCR1B | (1 << CS11);			/* CLKio / 8		*/
  //TCCR1B = TCCR1B | (1 << CS11) | (1 << CS10);	/* CLKio / 64		*/
  //TCCR1B = TCCR1B | (1 << CS12);			/* CLKio / 256		*/
  //TCCR1B = TCCR1B | (1 << CS12) | (1 << CS10);	/* CLKio / 1024		*/
}

/* set PWM
parameters:	w:	pulse width
		fwd:	forward/reverse
		brake:	braking (only if w = 0)
*/
void setPWM(unsigned char w, char fwd, char brake) {
  char num[4];		/* max 3 digits + tailing zero	*/

//  sprintf(num, "%d", w);
  num[0] = 'A';		//
  num[1] = 0;		//
  setPosLCD(0, 0);
  writeLCD(num);

  if (w == 0) {		/* width = 0, switch off	*/
    OCRPORT = 0;

    if (brake) {		/* braking		*/
      HIGH(PINREVERSE);
      HIGH(PINFORWARD);
    }
    else {			/* free-wheeling	*/
      LOW(PINREVERSE);
      LOW(PINFORWARD);
    }
  }
  else if (fwd) {		/* forward		*/
    OCRPORT = 0;
    LOW(PINREVERSE);
    HIGH(PINFORWARD);
    OCRPORT = w;
  }
  else {			/* reverse		*/
    OCRPORT = 0;
    LOW(PINFORWARD);
    HIGH(PINREVERSE);
    OCRPORT = w;
  }
}

int main(void) {
  unsigned char i;
  char fwd;
  char brake;

  initPWM();

  initLCD();
  switchOnLCD(_BV(BIT_CURSOR) | _BV(BIT_BLINKING));

  fwd = 1;
  brake = 0;
  while (1) {
    for (i = 0; i < MAXPWM / 5; i ++) {
      setPWM(i, fwd, 0);
      _delay_ms(10);
    }

    _delay_ms(4000);

    setPWM(0, fwd, brake);
    _delay_ms(2000);

    fwd = ! fwd;
    if (fwd) {
      brake = ! brake;
    }
  }
}
