/*
Hamming Error-Correcting Code (ECC)
Optimized for avr-gcc 4.8.1 in Atmel AVR Studio 6.2
August 12, 2014

You should include Hamming.c, Hamming.h, and only one of the other Hamming files in your project.
The other Hamming files implement the same methods in different ways, that may be better or worse for your needs.

This was created for LoFi in the TheHackadayPrize contest.
http://hackaday.io/project/1552-LoFi

Copyright 2014 David Cook
RobotRoom.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <avr/pgmspace.h>

#include "Hamming.h"

/****************************/
/*                          */
/* Constants and structures */
/*                          */
/****************************/

#ifndef null
#define null ((void*) 0)
#endif

// If transmitting/writing only, you don't need to include this file.
// If receiving/reading, then this provides the methods to correct bit errors.

#define UNCORRECTABLE	0xFF
#define ERROR_IN_PARITY	0xFE
#define NO_ERROR		0x00


/****************************/
/*                          */
/*     Global variables     */
/*                          */
/****************************/

// Private table. Faster and more compact than multiple if statements.
static const byte _hammingCorrect128Syndrome[16] PROGMEM =
{
	NO_ERROR,			// 0
	ERROR_IN_PARITY,	// 1
	ERROR_IN_PARITY,	// 2
	0x01,				// 3
	ERROR_IN_PARITY,	// 4
	0x02,				// 5
	0x04,				// 6
	0x08,				// 7
	ERROR_IN_PARITY,	// 8
	0x10,				// 9
	0x20,				// 10
	0x40,				// 11
	0x80,				// 12
	UNCORRECTABLE,		// 13
	UNCORRECTABLE,		// 14
	UNCORRECTABLE,		// 15
};

/****************************/
/*                          */
/*      Private methods     */
/*                          */
/****************************/

// Give a pointer to a received byte,
// and given a nibble difference in parity (parity ^ calculated parity)
// this will correct the received byte value if possible.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 3 means corrections not possible
static byte HammingCorrect128Syndrome(byte* value, byte syndrome)
{
	// Using only the lower nibble (& 0x0F), look up the bit
	// to correct in a table
	byte correction = pgm_read_byte(&(_hammingCorrect128Syndrome[syndrome & 0x0F]));

	if (correction != NO_ERROR)
	{
		if (correction == UNCORRECTABLE || value == null)
		{
			return 3; // Non-recoverable error
		}
		else
		{
			if ( correction != ERROR_IN_PARITY)
			{
				*value ^= correction;
			}

			return 1; // 1-bit recoverable error;
		}
	}

	return 0; // No errors
}


/****************************/
/*                          */
/*      Public  methods     */
/*                          */
/****************************/

// Given a pointer to a received byte and the received parity (as a lower nibble),
// this calculates what the parity should be and fixes the received value if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 3 means corrections not possible
byte HammingCorrect128(byte* value, nibble parity)
{
	byte syndrome;

	if (value == null)
	{
		return 3; // Non-recoverable error
	}

	syndrome = HammingCalculateParity128(*value) ^ parity;

	if (syndrome != 0)
	{
		return HammingCorrect128Syndrome(value, syndrome);
	}

	return 0; // No errors
}


// Given a pointer to a first value and a pointer to a second value and
// their combined given parity (lower nibble first parity, upper nibble second parity),
// this calculates what the parity should be and fixes the values if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 2 means two corrected errors
// 3 means corrections not possible
byte HammingCorrect2416(byte* first, byte* second, byte parity)
{
	byte syndrome;

	if (first == null || second == null)
	{
		return 3; // Non-recoverable error
	}

	syndrome = HammingCalculateParity2416(*first, *second) ^ parity;

	if (syndrome != 0)
	{
		return HammingCorrect128Syndrome(first, syndrome) + HammingCorrect128Syndrome(second, syndrome >> 4);
	}

	return 0; // No errors
}
