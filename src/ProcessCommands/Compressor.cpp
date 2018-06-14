//
// Created by Josh Wilson on 4/27/17.
//

#include "ProcessCommands/Compressor.h"
#include "Helpers.h"
#include <cmath>

void Compressor::execute(sample_t* first, sample_t* last, sample_t* result)
{
    // Implements a hard-knee compression algorithm
    std::transform(first, last, result, [this] (sample_t x) mutable {
        double magnitude = std::fabs(x); // calculate input magnitude

        envelope = magnitude + (envelope < magnitude ? attack : release) * (envelope - magnitude);
        // adjusts envelope based on magnitude, attack, and release
        
        double gain = Helpers::dbToAmp(slope * fmin(0.0, threshold - Helpers::ampToDb(envelope)))
                      * postgain;
        // use envelope, threshold, ratio, and postgain to hard-knee compress appropiately

        return x * gain; // applies the gain to the input sample and returns
    });
}

void Compressor::reset()
{
    envelope = 0.0; // clears envelope
}

Compressor::Compressor(): envelope(0.0) // invalid state
{}

Compressor::Compressor(double threshold, double slope, double attack, double release,
                       double postgain)
: envelope(0.0), threshold(threshold), slope(slope), attack(attack),
  release(release), postgain(postgain)
{}