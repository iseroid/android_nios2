#include "../common/nios2_common.h"
#include "../common/android_accessory.h"

extern "C" {
	int alt_getchar();
	int alt_putchar(int c);
	int alt_putstr(const char* str);
	void alt_printf(const char *fmt, ...);
}

static AndroidAccessory acc(
		"Google, Inc.",
		"DemoKit",
		"DemoKit Arduino Board",
		"1.0",
		"http://www.android.com",
		"0000000012345678"
		);

int main( void )
{

	Serial.print( "\nStart\n" );
	acc.powerOn();

	while( 1 ) {
		if( acc.isConnected() ) {
			uint8_t msg[10];
			int len = acc.read( msg, sizeof(msg), 1 );
			if( 0 < len ) {
				alt_printf( "len %x\n", len );
			}
		}
	}

	return 0;
}

