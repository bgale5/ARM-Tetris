#include "setup.h"
#include "registers.h"
#include "display.h"

extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

#define buttonL 0x4
#define buttonR 0x8
#define buttonF 0x10
#define PWMpin 0x1

void setupGPIOA(void)
{
	//Display
	SYSCTL_RCGCGPIO |= 0x1;
	while((SYSCTL_PRGPIO & 0x1) != 0x1);
	GPIOA_DIR |= (CSbit | RSTbit | A0bit);
	GPIOA_AFSEL &= ~(CSbit | RSTbit | A0bit);
	GPIOA_DEN |= (CSbit | RSTbit | A0bit);
	//Buttons
	GPIOA_DIR &= ~(buttonL | buttonR | buttonF);
	GPIOA_AFSEL &= ~(buttonL | buttonR | buttonF);
	GPIOA_DEN |= (buttonL | buttonR | buttonF);
}

void onGreen(void)
{
	GPIOE_DATA &= ~BLGreenBit;
}

void setupGPIOE(void)
{
	SYSCTL_RCGCGPIO |= 0x10;
	while((SYSCTL_PRGPIO & 0x10) != 0x10);
	GPIOE_DIR |= CLKbit | DATbit | BLGreenBit;
	GPIOE_AFSEL &= ~(CLKbit | DATbit | BLGreenBit);
	GPIOE_DEN |= (CLKbit | DATbit | BLGreenBit);
}

void setupGPIOB(void)
{
	SYSCTL_RCGCGPIO |= 0x2;
	while ((SYSCTL_PRGPIO & 0x2) != 0x2);
	GPIOB_DIR |= PWMpin;
	GPIOB_AFSEL |= PWMpin;
	GPIOB_PCTL &= ~0xF;
	GPIOB_PCTL |= 0x7;
	GPIOB_DEN |= PWMpin;
}

void setupPWM(void)
{
	SYSCTL_RCGCTIMER |= 0x4;
	TIMER0_CTL &= ~0x1;
	TIMER0_CTL &= ~0x40;
	TIMER0_CFG = 0x4;
	TIMER0_TAMR |= 0x8;
	TIMER0_TAILR = 0xFFFFE;
	TIMER0_TAMATCH = 0xFFFE / 2;
	
}

void setupInterrupts(void)
{
	DisableInterrupts();
	GPIOA_IM &= ~(buttonL | buttonR | buttonF);
	GPIOA_IS &= ~(buttonL | buttonR | buttonF);
	GPIOA_IEV &= ~(buttonL | buttonR | buttonF);
	GPIOA_IBE &= ~(buttonL | buttonR | buttonF);
	GPIOA_IM |= (buttonL | buttonR | buttonF);
	NVIC_EN0 |= 0x1;
	EnableInterrupts();
}

void setupSystick(unsigned char divisor)
{
	NVIC_ST_CTRL &= ~0x1;
	NVIC_ST_RELOAD = 0xFFFFFF / divisor;
	NVIC_ST_CURRENT = 1;
	NVIC_ST_CTRL |= (0x2 | 0x4);
	NVIC_ST_CTRL |= 0x1;
	
}
