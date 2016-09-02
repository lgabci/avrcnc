#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lib/lcd_hd44780.h"

unsigned char val[] = {1, 2, 4, 8};
//unsigned char val[] = {3, 6, 12, 9};
//unsigned char val[] = {1, 3, 2, 6, 4, 12, 8, 9};

void step(signed long int count, unsigned char hold) {
  static unsigned long int pos = 0;
  char s[8];

  while (count > 0) {
    pos ++;
    count --;
    PORTD = val[pos % (sizeof(val) / sizeof(val[0]))];
    _delay_ms(5);
  }
  while (count < 0) {
    pos --;
    count ++;
    PORTD = val[pos % (sizeof(val) / sizeof(val[0]))];
    _delay_ms(5);
  }
  if (! hold) {
    PORTD = 0;
  }

  ltoa(pos, s, 10);
  setPosLCD(0, 0);
  writeLCD(s);
  s[0] = ' ';
  s[1] = ' ';
  s[2] = ' ';
  s[3] = ' ';
  s[4] = ' ';
  s[5] = '\0';
  writeLCD(s);
}

int main(void) {
  initLCD();
  switchOnLCD(0, 0);

  PORTD = 0x00;
  DDRD = 0x0f;		/* set PD0..3 as output	*/

  while (1) {
    unsigned char i;

#if 0
    for (i = 0; i < 16; i ++) {
      step(1, 0);
      _delay_ms(1000);
    }
    for (i = 0; i < 16; i ++) {
      step(-1, 0);
      _delay_ms(1000);
    }
#else
    for (i = 0; i < 96; i ++) {
      step(1, 0);
      _delay_ms(25);
    }
    step(-96, 0);
    _delay_ms(2000);
    step(-96, 0);
    _delay_ms(2000);
    step(96, 0);
    _delay_ms(2000);
#endif
  }
}
