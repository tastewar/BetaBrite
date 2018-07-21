// The MIT License (MIT)

// Copyright (c) 2011 Michael Keirnan

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Example of displaying messages from a web site on a sign. The main loop
// requests a web page displays each line of the response as a BB message.
// A limited form of BetaBrite-markup is supported. For example the following
// should show as you'd expect on the sign:
//
// [red]as of 10/28/12 07:38pm
// [snow,green]Some Text here, it is green with snow effect
// [amber,rotate]I'm amber and I rotate
// [green, rotate]green
// [red,interlock]Monday
// [green,rotate]
//
// Requirements:
//  - RS232 serial driver circuit
//  - Ethernet shield or equivalent (project originally on Arduino Uno + Ethershield)
//  - network libs that work with above shield (ones below are circa Uno days
//     but reliable)
//  - you could also get your input from something other than a web server
//
// Been running this for a couple of years to display web site status at my company
// at it has had no problems.
//
// Improvements I'll Probably Never Make Time To Do
//
// - reacquire DHCP address and resolve server on connect error. Currently
//   whenenver our network blips (once a quarter-ish) I power cycle the duino. No
//   biggee, but rather inelegant. Current Ethernet libs may already do the right
//   thing here if you use a modern board.
// - the sign will often pause and restart a message while updating. Judicious use of
//   BetaBrite text files would probably solve this.
//
#include <Metro.h>

#include "NewSoftSerial.h"
#include <BETABRITE.h>

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetDHCP.h>
#include <EthernetDNS.h>

// put MAC address of your Enet device here
static byte mac[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static byte dnsServer[] = { 8, 8, 8, 8 };

// Put your web server here, also see URL used
// in queryServer(). It should be a constant too.
const char *webServerName = "www.example.com";

static byte webServerIp[4];
static unsigned int webServerPort = 4444;

const byte MAX_MSG_SIZE = 200;

const byte NET_ACTIVITY_LED = 10;

/* BetaBrite sign */
BETABRITE bb(7,8);
char bbTextFileName = 'A';
int numFiles = 10;
boolean inHeaders = true;

Client client(webServerIp, webServerPort);

// Metro is a nice simple library to check multiple
// timers during the main loop.
Metro queryTimer = Metro(180000);
Metro dhcpTimer = Metro(600000);

void setup()
{
  Serial.begin(9600);
  Serial.println("setup()");
  bb.WritePriorityTextFile("Initializing", BB_COL_AMBER, BB_DP_TOPLINE, BB_DM_SPECIAL, BB_SDM_INTERLOCK);
  delay(2500);
  bb.CancelPriorityTextFile();

  EthernetDHCP.begin(mac);
  const byte* clientIp = EthernetDHCP.ipAddress();
  Serial.print("DHCP IP: ");
  Serial.println(ip_to_str(clientIp));
  bb.WritePriorityTextFile(ip_to_str(clientIp), BB_COL_AMBER, BB_DP_FILL, BB_DM_SPECIAL, BB_SDM_SNOW);
  delay(3000);
  resolveServer();

  bb.SetMemoryConfiguration(bbTextFileName, numFiles);

  delay(1000);
  updateStats();
}

void loop ()
{
  if (queryTimer.check()) {
    updateStats();
  }
}

void updateStats()
{
  if (!client.connected()) {
    client.stop();
  }

  bbTextFileName = 'A';  
  inHeaders = true;

  queryServer();
  printResponse();
  EthernetDHCP.maintain();
}

void queryServer()
{
  Serial.println("connecting...");
  if (client.connect())
  {
    Serial.println("connected");

    // change this to URL of web service
    client.println("GET /messages.bb HTTP/1.0");
    client.println();
    queryTimer.interval(120000);
  }
  else
  {
    Serial.println("connection failed");
    queryTimer.interval(15000);
  }
}

char readChar()
{
  int timeout = 30000;
  unsigned long start = millis();
  unsigned long current = start;
  while (!client.available() && current < start + timeout)
  {
    current = millis();
  }
  if (client.available())
  {
    return client.read();
  }
  else
  {
    return -1;
  }
}

void printResponse()
{
  if (!client.connected())
  {
    client.stop();
    Serial.println("client disconnected");
    return;
  }

  char buf[MAX_MSG_SIZE];
  char *buf_ptr = buf;
  
  if (!client.available()) {
    delay(2000);
  }

  boolean failed = true;
  char rc[4] = "doh";
  while (client.available() && inHeaders) {
    char *header = readLine();
    Serial.print("head:"); Serial.println(header);

    // blank line ends headers
    boolean blank = true;
    for (int i = 0; i < strlen(header); i++) {
      if (!isspace(header[i])) {
        blank = false;
        break;
      }
    }
    if (blank) {
      inHeaders = false;
    }
    
    // Look for HTTP response code
    if (strstr(header, "HTTP") == header) {
      char *space = strchr(header, ' ');
      if (space != NULL) {
        char *start = strpbrk(space, "0123456789");
        if (start != NULL) {
          strncpy(rc, start, 3);
          if (strstr(rc, "200") == rc) {
            failed = false;
          }
        }
      }
    }
    
    if (failed) {
      Serial.println("fail!");
      bb.WritePriorityTextFile(rc, BB_COL_RAINBOW2, BB_DP_FILL, BB_DM_SPECIAL, BB_SDM_BOMB);
      client.stop();
      return;
    }
  }

  bb.CancelPriorityTextFile();
  while (client.available()) {
    processLine();
  }
  message(" ");
  message(" ");
}

void processLine()
{
  char *line = readLine();
  message(line);
}

char* readLine()
{
  static char buf[MAX_MSG_SIZE] = "";
  for (int i=0; i<=MAX_MSG_SIZE; i++)
  {
    char c = readChar();
    if (c == -1 || c == '\n')
    {
      buf[i] = 0;
      break;
    }
    else
    {
      buf[i] = c;
    }
  }
  return buf;
}

void message(const char *msg)
{
  Serial.print(bbTextFileName);
  Serial.println(msg);

  char color = BB_COL_AUTOCOLOR;
  char position = BB_DP_TOPLINE;
  char mode = BB_DM_COMPROTATE;
  char special = BB_SDM_TWINKLE;

  // handle options
  char *open_delim = strchr(msg, '[');
  char *close_delim = strchr(msg, ']');
  if (open_delim == msg && close_delim != NULL) {
    int options_length = close_delim - open_delim; 
    char options[options_length];
    strncpy(options, msg+1, options_length - 1);
    options[options_length - 1] = '\0';
    Serial.print("options: "); Serial.println(options);
 
     char *option = strtok(options, ",");
     while (option != NULL) {
       if (strstr(option, "trumpet") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_TRUMPET;
       }
       else if (strstr(option, "red") == option) {
         color = BB_COL_RED;
       }
       else if (strstr(option, "amber") == option) {
         color = BB_COL_AMBER;
       }
       else if (strstr(option, "green") == option) {
         color = BB_COL_GREEN;
       }
       else if (strstr(option, "rotate") == option) {
         mode = BB_DM_ROTATE;
       }
       else if (strstr(option, "hold") == option) {
         mode = BB_DM_HOLD;
       }
       else if (strstr(option, "flash") == option) {
         mode = BB_DM_FLASH;
       }
       else if (strstr(option, "rollup") == option) {
         mode = BB_DM_ROLLUP;
       }
       else if (strstr(option, "rolldown") == option) {
         mode = BB_DM_ROLLDOWN;
       }
       else if (strstr(option, "rollleft") == option) {
         mode = BB_DM_ROLLLEFT;
       }
       else if (strstr(option, "rollright") == option) {
         mode = BB_DM_ROLLRIGHT;
       }
       else if (strstr(option, "wipeup") == option) {
         mode = BB_DM_WIPEUP;
       }
       else if (strstr(option, "wipedown") == option) {
         mode = BB_DM_WIPEDOWN;
       }
       else if (strstr(option, "wipeleft") == option) {
         mode = BB_DM_WIPELEFT;
       }
       else if (strstr(option, "wiperight") == option) {
         mode = BB_DM_WIPERIGHT;
       }
       else if (strstr(option, "scroll") == option) {
         mode = BB_DM_SCROLL;
       }
       else if (strstr(option, "automode") == option) {
         mode = BB_DM_AUTOMODE;
       }
       else if (strstr(option, "rollin") == option) {
         mode = BB_DM_ROLLIN;
       }
       else if (strstr(option, "rollout") == option) {
         mode = BB_DM_ROLLOUT;
       }
       else if (strstr(option, "wipein") == option) {
         mode = BB_DM_WIPEIN;
       }
       else if (strstr(option, "wipeout") == option) {
         mode = BB_DM_WIPEOUT;
       }
       else if (strstr(option, "comprotate") == option) {
         mode = BB_DM_COMPROTATE;
       }
       else if (strstr(option, "explode") == option) {
         mode = BB_DM_EXPLODE;
       }
       else if (strstr(option, "clock") == option) {
         mode = BB_DM_CLOCK;
       }
       else if (strstr(option, "twinkle") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_TWINKLE;
       }
       else if (strstr(option, "sparkle") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_SPARKLE;
       }
       else if (strstr(option, "snow") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_SNOW;
       }
       else if (strstr(option, "interlock") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_INTERLOCK;
       }
       else if (strstr(option, "switch") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_SWITCH;
       }
       else if (strstr(option, "slide") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_SLIDE;
       }
       else if (strstr(option, "spray") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_SPRAY;
       }
       else if (strstr(option, "starburst") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_STARBURST;
       }
       else if (strstr(option, "welcome") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_WELCOME;
       }
       else if (strstr(option, "slots") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_SLOTS;
       }
       else if (strstr(option, "newsflash") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_NEWSFLASH;
       }
       else if (strstr(option, "cyclecolors") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_CYCLECOLORS;
       }
       else if (strstr(option, "thankyou") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_THANKYOU;
       }
       else if (strstr(option, "nosmoking") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_NOSMOKING;
       }
       else if (strstr(option, "dontdrinkanddrive") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_DONTDRINKANDDRIVE;
       }
       else if (strstr(option, "fish") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_FISHIMAL;
       }
       else if (strstr(option, "fireworks") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_FIREWORKS;
       }
       else if (strstr(option, "balloon") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_TURBALLOON;
       }
       else if (strstr(option, "bomb") == option) {
         mode = BB_DM_SPECIAL;
         special = BB_SDM_BOMB;
       }

       else {
         Serial.print("Unknown option:"); Serial.println(option);
       }
       option = strtok(NULL, ",");
     }
    msg = close_delim+ 1;
  }
  Serial.print(color);Serial.print(position);Serial.print(mode);Serial.println(special);
  bb.WriteTextFile(bbTextFileName++, msg, color, position, mode, special);  
}

void resolveServer()
{
  EthernetDNS.setDNSServer(dnsServer);
  Serial.print("resolving ");
  Serial.print(webServerName);
  Serial.print(" ...");
  DNSError result = EthernetDNS.resolveHostName(webServerName, webServerIp);
  if (result == DNSSuccess)
  {
    Serial.println(ip_to_str(webServerIp));
  }
  else
  {
    Serial.println("Problem resolving host name");
  }
}

const char* ip_to_str(const byte* ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}
