#ifndef F_CPU
#define F_CPU 8000000
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

#define PINFORWARD B,0		/* forward pin			*/
#define PINREVERSE D,7		/* reverse pin			*/

#define MAXPWM   255				/* max PWM value		*/

#define I0PORT D,2		/* INT0 port			*/
#define I1PORT D,3		/* INT1 port			*/

volatile signed long int	pos = 0;
volatile signed long int	err = 0;
volatile signed long int	spu = 0;

const unsigned char fwd[4] = {2, 0, 3, 1};
const unsigned char rev[4] = {1, 3, 0, 2};

volatile unsigned char prevval;

/* interrupt service rutin for INT0
*/
ISR(INT0_vect) {
  unsigned char val;

  val = (PIND & (_BV(PIND2) | _BV(PIND3))) >> PIND2;

  if (val == prevval) {			/* spurious int	*/
    spu ++;
    return;
  }
  else if (fwd[prevval] == val) {	/* forward	*/
    pos ++;
  }
  else if (rev[prevval] == val) {	/* reverse	*/
    pos --;
  }
  else {				/* error	*/
    err ++;
  }

  prevval = val;
}

/* interrupt service rutin for INT1, the same as INT0
*/
ISR(INT1_vect, ISR_ALIASOF(INT0_vect));

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
//  unsigned char i;
//  char fwd;
//  char brake;
char stopped = 0;
unsigned long int t = 0;
//signed int speed;

  initPWM();

  initLCD();
  switchOnLCD(0, 0);

  LOW(I0PORT);				/* make input for INT ports	*/
  INPUT(I0PORT);
  LOW(I1PORT);
  INPUT(I1PORT);

  prevval = (PIND & (_BV(PIND2) | _BV(PIND3))) >> PIND2;	/* init	*/

  GICR = GICR | _BV(INT0) | _BV(INT1);		/* enable INT0 and INT1	*/
  MCUCR = MCUCR | _BV(ISC00) | _BV(ISC10);	/* any change		*/
  sei();					/* enable interrupts	*/

  setPWM(255, 0, 0);
//  setPWM(255 / 62 * 44, 1, 0);
//  setPWM(255 / 62 * 20, 1, 0);

//  fwd = 1;
//  brake = 0;
  pos = 20000;
  while (1) {
    char buf[8];

#if 0
  speed = 640 - pos;
  if (speed < -255) {
    speed = -255;
  }
  if (speed > 255) {
    speed = 255;
  }
  setPWM(speed < 0 ? - speed : speed, speed >= 0, speed == 0);
#else
if (! stopped) {
  signed long int p;

  cli();
  p = pos;
  sei();

  t ++;
  if (p <= 10000) {
    stopped = 1;
    setPWM(0, 0, 1);
    _delay_ms(2000);
    initLCD();
    switchOnLCD(0, 0);
    setPWM(0, 0, 0);
//  initLCD();
//  switchOnLCD(0, 0);
//  }
//  else {
//    setPWM((10000 - pos) / 100 + 155, 1, 0);
  }
}
#endif

/* -----------------------------------------------------------------------------
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
----------------------------------------------------------------------------- */

    itoa(pos, buf, 10);
    setPosLCD(0, 1);
    writeLCD(buf);

    buf[0] = ' ';
    itoa(err, &buf[1], 10);
    writeLCD(buf);

    itoa(spu, &buf[1], 10);
    writeLCD(buf);

    itoa(t, &buf[1], 10);
    writeLCD(buf);

    buf[1] = pos % 32 == 0 ? 'X' : ' ';
    buf[2] = ' ';
    buf[3] = '\0';
    writeLCD(buf);

    _delay_ms(25);
  }
}
