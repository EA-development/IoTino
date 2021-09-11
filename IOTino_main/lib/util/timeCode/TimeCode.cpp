/*
 TimeCode.cpp - TimeCode-Library for Librarys.
*/

#include "TimeCode.h"

//private
#define VALID_FORMAT (format > 0 && format < 4)
#define MILLIS (format == 1)
#define DIGITAL (format == 2)
#define SEPERATE (format == 3)

unsigned long lastCycle = 0;

String fillDigits(unsigned long value, byte digits) {
    //returns if value has already enough digits
    if(value >= pow(10, digits-1))
        return String(value);
    //temp variables
    String out;
    byte v;
    //asserts digits of value to out string, at the end it asserts 0s
    for(int i = 0; i < digits; i++) {
        v=value%10;
        out=String(v)+out;
        value-=v;
        value/=10;
    }
    //returns out string 
    return out;
}

void printTimeCode(byte format, byte fillDigit, bool printText, bool printCycleDuration, byte fillCycleDigits, bool cycleSeconds) {
    //temp variables
    unsigned long time = millis();
    unsigned short cycleDuration = time-lastCycle;
    lastCycle = time;
    //print runtime if valid format
    if(VALID_FORMAT) {
        if(printText)
            Serial.print(F("runtime: "));
        //milliseconds only format
        if(MILLIS)
            Serial.print(fillDigits(time, fillDigit));
        else {
            //TODO: less code, more compact
            //get h, m, s and ms for the other formats
            unsigned short ms=time%1000;
            time-=ms;
            time/=1000;
            byte s=time%60;
            time-=s;
            time/=60;
            byte m=time%60;
            time-=m;
            time/=60;
            unsigned short h=time;
            //prints h, m, s and ms and filles them with 0s to the minimum digit count
            //hours
            Serial.print(fillDigits(h, max(fillDigit-7, 0)));
            if(fillDigit > 7 || h > 0) {
                if(DIGITAL)
                    //if digital format prints ":"
                    Serial.print(F(":"));
                else
                    //if seperate format prints units
                    Serial.print(F("h "));
            }
            //minutes
            Serial.print(fillDigits(m, max(min((int)fillDigit-5, DIGITAL?2:1), 0)));
            if(fillDigit > 5 || m > 0) {
                if(DIGITAL)
                    Serial.print(F(":"));
                else
                    Serial.print(F("m "));
            }
            //seconds
            Serial.print(fillDigits(s, max(min((int)fillDigit-3, DIGITAL?2:1), 0)));
            if(fillDigit > 3 || s > 0) {
                if(DIGITAL)
                    Serial.print(F(":"));
                else
                    Serial.print(F("s "));
            }
            //milliseconds
            Serial.print(fillDigits(ms, max(min((int)fillDigit, DIGITAL?2:1), 0)));
            if(SEPERATE)
                Serial.print(F("ms "));
        }
    }
    //print cycle duration
    if(printCycleDuration) {
        //prints tap to keep distance from runtime printout
        if(VALID_FORMAT)
            Serial.print(F("\t"));
        if(printText)
            Serial.print(F("cycle duration: "));
        //format with seconds seperate
        if(cycleSeconds && (cycleDuration>=1000 || fillCycleDigits > 3)) {
            //print seconds
            Serial.print(fillDigits(floor(cycleDuration/1000), max(fillCycleDigits-3, 0)));
            Serial.print(F("s "));
            //print milliseconds
            Serial.print(fillDigits(cycleDuration%1000, min((int)fillCycleDigits, 3)));
        //format with only milliseconds
        } else
            Serial.print(fillDigits(cycleDuration, fillCycleDigits));
        Serial.print(F("ms"));
    }
    //prints boarder to further prints
    Serial.print(F(" | \t"));
}
