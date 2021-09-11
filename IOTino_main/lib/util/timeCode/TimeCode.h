/*
  TimeCode.h - TimeCode-Library for Airduino.
*/

#ifndef TimeCode_h
#define TimeCode_h

#include <Arduino.h>

#define TIMECODE_FORMAT_NONE 0
#define TIMECODE_FORMAT_MILLIS 1
#define TIMECODE_FORMAT_DIGITAL 2
#define TIMECODE_FORMAT_SEPERATE 3

/**************************************************************************/
/*!
    @brief          fills an integer with zeros to a minimum amount of digits
    @param  value   the integer value which will be converted to a string an asserted with zeros to match a minimum amount of digits
    @param  digits  the minimum amount of digits to which the string value will be extended
    @returns        the string value of the integer, filled to the minimum amount of digits
*/
/**************************************************************************/
String fillDigits(unsigned long value, byte digits);

/**************************************************************************/
/*!
    @brief          prints the system runtime to the Serial Monitor
    @param  value   the integer value which will be converted to a string an asserted with zeros to match a minimum amount of digits
    @param  digits  the minimum amount of digits to which the string value will be extended
    @returns        the string value of the integer, filled to the minimum amount of digits
*/
/**************************************************************************/
void printTimeCode(byte format = 3, byte fillDigits = 7, bool printText = true, bool printCycleDuration = true, byte fillCycleDigits = 0, bool cycleSeconds = true);

#endif