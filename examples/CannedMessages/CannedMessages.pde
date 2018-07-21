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

// Display some canned messages and optionally the current time
// on a BetaBrite.
#include <Wire.h>
#include "RTClib.h"
#include "NewSoftSerial.h"

RTC_DS1307 RTC;
NewSoftSerial  bbPort(7, 8);

const int dtime = 15000;

const char *DisplayStrings[]={
 "This is a message",
 "... it came from an Arduino",
 "Beer is good"
};

void setup ()
{
 randomSeed(analogRead(1));
 Wire.begin();
 bbPort.begin (9600);
 bbWelcome();
}

void loop ()
{
//  displayTime();
  displayCount();
  displayRandomMessage();
}

unsigned int count = 1;
void displayCount()
{
  bbPreamble();
  bbPort.print(count++);
  bbFinish();
  delay(dtime);
  bbCancelPriorityMessage();
}

void displayTime()
{
  DateTime now = RTC.now ();

  bbPreamble();
  bbPort.print(now.year(), DEC );
  bbPort.print("/");
  bbPort.print(now.month(), DEC );
  bbPort.print("/");
  bbPort.print(now.day(), DEC );
  bbPort.print(" ");
  bbPort.print(now.hour(), DEC );
  bbPort.print(":");
  if (now.minute() < 10) bbPort.print('0');
  bbPort.print(now.minute(), DEC);
  bbFinish();
  delay(dtime);
  bbCancelPriorityMessage();
}

void displayRandomMessage()
{
  char *pStr = (char *)DisplayStrings[random(sizeof(DisplayStrings)/sizeof(char *))];

  bbPreamble();
  bbPort.print (pStr);
  bbFinish();
  delay(dtime);
  bbCancelPriorityMessage();
}

void bbPreamble()
{
  for (unsigned char i=0; i<20; i++) bbPort.print ('\000');
  bbPort.print("\001Z00\002A0\033 o");
}

void bbFinish()
{
  bbPort.print ("\004");
}

void bbCancelPriorityMessage()
{
  for (unsigned char i=0; i<20; i++) bbPort.print('\000');
  bbPort.print ("\001Z00\002A0\004");
}

void bbWelcome()
{
  bbPreamble();
  bbPort.print ("Go!");
  bbFinish();
  delay(4000);
  bbCancelPriorityMessage ();
}
