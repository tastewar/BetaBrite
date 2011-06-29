/* BETABRITE definitions for Arduino
   by Tom Stewart (tastewar.com)

   This library provides definitions and functions
   for interfacing with a BetaBrite sign through a
   Serial port

   The documentation for the protocol that this code
   is based on is (was) available at the time at:
   http://www.alpha-american.com/alpha-manuals/M-Protocol.pdf
   (c) 2011
*/

#ifndef BBDEFS_H
#define BBDEFS_H

// Common ASCII character definitions used by the protocol doc

#define BB_NUL '\000'
#define BB_SOH '\001'
#define BB_STX '\002'
#define BB_ETX '\003'
#define BB_EOT '\004'
#define BB_ESC '\033'

// Sign Types
#define BB_ST_ALLVV                '\041'
#define BB_ST_SERCLK               '\042
#define BB_ST_ALPHAVISION          '\043'
#define BB_ST_ALPHAVISIONFM        '\044'
#define BB_ST_ALPHAVISIONCM        '\045'
#define BB_ST_ALPHAVISIONLM        '\046'
#define BB_ST_RESPONSE             '\060'
#define BB_ST_1LINE                '\061'
#define BB_ST_2LINE                '\062'
#define BB_ST_ALL                  '\077'
#define BB_ST_430I                 '\103'
#define BB_ST_440I                 '\104'
#define BB_ST_460I                 '\105'
#define BB_ST_ALPHAECLIPSE3600DDB  '\106'
#define BB_ST_ALPHAECLIPSE3600TAB  '\107'
#define BB_ST_LIGHTSENSOR          '\114'
#define BB_ST_790I                 '\125'
#define BB_ST_ALPHAECLIPSE3600     '\126'
#define BB_ST_ALPHAECLIPSETIMETEMP '\127'
#define BB_ST_ALPHAPREMIERE        '\130'
#define BB_ST_ALL2                 '\132'
#define BB_ST_BETABRITE            '\136'
#define BB_ST_4120C                '\141'
#define BB_ST_4160C                '\142'
#define BB_ST_4200C                '\143'
#define BB_ST_4240C                '\144'
#define BB_ST_215R                 '\145'
#define BB_ST_215C                 '\146'
#define BB_ST_4120R                '\147'
#define BB_ST_4160R                '\150'
#define BB_ST_4200R                '\151'
#define BB_ST_4240R                '\152'
#define BB_ST_300S                 '\153'
#define BB_ST_7000S                '\154'
#define BB_ST_9616MS               '\155'
#define BB_ST_12816MS              '\156'
#define BB_ST_16016MS              '\157'
#define BB_ST_19216MS              '\160'
#define BB_ST_PPD                  '\161'
#define BB_ST_DIRECTOR             '\162'
#define BB_ST_1005DC               '\163'
#define BB_ST_4080C                '\164'
#define BB_ST_210C_220C            '\165'
#define BB_ST_ALPHAECLIPSE3500     '\166'
#define BB_ST_ALPHAECLIPSETT       '\167'
#define BB_ST_ALPHAPREMIERE9000    '\170'
#define BB_ST_TEMPPROBE            '\171'
#define BB_ST_ALLAZ                '\172'

// Command codes

#define BB_CC_WTEXT    'A'
#define BB_CC_RTEXT    'B'
#define BB_CC_WSPFUNC  'E'
#define BB_CC_RSPFUNC  'F'
#define BB_CC_WSTRING  'G'
#define BB_CC_RSTRING  'H'
#define BB_CC_WSDOTS   'I'
#define BB_CC_RSDOTS   'J'
#define BB_CC_WRGBDOTS 'K'
#define BB_CC_RRGBDOTS 'L'
#define BB_CC_WLDOTS   'M'
#define BB_CC_RLDOTS   'N'
#define BB_CC_WBULL    'O'
#define BB_CC_SETTO    'T'

// Display Positions

#define BB_DP_MIDLINE '\040'
#define BB_DP_TOPLINE '\042'
#define BB_DP_BOTLINE '\046'
#define BB_DP_FILL    '\060'
#define BB_DP_LEFT    '\061'
#define BB_DP_RIGHT   '\062'

// Display Modes

#define BB_DM_ROTATE     'a'
#define BB_DM_HOLD       'b'
#define BB_DM_FLASH      'c'
#define BB_DM_ROLLUP     'e'
#define BB_DM_ROLLDOWN   'f'
#define BB_DM_ROLLLEFT   'g'
#define BB_DM_ROLLRIGHT  'h'
#define BB_DM_WIPEUP     'i'
#define BB_DM_WIPEDOWN   'j'
#define BB_DM_WIPELEFT   'k'
#define BB_DM_WIPERIGHT  'l'
#define BB_DM_SCROLL     'm'
#define BB_DM_SPECIAL    'n'
#define BB_DM_AUTOMODE   'o'
#define BB_DM_ROLLIN     'p'
#define BB_DM_ROLLOUT    'q'
#define BB_DM_WIPEIN     'r'
#define BB_DM_WIPEOUT    's'
#define BB_DM_COMPROTATE 't'
#define BB_DM_EXPLODE    'u'
#define BB_DM_CLOCK      'v'

// Special Display Modes
#define BB_SDM_TWINKLE           '0'
#define BB_SDM_SPARKLE           '1'
#define BB_SDM_SNOW              '2'
#define BB_SDM_INTERLOCK         '3'
#define BB_SDM_SWITCH            '4'
#define BB_SDM_SLIDE             '5'
#define BB_SDM_SPRAY             '6'
#define BB_SDM_STARBURST         '7'
#define BB_SDM_WELCOME           '8'
#define BB_SDM_SLOTS             '9'
#define BB_SDM_NEWSFLASH         'A'
#define BB_SDM_TRUMPET           'B'
#define BB_SDM_CYCLECOLORS       'C'
#define BB_SDM_THANKYOU          'S'
#define BB_SDM_NOSMOKING         'U'
#define BB_SDM_DONTDRINKANDDRIVE 'V'
#define BB_SDM_FISHIMAL          'W'
#define BB_SDM_FIREWORKS         'X'
#define BB_SDM_TURBALLOON        'Y'
#define BB_SDM_BOMB              'Z'

// Text file or string formatting characters/commands

#define BB_FC_DOUBLEHIGH      '\005'
#define BB_FC_TRUEDESCENDERS  '\006'
#define BB_FC_CHARFLASH       '\007'
#define BB_FC_EXTENDEDCHARSET '\010'
#define BB_FC_NOHOLDSPEED     '\011'
#define BB_FC_CALLDATE        '\013'
#define BB_FC_NEWPAGE         '\014'
#define BB_FC_NEWLINE         '\015'
#define BB_FC_SPEEDCONTROL    '\017'
#define BB_FC_CALLSTRING      '\020'
#define BB_FC_DISABLEWIDECHAR '\021'
#define BB_FC_ENABLEWIDECHAR  '\022'
#define BB_FC_CALLTIME        '\023'
#define BB_FC_CALLSDOTS       '\024'
#define BB_FC_SPEED1          '\025'
#define BB_FC_SPEED2          '\026'
#define BB_FC_SPEED3          '\027'
#define BB_FC_SPEED4          '\030'
#define BB_FC_SPEED5          '\031'
#define BB_FC_SELECTCHARSET   '\032'
#define BB_FC_SELECTCHARCOLOR '\034'
#define BB_FC_SELECTCHARATTR  '\035'
#define BB_FC_SELECTCHARSPACE '\036'
#define BB_FC_CALLPICTURE     '\037'

// Character Sets

#define BB_CS_5HIGH        '1'
#define BB_CS_5STROKE      '2'
#define BB_CS_7HIGH        '3'
#define BB_CS_7STROKE      '4'
#define BB_CS_7HIGHFANCY   '5'
#define BB_CS_10HIGH       '6'
#define BB_CS_7SHADOW      '7'
#define BB_CS_FHIGHFANCY   '8'
#define BB_CS_FHIGH        '9'
#define BB_CS_7SHADOWFANCY ':'
#define BB_CS_5WIDE        ';'
#define BB_CS_7WIDE        '<'
#define BB_CS_7WIDEFANCY   '='
#define BB_CS_5WIDESTROKE  '>'
#define BB_CS_5HIGHCUSTOM  'W'
#define BB_CS_7HIGHCUSTOM  'X'
#define BB_CS_10HIGHCUSTOM 'Y'
#define BB_CS_15HIGHCUSTOM 'Z'

// Character Colors

#define BB_COL_RED       '1'
#define BB_COL_GREEN     '2'
#define BB_COL_AMBER     '3'
#define BB_COL_DIMRED    '4'
#define BB_COL_DIMGREEN  '5'
#define BB_COL_BROWN     '6'
#define BB_COL_ORANGE    '7'
#define BB_COL_YELLOW    '8'
#define BB_COL_RAINBOW1  '9'
#define BB_COL_RAINBOW2  'A'
#define BB_COL_COLORMIX  'B'
#define BB_COL_AUTOCOLOR 'C'

// Character Attributes

#define BB_CA_WIDE           '0'
#define BB_CA_DOUBLEWIDE     '1'
#define BB_CA_DOUBLEHIGH     '2'
#define BB_CA_TRUEDESCENDERS '3'
#define BB_CA_FIXEDWIDTH     '4'
#define BB_CA_FANCY          '5'
#define BB_CA_AUXPORT        '6'
#define BB_CA_SHADOW         '7'

// Attribute (and other) Switch

#define BB_OFF '0'
#define BB_ON  '1'

// Picture Types

#define BB_PT_QUICKFLICK   'C'
#define BB_PT_FASTERFLICKS 'G'
#define BB_PT_DOTSPICTURE  'L'

// Date Formats

#define BB_DF_MMDDYYSLASH  '0'
#define BB_DF_DDMMYYSLASH  '1'
#define BB_DF_MMDDYYHYPHEN '2'
#define BB_DF_DDMMYYHYPHEN '3'
#define BB_DF_MMDDYYPERIOD '4'
#define BB_DF_DDMMYYPERIOD '5'
#define BB_DF_MMDDYYSPACE  '6'
#define BB_DF_DDMMYYSPACE  '7'
#define BB_DF_MMMDDYYYY    '8'
#define BB_DF_DAYOFWEEK    '9'

// Temp Format

#define BB_TF_CELSIUS    '\034'
#define BB_TF_FAHRENHEIT '\035'

// Character Spacing

#define BB_SP_PROPORTIONAL '0'
#define BB_SP_FIXEDWIDTH   '1'

// Miscellaneous

#define BB_PRIORITY_FILE_LABEL '0'

#endif