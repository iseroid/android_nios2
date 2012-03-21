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

#define _NIOS2_COMMON_CPP_

#include "nios2_common.h"
#include <stdarg.h>
#include <unistd.h>

// sys/alt_stdio.h だと extern "C" してないから include せずに直接書く
extern "C" {
	int alt_getchar();
	int alt_putchar(int c);
	int alt_putstr(const char* str);
	void alt_printf(const char *fmt, ...);
}

SERIAL Serial;

void SERIAL::print( const char *str )
{
	alt_putstr( str );
}

void SERIAL::println( int n, int base )
{
	alt_printf( "%x\n", n );
}

void delay( uint32_t x )
{
	usleep( x * 1000 );
}

uint32_t millis( void )
{
	return alt_nticks();
}

