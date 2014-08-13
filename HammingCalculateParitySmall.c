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

#include "Hamming.h"

/****************************/
/*                          */
/*      Public  methods     */
/*                          */
/****************************/

// This is slower than using a table, but faster than
// using the textbook method.

// Given a byte to transmit, this returns the parity as a nibble
nibble HammingCalculateParity128(byte value)
{
	// Exclusive OR is associative and commutative, so order of operations and values does not matter.
	nibble parity;

	if ( ( value & 1 ) != 0 )
	{
		parity = 0x3;
	}
	else
	{
		parity = 0x0;
	}

	if ( ( value & 2 ) != 0 )
	{
		parity ^= 0x5;
	}

	if ( ( value & 4 ) != 0 )
	{
		parity ^= 0x6;
	}

	if ( ( value & 8 ) != 0 )
	{
		parity ^= 0x7;
	}

	if ( ( value & 16 ) != 0 )
	{
		parity ^= 0x9;
	}

	if ( ( value & 32 ) != 0 )
	{
		parity ^= 0xA;
	}

	if ( ( value & 64 ) != 0 )
	{
		parity ^= 0xB;
	}

	if ( ( value & 128 ) != 0 )
	{
		parity ^= 0xC;
	}

	return parity;
}

// Given two bytes to transmit, this returns the parity
// as a byte with the lower nibble being for the first byte,
// and the upper nibble being for the second byte.
byte HammingCalculateParity2416(byte first, byte second)
{
	return (HammingCalculateParity128(second) << 4) | HammingCalculateParity128(first);
}

