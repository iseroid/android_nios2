#include "../common/nios2_common.h"
#include "../common/adb.h"

// Adb connection.
Connection * connection;

// Event handler for the shell connection. 
void adbEventHandler( Connection *connection, adb_eventType event, uint16_t length, uint8_t *data )
{
  // Data packets contain two bytes, one for each servo, in the range of [0..180]
  if( event == ADB_CONNECTION_RECEIVE ) {
	  Serial.print( "rx\n" );
  }

}

int main( void )
{
  // Initialise the ADB subsystem.  
  ADB::init();

  // Open an ADB stream to the phone's shell. Auto-reconnect
  connection = ADB::addConnection("tcp:4567", true, adbEventHandler);  

  while( 1 ) {
	  ADB::poll();
  }

  return 0;
}

