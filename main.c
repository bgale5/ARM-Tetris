#include "registers.h"
#include "display.h"
#include "setup.h"
#include "general.h"
#include "game.h"

int main(void)
{
	setupGPIOA();
	setupGPIOE();
	setupGPIOB();
	setupInterrupts();
	setupSystick(2);
	setupPWM();
	onGreen();
	lcdinit();
	blank();
	drawGameBorder();
	refresh();
	gameLoop();
}
