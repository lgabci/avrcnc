/* HD44780 LCD kezelése         */

#ifndef lcd_hd44780_h
#define lcd_hd44780_h

#define BIT_CURSOR	1		/* cursor on bit	*/
#define BIT_BLINKING	0		/* blinking bit		*/

void initLCD();				/* initialize LCD	*/

void writeLCD(const char *s);		/* write string		*/

/* parameters:
_BV(BIT_CURSOR)      : cursor is visible
_BV(BIT_BLINKING)    : cursor is blinking
*/
void switchOnLCD(unsigned char cb);	/* switch on LCD	*/

void switchOffLCD();			/* switch off LCD	*/

void setPosLCD(unsigned char col, unsigned char row);	/* set position	*/

#endif
