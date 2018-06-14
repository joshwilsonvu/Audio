//
// Created by Josh Wilson on 4/27/17.
//

#ifndef PROCESSCOMMANDMACRO_H
#define PROCESSCOMMANDMACRO_H

#include "ProcessCommand.h"
#include <vector>
#include <exception>

class ProcessCommandMacro : public ProcessCommand {
public:

    // Executes all of the ProcessCommands in the order in which they were added.
    virtual void execute(sample_t* first, sample_t* last, sample_t* result);
    using ProcessCommand::execute;

    // Resets all of the contained ProcessCommands.
    virtual void reset();

    // Prints the name of each ProcessCommand in order.
    virtual std::string toString() const;

    // Destroys the instance and all stored ProcessCommands.
    virtual ~ProcessCommandMacro();


    // Gets a pointer to the ProcessCommand at index i.
    ProcessCommand* get(std::size_t i);

    // Adds a ProcessCommand to the macro
    void add(ProcessCommand* process);
    void add(ProcessCommand* process, std::size_t i);

    // Returns the number of ProcessCommands stored
    std::size_t count() const;

    // Removes and destroys the ProcessCommand at index i.
    void remove(std::size_t i);

    // Swaps the ProcessCommand at index orig with index dest.
    void swap(std::size_t orig, std::size_t dest);

    // Moves the ProcessCommand at index orig to index dest.
    void reorder(std::size_t orig, std::size_t dest);

private:
    // throws if i >= count()
    void rangeCheck(std::size_t i) const;

    std::vector<ProcessCommand*> processCommands; // the list of stored ProcessCommands

};

#endif //PROCESSCOMMANDMACRO_H
