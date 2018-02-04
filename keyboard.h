/*
    This file is part of IndiaOS
    Copyright (C) 2017 - 2018 Luis David Julio Macot

    IndiaOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IndiaOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//Partly(C)Tom Fritz //

void init_keyboard(); //iniciacion del teclado



// Esc es 27 en ASCII
#define ESC			27
// Backspace en ASCII es '\b'
#define BACKSPACE '\b'
// tab es '\t'
#define TAB '\t'
// nueva linea es '\n'
#define ENTER '\n'
// definimos nueva linea como ENTER
#define NEWLINE ENTER

#define RETURN '\r'

// Especiales scan codes
// Esc en scancode es 1
#define KESC			1
#define	KF1		0x80
#define	KF2		(KF1 + 1)
#define	KF3		(KF2 + 1)
#define	KF4		(KF3 + 1)
#define	KF5		(KF4 + 1)
#define	KF6		(KF5 + 1)
#define	KF7		(KF6 + 1)
#define	KF8		(KF7 + 1)
#define	KF9		(KF8 + 1)
#define	KF10		(KF9 + 1)
#define	KF11		(KF10 + 1)
#define	KF12		(KF11 + 1)

// cursor
#define	KINS		0x90
#define	KDEL		(KINS + 1)
#define	KHOME		(KDEL + 1)
#define	KEND		(KHOME + 1)
#define	KPGUP		(KEND + 1)
#define	KPGDN		(KPGUP + 1)
#define	KLEFT		(KPGDN + 1)
#define	KUP		(KLEFT + 1)
#define	KDOWN		(KUP + 1)
#define	KRIGHT		(KDOWN + 1)

// "Meta" keys
#define	KMETA_ALT	0x0200	// Alt
#define	KMETA_CTRL	0x0400	// Ctrl
#define	KMETA_SHIFT	0x0800	// Shift
#define	KMETA_ANY	(KMETA_ALT | KMETA_CTRL | KMETA_SHIFT)
#define	KMETA_CAPS	0x1000	// CapsLock
#define	KMETA_NUM	0x2000	// NumLock
#define	KMETA_SCRL	0x4000	// ScrollLock

// otros key
#define	KPRNT	( KRT + 1 )
#define	KPAUSE	( KPRNT + 1 )
#define	KLWIN	( KPAUSE + 1 )
#define	KRWIN	( KLWIN + 1 )
#define	KMENU	( KRWIN + 1 )

#define	KRLEFT_CTRL		0x1D
#define	KRLEFT_SHIFT		0x2A
#define	KRCAPS_LOCK		0x3A
#define	KRLEFT_ALT		0x38
#define	KRRIGHT_ALT		0x38	
#define	KRRIGHT_CTRL		0x1D	
#define	KRRIGHT_SHIFT		0x36
#define	KRSCROLL_LOCK		0x46
#define	KRNUM_LOCK		0x45
#define	KRDEL			0x53

// definimos la interrupt al presionar una tecla
#define KEYPRESS 0x80

// maximo keyboard buffer
#define MAXKEYBUFFER 64

// Es el puerto del teclado 
#define KEYPORT 0x60


static const unsigned char asciiNonSh[] = { 0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9',
'0', '-', '=', BACKSPACE, TAB, 'q', 'w',   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   '[', ']', ENTER, 0,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
'm', ',', '.', '/', 0, 0, 0, ' ',   0, KF1,   KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
KHOME, KUP, KPGUP,'-', KLEFT, '5',   KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };


static const unsigned char asciiShift[] = { 0, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(',
')', '_', '+', BACKSPACE, TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',   '{', '}', ENTER, 0,
'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
'M', '<', '>', '?', 0, 0, 0, ' ',   0, KF1,   KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
KHOME, KUP, KPGUP, '-', KLEFT, '5',   KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };
