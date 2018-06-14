// File name: main.cpp
// Author: Josh Wilson
// userid: wilsoj5
// Email: joshua.wilson@vanderbilt.edu
// Class: CS2201 @ Vanderbilt U.
// Description: This file
//
// Last Changed:

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include "../include/AudioFile.h"
#include "../include/ProcessCommands/ProcessCommandMacro.h"
#include "../include/Interpreter.h"

int main() {

    std::unique_ptr<Interpreter> interpreter(Interpreter::instance());
    // handles singleton memory management

    std::unique_ptr<AudioFile> w1(interpreter->readAudioFile());
    // blocks until an allocated AudioFile is successfuly generated

    unsigned sampleRate = w1->sampleRate();
    unsigned numChannels = w1->numChannels();

    Factory factory(sampleRate, numChannels);
    // creates a factory instance with the given sample rate

    interpreter->displayProcessCommands();
    std::unique_ptr<ProcessCommandMacro> pcCommands(interpreter->readProcessCommands(factory));
    std::cout << '\n' << pcCommands->toString() << std::endl;

    std::vector<sample_t> buffer(1024 * numChannels);

    size_t numSamples = 0;

    while (w1->is_open()) {
        size_t bufferSize = w1->load(buffer.data(), 1024 * numChannels);
        numSamples += bufferSize;

        pcCommands->execute(buffer.data(), buffer.data() + bufferSize);

        w1->write(buffer.data());
    }

    std::cout << numSamples << " samples were processed." << std::endl;

    if (w1->samplesClipped() > 0) {
        std::cout << "Warning: " << w1->samplesClipped()
                  << " samples were clipped." << std::endl;
    }

    return 0;
}