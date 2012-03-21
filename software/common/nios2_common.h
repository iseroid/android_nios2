/*
	Copyright 2012 M.ISE

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

#ifndef _NIOS2_COMMON_H_
#define _NIOS2_COMMON_H_

#include <stdint.h>
#include <stddef.h>
#include "sys/alt_alarm.h"

#define DEC 10
#define HEX 16
#define OCT  8
#define BIN  2
#define BYTE 0

typedef uint8_t boolean;

class SERIAL {
	public:
//		void print(const String &);
		void print(const char[]);
//		void print(char, int = BYTE);
//		void print(unsigned char, int = BYTE);
//		void print(int, int = DEC);
//		void print(unsigned int, int = DEC);
//		void print(long, int = DEC);
//		void print(unsigned long, int = DEC);
//		void print(double, int = 2);

//		void println(const String &s);
//		void println(const char[]);
//		void println(char, int = BYTE);
//		void println(unsigned char, int = BYTE);
		void println(int, int = DEC);
//		void println(unsigned int, int = DEC);
//		void println(long, int = DEC);
//		void println(unsigned long, int = DEC);
//		void println(double, int = 2);
//		void println(void);

};

extern SERIAL Serial;

void delay( uint32_t ms );
uint32_t millis( void );

#endif
