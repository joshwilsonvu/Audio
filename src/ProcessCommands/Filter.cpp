//
// Created by Josh Wilson on 4/26/17.
//

#include "ProcessCommands/Filter.h"
#include <algorithm>
#include <cmath>

void Filter::execute(sample_t* first, sample_t* last, sample_t* result)
{
    // Implements a second-order direct form II infinite impulse response filter
    std::transform(first, last, result, [this] (sample_t x) mutable {
        double y = std::fma(x, b0, feedback1); // = input * b0 + feedback1
        feedback1 = std::fma(x, b1, std::fma(y, a1, feedback2));
        feedback2 = std::fma(x, b2, y * a2);
        return y;
    });
}

void Filter::reset()
{
    feedback1 = feedback2 = 0.0; // clears feedback values
}

Filter::Filter(): feedback1(0.0), feedback2(0.0)
{}

Filter::Filter(double b0, double b1, double b2, double a1, double a2)
: feedback1(0.0), feedback2(0.0), b0(b0), b1(b1), b2(b2), a1(a1), a2(a2)
{}