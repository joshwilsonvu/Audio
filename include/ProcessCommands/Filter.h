//
// Created by Josh Wilson on 4/26/17.
//

#ifndef FILTER_H
#define FILTER_H

#include "Sample_t.h"
#include "ProcessCommand.h"

class Factory;

class Filter : public ProcessCommand {
public:
    /*
     * Filters the range beginning at first and up to last, and stores the filtered
     * signal in a range beginning at result. Uses feedback values set in last call,
     * and is thus appropriate for running over successive intervals of a longer signal.
     */
    virtual void execute (sample_t* first, sample_t* last, sample_t* result);
    using ProcessCommand::execute;

    /*
     * Clears the internal feedback values, so that it may be run over a new signal.
     */
    virtual void reset();

    virtual std::string toString() const {
        return "Filter";
    }

    virtual ~Filter() {}

    // Frequency response (from sox.exe)
    // o=2*pi/Fs
    // H(f)=sqrt((b0*b0+b1*b1+b2*b2+2.*(b0*b1+b1*b2)*cos(f*o)+2.*(b0*b2)*cos(2.*f*o))
    // /(1.+a1*a1+a2*a2+2.*(a1+a1*a2)*cos(f*o)+2.*a2*cos(2.*f*o)))

private:
    friend class Factory; // only Factory can create Filter instances
    Filter(); // default constructor, not a valid state
    Filter(double b0, double b1, double b2, double a1, double a2); // alt constructor

    sample_t feedback1, feedback2;
    // the memory retained from previous filtering

    double b0, b1, b2, a1, a2;
    // the coefficients of the difference equation that defines the filter
};

#endif //FILTER_H
