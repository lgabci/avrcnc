#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  PORTD = 0x00;
  DDRD = 0x0f;		/* set PORTD0..3 as output	*/

  while (1) {
// 0001 1
// 0100 4
// 0010 2
// 1000 8
    PORTD = 1;
    _delay_ms(75);
    PORTD = 0;
    _delay_ms(450);
    PORTD = 2;
    _delay_ms(75);
    PORTD = 0;
    _delay_ms(450);
    PORTD = 4;
    _delay_ms(75);
    PORTD = 0;
    _delay_ms(450);
    PORTD = 8;
    _delay_ms(75);
    PORTD = 0;
    _delay_ms(450);
  }
}
