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

#ifndef _HAMMING_H
#define _HAMMING_H

#ifndef _AVR_H
typedef unsigned char byte;
typedef unsigned char nibble;
#endif

/**** These are needed to transmit and receive ****/

// Given a byte to transmit, this returns the parity as a nibble
nibble HammingCalculateParity128(byte value);

// Given two bytes to transmit, this returns the parity
// as a byte with the lower nibble being for the first byte,
// and the upper nibble being for the second byte.
byte HammingCalculateParity2416(byte first, byte second);



/**** These are needed only to receive ****/

// Given a pointer to a received byte and the received parity (as a lower nibble),
// this calculates what the parity should be and fixes the received value if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 3 means corrections not possible
byte HammingCorrect128(byte* value, nibble parity);

// Given a pointer to a first value and a pointer to a second value and
// their combined given parity (lower nibble first parity, upper nibble second parity),
// this calculates what the parity should be and fixes the values if needed.
// It returns the number of bits corrected:
// 0 means no errors
// 1 means one corrected error
// 2 means two corrected errors
// 3 means corrections not possible
byte HammingCorrect2416(byte* first, byte* second, byte parity);

#endif // _HAMMING_H
