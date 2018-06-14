//
// Created by Josh Wilson on 4/27/17.
//

#include "ProcessCommands/ProcessCommandMacro.h"
#include <algorithm>
#include <sstream>

void ProcessCommandMacro::execute(sample_t* first, sample_t* last, sample_t* result)
{
    for (ProcessCommand* pc : processCommands) {
        pc->execute(first, last, result); // execute every ProcessCommand, in order
    }
}

void ProcessCommandMacro::reset()
{
    for (ProcessCommand* pc : processCommands) {
        pc->reset(); // reset every ProcessCommand
    }
}

std::string ProcessCommandMacro::toString() const
{
    std::stringstream out;
    for (ProcessCommand* pc : processCommands) {
        out << pc->toString() << std::endl;
    }
    /* ex.
     Filter
     Compressor
     Gain
    */
    return out.str();
}

ProcessCommandMacro::~ProcessCommandMacro()
{
    for (ProcessCommand* pc : processCommands) {
        delete pc; // calls delete on every ProcessCommand pointer
    }
}


ProcessCommand* ProcessCommandMacro::get(std::size_t i)
{
    return processCommands.begin()[i];
}

void ProcessCommandMacro::add(ProcessCommand *pc)
{
    processCommands.push_back(pc);
}
void ProcessCommandMacro::add(ProcessCommand *pc, size_t i)
{
    processCommands.insert(processCommands.begin() + i, pc);
}

std::size_t ProcessCommandMacro::count() const
{
    return processCommands.size();
}

void ProcessCommandMacro::remove(std::size_t i)
{
    rangeCheck(i);
    auto pos = processCommands.begin() + i; // gets iterator to element to remove
    ProcessCommand* pc = *pos; // gets element at iterator
    processCommands.erase(pos); // removes element from container
    delete pc; // destroys the ProcessCommand
}

void ProcessCommandMacro::swap(std::size_t orig, std::size_t dest)
{
    if (orig != dest) {
        std::iter_swap(processCommands.begin() + orig, processCommands.begin() + dest);
    }
}

void ProcessCommandMacro::reorder(std::size_t orig, std::size_t dest)
{
    if (orig != dest) {
        std::ptrdiff_t direction = dest > orig ? 1 : -1;

        auto origIter = processCommands.begin() + orig, // iter to orig index
        nextIter = origIter,
        destIter = processCommands.begin() + dest; // iter to dest index

        while (origIter != destIter) {
            std::advance(nextIter, direction); // nextIter moves one toward dest, equals dest at end
            std::iter_swap(origIter, nextIter); // move orig data up one, other data down one
            origIter = nextIter; // origIter points to the same data as it did before
        }
    }
}

void ProcessCommandMacro::rangeCheck(std::size_t i) const
{
    if (i >= count()) {
        throw std::out_of_range("No command exists at the given index.");
    }
}

