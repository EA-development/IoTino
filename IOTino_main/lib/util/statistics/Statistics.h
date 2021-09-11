/*
  Statistics.h - Statistics-Library for Airduino.
*/

#ifndef Statistics_h
#define Statistics_h

#include <Arduino.h>
#include <functional>
#include <array>
#include <map>
#include <vector>

#define DEFAULT_TRIES 10

/**************************************************************************/
/*!
    @brief              alias of mean() --
                        averages the returned values of a function
    @param  function    the function which is executed to get the values from which the average will be calculated.
                        For example a function which returns the value of a sensor.
                        Note: the function can return any integer type.
    @param  samples     the amount how often the function will be executed to collect the data for the average.
    @param  invalid     Optional - the function return value, which markes the function return value as invalid.
                        If this value is still returned after several retries, the current sample won't be considered for the average.
    @param  tries       Optional - the number of tries that will be done before an sample is excluded from the average because of invalid function values. Default: 10
    @param  failMessage Optional - this Message will be outputed to the Serial Monitor when after the retries a sample is still invalid.
    @returns            the average of the collected samples of the function's return values.
*/
/**************************************************************************/
double average(std::function<double()> function, int samples, double invalid = -__DBL_MAX__+1, int tries = DEFAULT_TRIES, String failMessage = "");

/**************************************************************************/
/*!
    @brief              calculates the mean of the returned values of a function
    @param  function    the function which is executed to get the values from which the mean will be calculated.
                        For example a function which returns the value of a sensor.
                        Note: the function can return any integer type.
    @param  samples     the amount how often the function will be executed to collect the data for the mean calculation.
    @param  invalid     Optional - the function return value, which markes the function return value as invalid.
                        If this value is still returned after several retries, the current sample won't be considered for the mean calculation.
    @param  tries       Optional - the number of tries that will be done before an sample is excluded from the mean calculation because of invalid function values. Default: 10
    @param  failMessage Optional - this Message will be outputed to the Serial Monitor when after the retries a sample is still invalid.
    @returns            the mean of the collected samples of the function's return values.
*/
/**************************************************************************/
double mean(std::function<double()> function, int samples, double invalid = -__DBL_MAX__+1, int tries = DEFAULT_TRIES, String failMessage = "");

/**************************************************************************/
/*!
    @brief              alias of mean() --
                        calculates the average of the center of the distribution of the returned values of a function
    @param  function    the function which is executed to get the values from which the average will be calculated.
                        For example a function which returns the value of a sensor.
                        Note: the function can return any integer type.
    @param  samples     the amount how often the function will be executed to collect the data for the average.
    @param  percent     the percent of the values which will be averaged. they are taken from the center of the distribution.
    @param  invalid     Optional - the function return value, which markes the function return value as invalid.
                        If this value is still returned after several retries, the current sample won't be considered for the average.
    @param  tries       Optional - the number of tries that will be done before an sample is excluded from the average because of invalid function values. Default: 10
    @param  failMessage Optional - this Message will be outputed to the Serial Monitor when after the retries a sample is still invalid.
    @returns            the average of the collected samples of the function's return values.
*/
/**************************************************************************/
double centralAverage(std::function<double()> function, int samples, byte percent,  double invalid = -__DBL_MAX__+1, int tries = DEFAULT_TRIES, String failMessage = "");


/**************************************************************************/
/*!
    @brief              calculates the mean of the center of the distribution of the returned values of a function
    @param  function    the function which is executed to get the values from which the mean will be calculated.
                        For example a function which returns the value of a sensor.
                        Note: the function can return any integer type.
    @param  samples     the amount how often the function will be executed to collect the data for the mean calculation.
                        This value should be greater so that the central mean has an effect.
    @param  percent     the percent of the values which will be averaged. they are taken from the center of the distribution.
    @param  invalid     Optional - the function return value, which markes the function return value as invalid.
                        If this value is still returned after several retries, the current sample won't be considered for the mean calculation.
    @param  tries       Optional - the number of tries that will be done before an sample is excluded from the mean calculation because of invalid function values. Default: 10
    @param  failMessage Optional - this Message will be outputed to the Serial Monitor when after the retries a sample is still invalid.
    @returns            the mean of the collected samples of the function's return values.
*/
/**************************************************************************/
double centralMean(std::function<double()> function, int samples, byte percent, double invalid = -__DBL_MAX__+1, int tries = DEFAULT_TRIES, String failMessage = "");

/**************************************************************************/
/*!
    @brief              calculates the median of the returned values from a function
    @param  function    the function which is executed to get the values from which the median will be calculated.
                        For example a function which returns the value of a sensor.
                        Note: the function can return any integer type.
    @param  samples     the amount how often the function will be executed to collect the data for the median calculation.
    @param  invalid     Optional - the function return value, which markes the function return value as invalid.
                        If this value is still returned after several retries, the current sample won't be considered for the median calculation.
    @param  tries       Optional - the number of tries that will be done before an sample is excluded from the median calculation because of invalid function values. Default: 10
    @param  failMessage Optional - this Message will be outputed to the Serial Monitor when after the retries a sample is still invalid.
    @returns            the median of the collected samples of the function's return values.
*/
/**************************************************************************/
double median(std::function<double()> function, int samples, double invalid = -__DBL_MAX__+1, int tries = DEFAULT_TRIES, String failMessage = "");

/**************************************************************************/
/*!
    @brief              calculates the modal value the returned values from a function
    @param  function    the function which is executed to get the values from which the modal value will be calculated.
                        For example a function which returns the value of a sensor.
                        Note: the function can return any integer type.
    @param  samples     the amount how often the function will be executed to collect the data for the modal value calculation.
    @param  invalid     Optional - the function return value, which markes the function return value as invalid.
                        If this value is still returned after several retries, the current sample won't be considered for the modal value calculation.
    @param  tries       Optional - the number of tries that will be done before an sample is excluded from the modal value calculation because of invalid function values. Default: 10
    @param  failMessage Optional - this Message will be outputed to the Serial Monitor when after the retries a sample is still invalid.
    @returns            the modal value of the collected samples of the function's return values.
*/
/**************************************************************************/
double modal(std::function<double()> function, int samples, double invalid = -__DBL_MAX__+1, int tries = DEFAULT_TRIES, String failMessage = "");

#endif