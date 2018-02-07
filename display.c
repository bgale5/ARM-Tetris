#include "display.h"
#include "registers.h"
#include "general.h"

unsigned char LCDmat[1024];

void lcdinit(void)
{
	CSport &= ~CSbit;
	RSTport &= ~RSTbit;
	DelayMs(70);
	RSTport |= RSTbit;
	DelayMs(70);
	outcmd(0xA2);
	outcmd(0xA0);
	outcmd(0xC0);
	outcmd(0xC8);
	outcmd(0x20 | 0x3);
	outcmd(0x28 | 0x4);
	DelayMs(70);
	outcmd(0x28 | 0x6);
	DelayMs(70);
	outcmd(0x28 | 0x7);
	DelayMs(70);
	outcmd(0x26);
	outcmd(0x81);
	outcmd(31);  //#31 or 0x31?
	outcmd(0x40);
	outcmd(0xAF);
	CSport |= CSbit;
	contrast(5, 50);
	outcmd(0xF0);
}

void outcmd(unsigned char cmd)
{
	A0port &= ~A0bit;
	outchar(cmd & 0xFF);
}

void outdat(unsigned char dat)
{
	A0port |= A0bit;
	outchar(dat & 0xFF);
}

void outchar(unsigned char cmd)
{
	CSport &= ~CSbit;
	for (int i = 0; i < 8; i++) {
		if ((cmd & 0x80) == 0)
			DATport &= ~DATbit;
		else
			DATport |= DATbit;
		CLKport |= CLKbit;
		CLKport &= ~CLKbit;
		cmd = cmd << 1;
	}
	CSport |= CSbit;
}

void contrast(unsigned char resratio, unsigned char contrast)
{
	if (resratio > 7)
		resratio = 7;
	if (contrast > 63)
		contrast = 63;
	outcmd(resratio | 0x20);
	outcmd(0x81);
	outcmd(contrast);
}

void pixel(unsigned char x, unsigned char y, unsigned char colour)
{
	if (y < 33)
		y += 32;
	else
		y -= 32;
	y--;
	//x--;
	if (colour == WHITE)
		LCDmat[(y / 8) * 128 + x] |= (1 << (y % 8));
	else
		LCDmat[(y / 8) * 128 + x] &= ~(1 << (y % 8));
}

void block(unsigned char x, unsigned char y, unsigned char colour)
{
	for (unsigned char i = x; i < x + 5; i++) {
		for (unsigned char j = y; j < y + 5; j++) {
			pixel(i, j, colour);
		}
	}
}

void refresh(void)
{
	for (int y = 0; y <= 8; y++) { // y< 8?
		outcmd(y | 0xB0);
		outcmd(0);
		outcmd(0x10);
		for (int x = 0; x <= 128; x++) { // x <=128?
			outdat(LCDmat[y * 128 + x]);
		}
	}
}

void blank(void)
{
	for (int i = 0; i < 1024; i++) {
		LCDmat[i] = 0;
	}
}
