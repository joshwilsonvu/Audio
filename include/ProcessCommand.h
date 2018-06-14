//
// Created by Josh Wilson on 4/27/17.
//

#ifndef PROCESSCOMMAND_H
#define PROCESSCOMMAND_H

#include <string>
#include "Sample_t.h"

// An abstract base class that enforces the interface
// for any command to be run on a range of samples.
class ProcessCommand {
public:
    /*
     * The function that executes the command parameterized by begin and end pointers
     * and a result pointer of type sample_t. Subclasses ~must~ define execute with these
     * parameters.
     * Two parameter version alters data in-place.
     */
    virtual void execute (sample_t* first, sample_t* last, sample_t* result) = 0;
    virtual void execute (sample_t* first, sample_t* last) {
        execute(first, last, first);
    }

    virtual void reset()
    {
        // no-op if not overridden
    }

    virtual std::string toString() const
    {
        // return empty string if not overridden
        return "";
    }

    virtual ~ProcessCommand()
    {
        // virtual destructor for abstract base class
    }
};

#endif //PROCESSCOMMAND_H
