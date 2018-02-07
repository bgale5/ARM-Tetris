#include "registers.h"
#include "general.h"

void DelayMs(unsigned int numLoops)
{
unsigned int lp;
unsigned int i;

for(lp=0; lp<numLoops; lp++)
	for (i=0; i<=4000; i++) ;
}

// Generate a random-enough value based on the current system clock
unsigned int rand(unsigned int *seed)
{
	*seed += NVIC_ST_CURRENT;
	return NVIC_ST_CURRENT + *seed;
}

void beep(int ms)
{
	TIMER0_CTL |= 0x1;
	DelayMs(ms);
	TIMER0_CTL &= ~0x1;
}
