/* ATMEL port macros for port manipulation	*/

#ifndef __atmel_h__
#define __atmel_h__

/* concatenate evaluated macro strings		*/
#define _CON(a, b)		a ## b
#define CON(a, b)		_CON(a, b)

/* port handling macros for other macros
parameters:	p	PORT/DDR/BIN
		x	numbering letter: A/B/C/D
		n	bit number: 1, 2, 3, 4, 5, 6, 7
		val	bit value: 0, 1
*/
#define _SET(p,x,n)		p ## x = p ## x | _BV(p ## x ## n)
#define _RESET(p,x,n)		p ## x = p ## x & ~ _BV(p ## x ## n)
#define _TOGGLE(p,x,n)		p ## x = p ## x ^ _BV(p ## x ## n)
#define _READ(p,x,n)		(p ## x >> p ## x ## n & 1)
//#define _WRITE(p,x,n,val)	(val) ? _SET(p,x,n) : _RESET(p,x,n)

/* port handling macros for API
parameters:	x	numbering letter: A/B/C/D
		n	bit number: 1, 2, 3, 4, 5, 6, 7
*/
#define OUTPUT(xn)		_SET(DDR,xn)
#define INPUT(xn)		_RESET(DDR,xn)
#define HIGH(xn)		_SET(PORT,xn)
#define LOW(xn)			_RESET(PORT,xn)
#define TOGGLE(xn)		_TOGGLE(PORT,xn)
#define READ(xn)		_READ(PIN,xn)
#define IS_HIGH(xn)		_READ(PIN,xn)
#define IS_LOW(xn)		(! _READ(PIN,xn))

#define SETVAL(port,mask,value)	((p) = (p) & ~(mask) | (value))

#endif	/* #ifndef __atmel_h__	*/
