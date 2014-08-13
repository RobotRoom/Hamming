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
/*     Global variables     */
/*                          */
/****************************/

// This contains all of the precalculated parity values for a nibble (4 bits).
static const byte _hammingCalculateParityLowNibble[] PROGMEM =
{ 0,  3,  5,  6,  6,  5,  3,  0,  7,  4,  2,  1,  1,  2,  4,  7 };

static const byte _hammingCalculateParityHighNibble[] PROGMEM =
{ 0,  9, 10,  3, 11,  2,  1,  8, 12,  5,  6, 15,  7, 14, 13,  4 };


/****************************/
/*                          */
/*      Public  methods     */
/*                          */
/****************************/

// Given a byte to transmit, this returns the parity as a nibble
nibble HammingCalculateParity128(byte value)
{
	return pgm_read_byte(&(_hammingCalculateParityLowNibble[value&0x0F])) ^ pgm_read_byte(&(_hammingCalculateParityHighNibble[value >> 4]));
}

// Given two bytes to transmit, this returns the parity
// as a byte with the lower nibble being for the first byte,
// and the upper nibble being for the second byte.
byte HammingCalculateParity2416(byte first, byte second)
{
	return HammingCalculateParity128(second) << 4 | HammingCalculateParity128(first);
}
