//
// Created by Josh Wilson on 4/27/17.
//

#ifndef FACTORY_H
#define FACTORY_H

#include "ProcessCommands/ChangeGain.h"
#include "ProcessCommands/Filter.h"
#include "ProcessCommands/Compressor.h"

class Factory {
public:

    /*
     * Every Factory make method returns a pointer to dynamically allocated memory
     * in which the new object has been created. All of these pointers can be safely
     * cast to ProcessCommand*. Memory must be deallocated elsewhere.
     */

    /*
     * Initializes the factory with the sample rate and optionally the number of channels.
     */
    Factory(double sampleRate, unsigned numChannels = 1u);

    double sampleRate() { return _sampleRate; }

    unsigned numChannels() { return _numChannels; }


    Gain* makeChangeGain(double gain);

    /*
     * freq - the fundamental frequency at which the filter is centered
     * Q - the width or resonance of the filter
     * gain - the decibel change at the peak or shelf
     */
    Filter* makeLowpassFilter(double freq, double Q);

    Filter* makeHighpassFilter(double freq, double Q);

    Filter* makeBandpassFilter(double freq, double Q);

    Filter* makeAllpassFilter(double freq, double Q);

    Filter* makePeakFilter(double freq, double Q, double gain);

    Filter* makeLowshelfFilter(double freq, double Q, double gain);

    Filter* makeHighshelfFilter(double freq, double Q, double gain);

    Compressor* makeCompressor(double threshold, double ratio, double attackTime,
                       double releaseTime, double postgain);


private:
    // The number of samples of each channel per second.
    double _sampleRate;

    // The number of channels (1 = mono, 2 = stereo, etc.)
    unsigned _numChannels;

};

#endif //FACTORY_H
