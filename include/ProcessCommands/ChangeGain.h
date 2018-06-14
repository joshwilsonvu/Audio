//
// Created by Josh Wilson on 5/10/17.
//

#ifndef CHANGEGAIN_H
#define CHANGEGAIN_H

#include "Sample_t.h"
#include "ProcessCommand.h"

class Factory;

class Gain : public ProcessCommand {
public:
    /*
     * Changes the volume of the specified range.
     */
    virtual void execute (sample_t* first, sample_t* last, sample_t* result);
    using ProcessCommand::execute;

    virtual std::string toString() const {
        return "Gain";
    }

    virtual ~Gain() {}

private:
    friend class Factory; // only Factory can create Gain instances
    Gain(double gain_db);

    double gain;
};

#endif //CHANGEGAIN_H