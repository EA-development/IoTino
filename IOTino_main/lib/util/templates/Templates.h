/*
  Template.h - Template-Library for Airduino.
*/

#ifndef Template_h
#define Template_h

#include <Arduino.h>

/**************************************************************************/
/*!
    @brief        get the size of an array
    @param array  the array whose size will be returned
    @returns      the number of elements stored in the array
*/
/**************************************************************************/
template<class T, size_t size>
int sizeOf(const T (&array)[size]) {
  return (sizeof(array)/sizeof(T));
}

/**************************************************************************/
/*!
    @brief        get the size of an array
    @param array  the array whose size will be returned
    @returns      the number of elements stored in the array
*/
/**************************************************************************/
template<class T>
int sizeOf(T* (&array)) {
  return (sizeof(array)/sizeof(T));
}

#endif