#ifndef DISPLAY_H
#define DISPLAY_H

#define CSbit 	0x80
#define RSTbit 	0x40
#define A0bit 	0x20
#define CLKbit 	0x20
#define DATbit 	0x10
#define BLGreenBit 0x8
#define CSport (*((volatile unsigned long *)0x400043FC))
#define RSTport (*((volatile unsigned long *)0x400043FC))
#define A0port (*((volatile unsigned long *)0x400043FC))
#define CLKport (*((volatile unsigned long *)0x400243FC))
#define DATport (*((volatile unsigned long *)0x400243FC))
#define WHITE 1
#define BLACK 0
#define Y_MAX 63
#define X_MAX 127

#include "display.h"
#include "registers.h"
#include "general.h"



void lcdinit(void);

void outcmd(unsigned char cmd);

void outdat(unsigned char dat);

void outchar(unsigned char cmd);

void contrast(unsigned char resratio, unsigned char contrast);

void pixel(unsigned char x, unsigned char y, unsigned char colour);

void block(unsigned char x, unsigned char y, unsigned char colour);

void refresh(void);

void blank(void);

#endif
