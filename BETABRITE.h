/* BETABRITE class for Arduino
   by Tom Stewart (tastewar.com)

   This library provides definitions and functions
   for interfacing with a BetaBrite sign through a
   Serial port

   The documentation for the protocol that this code
   is based on is (was) available at the time at:
   http://www.alpha-american.com/alpha-manuals/M-Protocol.pdf
   (c) 2011
*/

#ifndef BETABRITE_H
#define BETABRITE_H

#include <WProgram.h>
#include "BBDEFS.h"
#include "../NewSoftSerial/NewSoftSerial.h"


// following is based on Arduino forum user etracer's suggestion at
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1237301397
/* #ifdef DEVELOPMENT_MODE
#define SerialPort bbPort
#else
#define SerialPort Serial
#endif //DEVELOPMENT_MODE
*/

class BETABRITE : NewSoftSerial
{
public:
	BETABRITE ( uint8_t receivePin, uint8_t transmitPin, const char Type=BB_ST_ALL, const char Address[2]=NULL );
	~BETABRITE ( void );
	void WriteTextFile ( const char Name, const char *Contents, const char initColor=BB_COL_AUTOCOLOR, const char Position=BB_DP_TOPLINE, const char Mode=BB_DM_COMPROTATE, const char Special=BB_SDM_TWINKLE );
	void WriteTextFileNested ( const char Name, const char *Contents, const char initColor=BB_COL_AUTOCOLOR, const char Position=BB_DP_TOPLINE, const char Mode=BB_DM_COMPROTATE, const char Special=BB_SDM_TWINKLE );
	void WritePriorityTextFile ( const char *Contents, const char initColor=BB_COL_AUTOCOLOR, const char Position=BB_DP_TOPLINE, const char Mode=BB_DM_COMPROTATE, const char Special=BB_SDM_TWINKLE );
	void WritePriorityTextFileNested ( const char *Contents, const char initColor=BB_COL_AUTOCOLOR, const char Position=BB_DP_TOPLINE, const char Mode=BB_DM_COMPROTATE, const char Special=BB_SDM_TWINKLE );
	void CancelPriorityTextFile ( void );
	void WriteStringFile ( const char Name, const char *Contents );
	void WriteStringFileNested ( const char Name, const char *Contents );
  void SetMemoryConfiguration ( const char startingFile, unsigned int numFiles=26, unsigned int size=256 );
	void BeginCommand ( void );
	void BeginNestedCommand ( void );
	void EndCommand ( void );
	void EndNestedCommand ( void );
	void DelayBetweenCommands ( void );
#ifdef DATEFUNCTIONS
	void SetDateTime ( DateTime now, bool UseMilitaryTime=false );
#endif
private:
	char	_type;
	char	_address[2];
	void Sync ( void );
};

#endif //BETABRITE_H