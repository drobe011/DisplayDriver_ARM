/*
 * init.h
 *
 *  Created on: Dec 23, 2013
 *      Author: drob
 */

#ifndef INIT_H_
#define INIT_H_

#ifdef __USE_CMSIS
#include "LPC13Uxx.h"
#endif

#define DIGITS 4
#define DOTS 2
#define SOURCE 0
#define SINK 1

//DIGIT BITMASKS
#define A_SEG 0
#define B_SEG 1
#define C_SEG 2
#define D_SEG 3
#define E_SEG 4
#define F_SEG 5
#define G_SEG 6

#define ONE (1 << B_SEG) | (1 << C_SEG)
#define TWO (1 << A_SEG) | (1 << B_SEG) | (1 << G_SEG) | (1 << E_SEG) | (1 << D_SEG)
#define THREE (1 << A_SEG) | (1 << B_SEG) | (1 << C_SEG) | (1 << D_SEG) | (1 << G_SEG)
#define FOUR (1 << B_SEG) | (1 << C_SEG) | (1 << F_SEG) | (1 << G_SEG)
#define FIVE (1 << A_SEG) | (1 << C_SEG) | (1 << D_SEG) | (1 << F_SEG) | (1 << G_SEG)
#define SIX (1 << A_SEG) | (1 << C_SEG) | (1 << D_SEG) | (1 << E_SEG) | (1 << F_SEG) | (1 << G_SEG)
#define SEVEN (1 << A_SEG) | (1 << B_SEG) | (1 << C_SEG)
#define EIGHT (1 << A_SEG) | (1 << B_SEG) | (1 << C_SEG) | (1 << D_SEG) | (1 << E_SEG) | (1 << F_SEG) | (1 << G_SEG)
#define NINE (1 << A_SEG) | (1 << B_SEG) | (1 << C_SEG) | (1 << D_SEG) | (1 << F_SEG) | (1 << G_SEG)
#define ZERO (1 << A_SEG) | (1 << B_SEG) | (1 << C_SEG) | (1 << D_SEG) | (1 << E_SEG) | (1 << F_SEG)

//SINK PINS
#define SINKPINS 3
#define PIN_1 0
#define PIN_2 1
#define PIN_3 2

//SOURCE PINS
#define SOURCEPINS 14
#define PIN_4 0
#define PIN_5 1
#define PIN_6 2
#define PIN_9 3
#define PIN_10 4
#define PIN_12 5
#define PIN_13 6
#define PIN_15 7
#define PIN_16 8
#define PIN_17 9
#define PIN_18 10
#define PIN_19 11
#define PIN_20 12
#define PIN_21 13

#define EMPTY 0

void setupPorts();
void setupTimers();
extern uint8_t currentDigit;
extern uint8_t currentBit;

typedef struct _pin
{
    volatile uint8_t mPORT;
    volatile uint8_t mPin;
}_pin;

_pin sourcePin[SOURCEPINS] =
{
	{1, 24}, //PIN_4 **not on 1347
	{1, 23}, //PIN_5
	{0, 21}, //PIN_6
	{0, 5}, //PIN_9
	{0, 4}, //PIN_10
	{0, 3}, //PIN_12
	{1, 20}, //PIN_13
	{1, 27}, //PIN_15
	{1, 26}, //PIN_16
	{1, 22}, //PIN_17 
	{0, 15}, //PIN_18  **alt
	{0, 16}, //PIN_19
	{1, 18}, //PIN_20 *****change to 1, 25 on LPC1315
	{1, 16}  //PIN_21
};

_pin sinkPin[SINKPINS] =
{
	{1, 31}, //PIN_1
	{1, 28}, //PIN_2
	{0, 7} //PIN_3
};

typedef struct _digit
{
    uint8_t Pin[2][8];
}_digit;

_digit portMap[DIGITS+DOTS] =
{
    {{{EMPTY, PIN_6, PIN_9, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}, {EMPTY, PIN_2, PIN_2, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},},
    {{{PIN_13, PIN_10, PIN_12, PIN_12, PIN_9, PIN_13, PIN_10, EMPTY}, {PIN_2, PIN_2, PIN_2, PIN_1, PIN_1, PIN_1, PIN_1, EMPTY}},},
    {{{PIN_15, PIN_16, PIN_17, PIN_17, PIN_18, PIN_15, PIN_16, EMPTY}, {PIN_1, PIN_1, PIN_1, PIN_2, PIN_2, PIN_2, PIN_2, EMPTY}},},
    {{{PIN_21, PIN_19, PIN_20, PIN_20, PIN_18, PIN_21, PIN_19, EMPTY}, {PIN_2, PIN_2, PIN_2, PIN_1, PIN_1, PIN_1, PIN_1, EMPTY}},},
    {{{PIN_5, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}, {PIN_1, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},},
    {{{PIN_4, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}, {PIN_3, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}
};

__inline void currentSegmentOn()
{
	LPC_GPIO->SET[sinkPin[portMap[currentDigit].Pin[SINK][currentBit]].mPORT] = (1 << sinkPin[portMap[currentDigit].Pin[SINK][currentBit]].mPin);
	LPC_GPIO->SET[sourcePin[portMap[currentDigit].Pin[SOURCE][currentBit]].mPORT] = (1 << sourcePin[portMap[currentDigit].Pin[SOURCE][currentBit]].mPin);

}

__inline void currentSegmentOff()
{
	LPC_GPIO->CLR[sourcePin[portMap[currentDigit].Pin[SOURCE][currentBit]].mPORT] = (1 << sourcePin[portMap[currentDigit].Pin[SOURCE][currentBit]].mPin);
    LPC_GPIO->CLR[sinkPin[portMap[currentDigit].Pin[SINK][currentBit]].mPORT] = (1 << sinkPin[portMap[currentDigit].Pin[SINK][currentBit]].mPin);
}

#endif /* INIT_H_ */
