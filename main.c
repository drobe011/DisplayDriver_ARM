/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include "init.h"

//DISPLAY UPDATE FREQUENCY (REFRESH RATE = TICK_FREQ/25) 25=# OF TOTAL SEGMENTS
#define TICK_FREQ 5000

//STATE CONSTANTS
#define TURN_ON 1
#define TURN_OFF 0

//ENABLES ADDITIONAL DEBUGGING
#define TEST

//FLAGGED BY SYSTICK_HANDLER TO UPDATE SEGMENT
volatile uint8_t updateDisplay;
//VALUE FOR CURRENT OPERATION DURING UPDATEDISPLAY
uint8_t currentDigit, currentBit;
volatile uint32_t sysTicks;

int main(void) {
    uint8_t state = TURN_ON;

	#ifdef TEST
		uint8_t digitNum = 0;	//CURRENT SINGLE DIGIT NUMBER TO DISPLAY
	#endif // TEST

	//HOLD DIGIT FONTS AND DOT DATA CONSTANTS
    const uint8_t digitData[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE}; //TODO: ADD DOT ON/OFF
    //CONTAINER FOR CURRENT DIGIT DATA
    uint8_t data[DIGITS+DOTS] = {0, 0, 0, 0, 0, 0};
    //data[0] = DASH;
    //data[1] = digitData[8];

	uint32_t currTicks = 0;
	setupPorts();
	setupTimers();

	while(1)
	{
		if (updateDisplay)
		{
			switch (state)
			{
			case TURN_ON:
				if (data[currentDigit] & (1<<currentBit)) currentSegmentOn();
				state = TURN_OFF;
				break;

			case TURN_OFF:
				if (data[currentDigit] & (1<<currentBit)) currentSegmentOff();
				if (currentDigit == (DIGITS+DOTS-1)) currentDigit = 0; //ADD DOTS TO THE MIX
				else
				{
					currentDigit++;
					currentBit++;
				}
				(currentBit == sizeof(currentBit)*8) ? currentBit = 0 : currentBit;
				state = TURN_ON;
				break;
			}
			updateDisplay = 0;
		}
		#ifdef TEST
		if (sysTicks - currTicks >= TICK_FREQ)
		{
			digitNum = (digitNum == 9) ? 0 : digitNum+1;
			data[1] = digitData[digitNum];
			currTicks = sysTicks;
		}
		#endif // TEST
    }
    return 0 ;
}
void setupPorts()
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
    uint8_t loop = 0;
    for (loop = 0; loop < SOURCEPINS; loop++)
    {
    	LPC_GPIO->DIR[sourcePin[loop].mPORT] |= (1<<sourcePin[loop].mPin);
    }
    for (loop = 0; loop < SINKPINS; loop++)
    {
    	LPC_GPIO->DIR[sinkPin[loop].mPORT] |= (1<<sinkPin[loop].mPin);
    }
}

void setupTimers()
{
//	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
//	LPC_CT16B0->PR = 2048;
//	LPC_CT16B0->MCR = 2;
//	LPC_CT16B0->MR0 = 256;
//	LPC_CT16B0->TCR = 1;
//	LPC_CT16B0->EMR |= (1<<6) | (1<<7);
//	LPC_IOCON->PIO0_9 |= (1<<1); //CT16B0_MAT1

    // Force the counter to be placed into memory
    if (SysTick_Config(SystemCoreClock / TICK_FREQ)) {
    	    while (1);                                  /* Capture error */
    }

}

void SysTick_Handler(void)
{
  sysTicks++;
  updateDisplay = 1;
}

