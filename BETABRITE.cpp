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
#include <WProgram.h>
#ifndef NODATEFUNCTIONS
#include "RTClib.h"
#endif
#include "BETABRITE.h"
#define BB_BETWEEN_COMMAND_DELAY 110

BETABRITE::BETABRITE ( uint8_t receivePin, uint8_t transmitPin, const char Type, const char Address[2] ) : NewSoftSerial ( receivePin, transmitPin )
{
	begin ( 9600 );
	_type = Type;
	if ( Address )
	{
		_address[0] = Address[0];
		_address[1] = Address[1];
	}
	else
	{
		_address[0] = '0';
		_address[1] = '0';
	}
}

BETABRITE::~BETABRITE ( void )
{
}

void BETABRITE::WriteTextFile ( const char Name, const char *Contents, const char initColor, const char Position, const char Mode, const char Special )
{

	BeginCommand ( );
	BeginNestedCommand ( );
	WriteTextFileNested ( Name, Contents, initColor, Position, Mode, Special );
	EndCommand ( );
}

void BETABRITE::WriteTextFileNested ( const char Name, const char *Contents, const char initColor, const char Position, const char Mode, const char Special )
{
	print ( BB_CC_WTEXT );
	print ( Name );
	print ( BB_ESC ); print ( Position ); print ( Mode );
	if ( BB_DM_SPECIAL == Mode ) print ( Special );
	if ( initColor != BB_COL_AUTOCOLOR )
	{
		print ( BB_FC_SELECTCHARCOLOR );
		print ( initColor );
	}
	print ( (char *)Contents );
}

void BETABRITE::WritePriorityTextFile ( const char *Contents, const char initColor, const char Position, const char Mode, const char Special )
{
	WriteTextFile ( BB_PRIORITY_FILE_LABEL, Contents, initColor, Position, Mode, Special );
}

void BETABRITE::WritePriorityTextFileNested ( const char *Contents, const char initColor, const char Position, const char Mode, const char Special )
{
	WriteTextFileNested ( BB_PRIORITY_FILE_LABEL, Contents, initColor, Position, Mode, Special );
}

void BETABRITE::CancelPriorityTextFile ( void )
{
	BeginCommand ( );
	BeginNestedCommand ( );
	print ( BB_CC_WTEXT );
	print ( BB_PRIORITY_FILE_LABEL );
	EndCommand ( );
}

void BETABRITE::WriteStringFile ( const char Name, const char *Contents )
{
	BeginCommand ( );
	BeginNestedCommand ( );
	WriteStringFileNested ( Name, Contents );
	EndCommand ( );
}

void BETABRITE::WriteStringFileNested ( const char Name, const char *Contents )
{
	print ( BB_CC_WSTRING );
	print ( Name );
	print ( (char *)Contents );
}

void BETABRITE::BeginCommand ( void )
{
	Sync ( ); print ( BB_SOH ); print ( _type ); print ( _address[0] ); print ( _address[1] );
}

void BETABRITE::BeginNestedCommand ( void )
{
	print ( BB_STX );
}

void BETABRITE::EndCommand ( void )
{
	print ( BB_EOT );
}

void BETABRITE::EndNestedCommand ( void )
{
	print ( BB_ETX );
}

void BETABRITE::DelayBetweenCommands ( void )
{
	delay ( BB_BETWEEN_COMMAND_DELAY );
}

#ifndef NODATEFUNCTIONS
void BETABRITE::SetDateTime ( DateTime now, bool UseMilitaryTime )
{
	char		dow, strbuff[3];
	uint8_t		hour, minute, month, day;
	uint16_t	year;

	dow = now.dayOfWeek ( );
	dow += '1';
	hour = now.hour ( );
	minute = now.minute ( );
	month = now.month ( );
	day = now.day ( );
	year = now.year ( );

	BeginCommand ( );
	BeginNestedCommand ( );
	DelayBetweenCommands ( );
	print ( BB_CC_WSPFUNC );
	print ( ' ' );
	if ( hour <= 9 )
	{
		print ( '0' );
	}
	itoa ( hour, strbuff, 10 );
	print ( strbuff );
	if ( minute <= 9 )
	{
		print ( '0' );
	}
	itoa ( minute, strbuff, 10 );
	print ( strbuff );
	EndNestedCommand ( );
	BeginNestedCommand ( );
	DelayBetweenCommands ( );
	print ( BB_CC_WSPFUNC );
	print ( '\047' );
	print ( UseMilitaryTime ? 'M' : 'S' );
	EndNestedCommand ( );
	BeginNestedCommand ( );
	DelayBetweenCommands ( );
	print ( BB_CC_WSPFUNC );
	print ( '&' );
	print ( dow );
	EndNestedCommand ( );
	BeginNestedCommand ( );
	DelayBetweenCommands ( );
	print ( BB_CC_WSPFUNC );
	print ( ';' );
	if ( month <= 9 )
	{
		print ( '0' );
	}
	itoa ( month, strbuff, 10 );
	print ( strbuff );
	if ( day <= 9 )
	{
		print ( '0' );
	}
	itoa ( day, strbuff, 10 );
	print ( strbuff );
	if ( year < 2000 ) print ( "00" );
	else
	{
		year -= 2000;
		if ( year <= 9 )
		{
			print ( '0' );
		}
		itoa ( year, strbuff, 10 ); // 2 digits for the foreseeable future, and positive
		print ( strbuff );
	}
	EndNestedCommand ( );
	EndCommand ( );
}
#endif

void BETABRITE::Sync ( void )
{
  for ( char i=0;i<5;i++ ) print ( BB_NUL );
}
