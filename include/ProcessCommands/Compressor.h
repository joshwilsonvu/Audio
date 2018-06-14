//
// Created by Josh Wilson on 4/27/17.
//

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "ProcessCommand.h"

class Factory;

class Compressor : public ProcessCommand {
public:
    /*
     * Compresses the range beginning at first and up to last, and stores the compressed
     * signal in a range beginning at result. Uses envelope value set in last call,
     * and is thus appropriate for running over successive intervals of a longer signal.
     */
    virtual void execute (sample_t* first, sample_t* last, sample_t* result);
    using ProcessCommand::execute;

    /*
     * Clears the internal envelope value, so that it may be run over a new signal.
     */
    virtual void reset();

    virtual std::string toString() const {
        return "Compressor";
    }

    virtual ~Compressor() {}

private:
    friend class Factory; // only Factory can create Compressor instances
    Compressor(); // default constructor, not a valid state
    Compressor(double threshold, double slope, double attack,
               double release, double postgain);// alt constructor

    double envelope;
    // the averaged level of the signal, affected by attack and release

    double threshold, slope, attack, release, postgain;
    // the values that define the compressor
};

#endif //COMPRESSOR_H
