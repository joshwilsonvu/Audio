//
// Created by Josh Wilson on 4/28/17.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <map>
#include "ProcessCommands/ProcessCommandMacro.h"
#include "Factory.h"
#include "AudioFile.h"

class Interpreter {
public:
    // Singleton--get ptr to the only instance of an Interpreter
    static Interpreter* instance();

    // Destroys the instance. To ensure that the destructor is
    // called, store the return of instance() in a std::unique_ptr.
    ~Interpreter();

    // Gets the name and type of an audio file from the user
    AudioFile* readAudioFile();

    // Displays all of the process commands available to the user.
    void displayProcessCommands();

    // Gets a list of process commands ready to go from the user.
    ProcessCommandMacro* readProcessCommands(Factory factory);


private:
    Interpreter(); // the access-controlled constructor. Initializes type map.

    enum class PCType {
        ChangeGain,
        Lowpass,
        Highpass,
        Bandpass,
        Allpass,
        Peak,
        Lowshelf,
        Highshelf,
        Compress,
        Process,
        // Final type for strings that don't match the above
        Undefined
    };



    // Attempts to convert a string into a potential ProcessCommand type code.
    // Returns PCType::Undefined if the string does not represent a defined type.
    PCType getPCType(std::string str);

    // Split a string into single-space separated tokens
    std::vector<std::string> tokenizeString(const std::string& input);

    // The private implementation of readProcessCommand. Makes use of early returns.
    // Returning nullptr
    ProcessCommand* readProcessCommandImpl(Factory factory,
                                           std::string& errorMessage);

    // Attempts to convert a vector element into a double. Returns true if succeeded.
    bool getDouble(const std::vector<std::string>& tokens, size_t i, double& d);

    // The pointer to the only instance of Interpreter
    static Interpreter *inst;

    // The map used in getPCType
    std::map<std::string, PCType> pcMap;

};

#endif //INTERPRETER_H
