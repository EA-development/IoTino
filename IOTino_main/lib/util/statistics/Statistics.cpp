/*
 Statistics.cpp - Statistics-Library for Librarys.
*/

#include "Statistics.h"

//private
#define INVALID (invalid != -__DBL_MAX__+1 && v == invalid)
double v;
std::vector<double> values;
std::map<double, int> amounts;

//method for getting valid sample data
double getValue(std::function<double()> function, double invalid, int tries, String failMessage) {
    //temp variables;
    v = 0;
    int count = tries;
    do {
        v = function();
        count--;
    } while(INVALID && count >= 0);     //repeat if invalid until no try left
    //prints fail message if still invalid and fail message set
    if(INVALID && failMessage != "") {
        Serial.print("WARNING: ");
        Serial.print(failMessage);
        Serial.print(" | ");
        Serial.print(tries);
        Serial.print(" tries");
        Serial.println();
    }
    //returns value
    return v;
}

std::vector<double> getDistribution(std::function<double()> function, int samples, double invalid, int tries, String failMessage) {
    values.clear();
    //returns if invalid sample amount
    if(samples == 0)
        return values;
    //temp variables
    v = 0;
    //get data
    for(int i = 0; i < samples; i++) {
        //gets next value
        v = getValue(function, invalid, tries, failMessage);
        //if valid stores it, if not decreases samples and tries again
        if(!INVALID)
            values.push_back(v);
    }
    //sorts values
    std::sort(values.begin(), values.end());
    return values;
}

//public
double average(std::function<double()> function, int samples, double invalid, int tries, String failMessage) {
    return mean(function, samples, invalid, tries, failMessage);
}

double mean(std::function<double()> function, int samples, double invalid, int tries, String failMessage) {
    //returns if invalid sample amount
    if(samples == 0)
        return 0;
    //temp variables
    double value = 0;
    v = 0;
    //get data
    for(int i = 0; i < samples; i++) {
        //gets next value
        v = getValue(function, invalid, tries, failMessage);
        //if valid adds it to the sum, if not decreases samples and tries again
        if(INVALID) {
            i--;
            samples--;
        } else
            value+=v;
    }
    //divides the values through the amount of valid samples
    value/=samples;
    //returns the mean
    return value;
}

double centralAverage(std::function<double()> function, int samples, byte percent, double invalid, int tries, String failMessage) {
    return centralMean(function, samples, percent, invalid, tries, failMessage);
}

double centralMean(std::function<double()> function, int samples, byte percent, double invalid, int tries, String failMessage) {
    //returns if invalid sample amount
    if(samples == 0)
        return 0;
    getDistribution(function, samples, invalid, tries, failMessage);
    //calculates value span for average
    int range = round(values.size()*min(percent/100., (double)1)/2);
    int center = floor(values.size()/2);
    double value = 0;
    //averages central percent of values
    for(int i = center-range; i < center+range+(values.size()%2!=0?1:0); i++)
        value+=values.at(i);
    value/=range*2+(values.size()%2!=0?1:0);
    //returns mean
    return value;
}

double median(std::function<double()> function, int samples, double invalid, int tries, String failMessage) {
    //returns if invalid sample amount
    if(samples == 0)
        return 0;
    getDistribution(function, samples, invalid, tries, failMessage);
    //returns...
    if(values.size()%2 == 0)
        //if even the average of the central two
        return (values[values.size()/2-1]+values[values.size()/2])/2.;
    else
        //if odd the central value
        return values[(int)floor(values.size()/2.)];
}

double modal(std::function<double()> function, int samples, double invalid, int tries, String failMessage) {
    //returns if sample amount is invalid
    if(samples == 0)
        return 0;
    //temp variables
    amounts.clear();
    v = 0;
    //get data
    for(int i = 0; i < samples; i++) {
        //gets next value
        v = getValue(function, invalid, tries, failMessage);
        //if valid, increases the amount of the value in the map by one.
        if(!INVALID)
            amounts[v]++;
    }
    short value = 0;
    //searches for the value with the highest amount
    //TODO: handle if multiple highest amounts
    for(auto const& x : amounts)
        if(x.second > amounts[value])
        value = x.first;
    //returns modal value
    return value;
}