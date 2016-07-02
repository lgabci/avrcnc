#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "lib/atmel.h"
#include "lib/lcd_hd44780.h"

#define PWMPORT B,1		/* PWM port			*/
#define OCRPORT OCR1A		/* OCR				*/

#define PINFORWARD D,7		/* forward pin			*/
#define PINREVERSE B,0		/* reverse pin			*/

#define MAXPWM   255				/* max PWM value		*/

#define I0PORT D,2		/* INT0 port			*/
#define I1PORT D,3		/* INT1 port			*/

volatile unsigned long int	pos = 0;

/* interrupt service rutin for INT0
*/
ISR(INT0_vect) {
  pos ++;
}

/* initialize PWM
parameters:	-
*/
void initPWM() {
  LOW(PWMPORT);
  OUTPUT(PWMPORT);
  LOW(PINFORWARD);
  OUTPUT(PINFORWARD);
  LOW(PINREVERSE);
  OUTPUT(PINREVERSE);

  ICR1 = MAXPWM;

  TCCR1A = TCCR1A | (1 << COM1A1);			/* non-inverting mode	*/
  // TCCR1A = TCCR1A | (1 << COM1A1) | (1 << COM1A0);	/* inverting mode	*/

  //TCCR1B = TCCR1B | (1 << WGM13) | (1 << WGM12);	/* fast PWM, TOP = ICR1	*/
  //TCCR1A = TCCR1A | (1 << WGM11);			/* fast PWM, TOP = ICR1	*/
  //TCCR1B = TCCR1B | (1 << WGM12);			/* fast PWM, 8 bit	*/
  //TCCR1A = TCCR1A | (1 << WGM10);			/* fast PWM, 8 bit	*/
  TCCR1B = TCCR1B | (1 << WGM13);	/* phase and freq correct, TOP = ICR1	*/

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
  char text[8];		/* max 3 digits + tailing zero	*/
  char len;

  text[0] = ' ';

  if (w == 0) {		/* width = 0, switch off	*/
    OCRPORT = 0;

    if (brake) {		/* braking		*/
      HIGH(PINREVERSE);
      HIGH(PINFORWARD);

      text[0] = 'B';
    }
    else {			/* free-wheeling	*/
      LOW(PINREVERSE);
      LOW(PINFORWARD);
    }

    text[1] = ' ';
  }
  else if (fwd) {		/* forward		*/
    OCRPORT = 0;
    LOW(PINREVERSE);
    HIGH(PINFORWARD);
    OCRPORT = w;

    text[1] = '+';
  }
  else {			/* reverse		*/
    OCRPORT = 0;
    LOW(PINFORWARD);
    HIGH(PINREVERSE);
    OCRPORT = w;

    text[1] = '-';
  }

  itoa(w, &text[2], 10);
  len = strlen(text);
  text[(int)len] = ' ';
  text[len + 1] = ' ';
  text[len + 2] = '\0';

  setPosLCD(0, 0);
  writeLCD(text);
}

int main(void) {
  unsigned char i;
  char fwd;
  char brake;

  initPWM();

  initLCD();
  switchOnLCD(0, 0);

  LOW(I0PORT);				/* make input for INT ports	*/
  INPUT(I0PORT);
  LOW(I1PORT);
  INPUT(I1PORT);

  GICR = GICR | _BV(INT0) | _BV(INT1);		/* enable INT0 and INT1	*/
  MCUCR = MCUCR | _BV(ISC00) | _BV(ISC10);	/* any change		*/
  sei();					/* enable interrupts	*/

  fwd = 1;
  brake = 0;
  while (1) {
    char buf[8];

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

    itoa(pos, buf, 10);
    setPosLCD(0, 1);
    writeLCD(buf);

  }
}
