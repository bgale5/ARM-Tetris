#ifndef GENERAL_H
#define GENERAL_H

#define true 1
#define false 0

void DelayMs(unsigned int numLoops);

/*
 * Quick and dirty rand() implementation
 */
unsigned int rand(unsigned int *seed);

void beep(int ms);

#endif
