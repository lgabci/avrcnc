/*
 *
 * HD44780 LCD conrtoller
 *
 * to be defined:
 * F_CPU	CPU clock
 * LCDPORT	LCD wired to this port (A, B, C, D)
 *
 * wiring:
 * AVR		LCD
 * ----------	-----------------------------------------------------
 * -		ground (pin 1) -> ground
 * -		Vdd (pin 2) -> Vdd
 * -		Vee (pin 3) -> contrast
 * -		RW (pin 5) -> ground
 * -		D0..D3 (pin 7..10) -> ground
 * 0..3:	D4..D7 (pin 11..14)
 * 4:		RS (pin 4)
 * 5:		Enable (pin 6)
 * 6:		via 1K resistor to backlight positive (pin 15)
 * -		backlight ground (16) -> ground
 * 7:		- (not used)
 *
 * If don't want to control the backlight from program, then
 * pin 15 goes via 1K resistor to Vdd.
 *
 * */

#ifndef F_CPU
#define F_CPU 8000000
#endif

#define LCDPORT		C

#include <avr/io.h>
#include <util/delay.h>

#include "atmel.h"
#include "lcd_hd44780.h"

#define PIN_LCD		CON(PIN,LCDPORT)
#define DDR_LCD		CON(DDR,LCDPORT)
#define PORT_LCD	CON(PORT,LCDPORT)

#define BIT_RS		CON(CON(P,LCDPORT),4)	/* r/s bit in port	*/
#define BIT_ENABLE	CON(CON(P,LCDPORT),5)	/* enable bit in port	*/
#define BIT_BLACKLIGHT	CON(CON(P,LCDPORT),6)	/* backlight bit	*/

static void sendByte(unsigned char val, unsigned char rs);
static void sendNibble(unsigned char val, unsigned char rs);

static unsigned char backlight = 0;	/* backlight is off         */

void delay_ms(unsigned char ms) {
  while (ms --) {
    _delay_ms(1);
  }
}

static void sendNibble(unsigned char val, unsigned char rs) {
  PORT_LCD =  backlight | (rs ? _BV(BIT_RS) : 0) | _BV(BIT_ENABLE) | val & 0x0f;
  delay_ms(1);
  PORT_LCD = PORT_LCD & ~ _BV(BIT_ENABLE);
  delay_ms(2);
}

static void sendByte(unsigned char val, unsigned char rs) {
  sendNibble(val >> 4, rs);           /* high nibble          */
  sendNibble(val, rs);                /* low nibble           */
}

void initLCD() {
  unsigned char i;

  PORT_LCD = 0x00;
  DDR_LCD = 0xff;		/* bits 0..7 are outputs	*/

  _delay_ms(100);			/* wait after power on	*/

  for (i = 0; i < 3; i ++) {
    sendNibble(0b0011, 0);      /* set 8 bit mode           */
    delay_ms(5);
  }

  sendNibble(0b0010, 0);          /* set 4 bit mode           */
  delay_ms(1);

  sendByte(0b00101000, 0);        /* function set             */
  sendByte(0b00001000, 0);        /* set display off          */
  sendByte(0b00000001, 0);        /* clear screen             */
  delay_ms(5);
  sendByte(0b00000110, 0);        /* set entry mode           */

  switchOnLCD(0);
}

void sendStringLCD(const char *s) {
  while (*s != '\0') {
    sendByte(*s ++, 1);
  }
}

void switchOnLCD(unsigned char cb) {    /* switch display on    */
  backlight = _BV(BIT_BLACKLIGHT);
  sendByte(0b00001100 | cb & (BIT_CURSOR | BIT_BLINKING), 0);
}

void switchOffLCD() {           /* switch display off               */
  backlight = 0;
  sendByte(0b00001000, 0);
}

void setPosLCD(unsigned char col, unsigned char row) {  /* set position     */
  sendByte(0b10000000 | (row << 6) + col, 0);
}
