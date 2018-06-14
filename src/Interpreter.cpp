//
// Created by Josh Wilson on 4/28/17.
//

#include "../include/Interpreter.h"
#include "../include/Helpers.h"
#include "../include/AudioFile.h"
#include <fstream>
#include <memory>

Interpreter* Interpreter::inst = nullptr; // initializes the static instance variable

Interpreter* Interpreter::instance()
{
    if (inst == nullptr) { // only create a new instance if none exists
        inst = new Interpreter();
    }
    return inst; // return the pointer to the only instance
}

Interpreter::~Interpreter()
{
    inst = nullptr; // resets the inst pointer
}

AudioFile* Interpreter::readAudioFile()
{
    std::string fname;
    std::cout << "Enter input file name: ";
    std::getline(std::cin, fname);
    while (true) {
        try {
            return std::unique_ptr<AudioFile>(new AudioFile(fname)).release();
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            std::cout << "Enter input file name: ";
            std::getline(std::cin, fname);
        }
    }
}

void Interpreter::displayProcessCommands()
{
    std::ifstream inFile("../text/AvailableProcessCommands.txt");
    std::string str;
    if (inFile.is_open()) {
        while (std::getline(inFile, str)) {
            std::cout << str << std::endl;
        }
    } else {
        std::cout << "Unable to open text/AvailableProcessCommands.txt";
    }
}

ProcessCommandMacro* Interpreter::readProcessCommands(Factory factory)
{
    std::unique_ptr<ProcessCommandMacro> pcm(new ProcessCommandMacro());
    std::string errorMessage;

    // readProcessCommandImpl returns one of three cases:
    // 1. Non-null ProcessCommand*, meaning a valid command has been made.
    // 2. Null ProcessCommand*, non-empty errorMessage, meaning parsing did not succeed.
    // 3. Null ProcessCommand*, empty errorMessage, meaning the ProcessCommandMacro is complete.
    ProcessCommand* pc = readProcessCommandImpl(factory, errorMessage);
    while (!(pc == nullptr && errorMessage.empty())) {
        if (pc != nullptr) {
            pcm->add(pc); // 1.
        } else {
            std::cout << errorMessage; // 2.
            displayProcessCommands();
        }
        errorMessage.clear();
        pc = readProcessCommandImpl(factory, errorMessage);
    }
    return pcm.release(); // 3.
}

Interpreter::Interpreter()
{
    pcMap["changegain"] = PCType::ChangeGain;
    pcMap["lowpass"] = PCType::Lowpass;
    pcMap["highpass"] = PCType::Highpass;
    pcMap["bandpass"] = PCType::Bandpass;
    pcMap["allpass"] = PCType::Allpass;
    pcMap["peak"] = PCType::Peak;
    pcMap["lowshelf"] = PCType::Lowshelf;
    pcMap["highshelf"] = PCType::Highshelf;
    pcMap["compress"] = PCType::Compress;
    pcMap["process"] = PCType::Process;
    // PCType::Undefined intentionally not included here
}

typename Interpreter::PCType Interpreter::getPCType(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower); // lowercase
    try {
        return pcMap.at(str);
        // return the PCType associated with the string
    } catch (const std::out_of_range&) {
        return PCType::Undefined;
        // if string is not a defined type, return PCType::Undefined
    }
}

std::vector<std::string> Interpreter::tokenizeString(const std::string &input)
{
    size_t pos = 0, newPos = pos - 1;
    std::vector<std::string> tokens;
    do {
        pos = newPos + 1; // 0
        newPos = input.find(' ', pos); // find the next space
        tokens.push_back(input.substr(pos, newPos - pos));
    } while (newPos != std::string::npos);
    return tokens;
}

ProcessCommand* Interpreter::readProcessCommandImpl(Factory factory, std::string& errorMessage)
{
    std::string input;
    std::getline(std::cin, input); // read the next line of input from the user
    if (input.empty()) {
        return nullptr; // if the user just presses enter, run the processing
    }
    std::vector<std::string> tokens = tokenizeString(input);

    PCType pcType = getPCType(tokens[0]); // guaranteed to be at least length 1
    switch (pcType) {

        case PCType::Process:
            return nullptr; // stop reading for more commands, run the main program

        case PCType::ChangeGain:
        {
            double gain;
            if (!getDouble(tokens, 1, gain)) {
                errorMessage = "Parameter (gain) could not be read.";
                return nullptr;
            }
            return factory.makeChangeGain(gain);
        }

        case PCType::Lowpass:
        case PCType::Highpass:
        case PCType::Bandpass:
        case PCType::Allpass:
        case PCType::Peak:
        case PCType::Lowshelf:
        case PCType::Highshelf: // All filter types
        {
            double f0, Q, gain;
            if (!(getDouble(tokens, 1, f0)
                  && getDouble(tokens, 2, Q) // if not all return true,
            )) {
                errorMessage = "Parameter (frequency or Q) could not be read.";
                return nullptr;
            }

            if (f0 <= 0.0 || Q <= 0.0) {
                errorMessage = "Filter frequency or Q are nonpositive.";
                return nullptr;
            }

            f0 = Helpers::clamp(f0, 0.0, factory.sampleRate() / 2.0);
            Q = Helpers::clamp(Q, 0.1, 10.0);

            // Two-parameter filters
            if (pcType == PCType::Lowpass) {
                return factory.makeLowpassFilter(f0, Q);
            } else if (pcType == PCType::Highpass) {
                return factory.makeHighpassFilter(f0, Q);
            } else if (pcType == PCType::Bandpass) {
                return factory.makeBandpassFilter(f0, Q);
            } else if (pcType == PCType::Allpass) {
                return factory.makeAllpassFilter(f0, Q);
            } else if (!getDouble(tokens, 3, gain)) { // initializes gain if possible
                errorMessage = "Parameter (gain) could not be read.";
                return nullptr; // exits if not a two-parameter and gain uninitialized
            }

            // Three-parameter filters
            Helpers::clamp(gain, -20.0, 20.0); // limits gain in filtering
            if (pcType == PCType::Peak) {
                return factory.makePeakFilter(f0, Q, gain);
            } else if (pcType == PCType::Lowshelf) {
                return factory.makeLowshelfFilter(f0, Q, gain);
            } else {
                return factory.makeHighshelfFilter(f0, Q, gain);
            }
        }

        case PCType::Compress:
        {
            double threshold, ratio, attackTime, releaseTime, postgain;
            if (!(getDouble(tokens, 1, threshold)
                  && getDouble(tokens, 2, ratio)
                  && getDouble(tokens, 3, attackTime)
                  && getDouble(tokens, 4, releaseTime)
            )) {
                errorMessage = "Parameter (threshold, ratio, attackTime, or releaseTime) could not be read.";
                return nullptr;
            }
            postgain = 0.0;
            getDouble(tokens, 5, postgain); // optional parameter, 0.0 if left out
            // envelope, attack, release, threshold, slope, postgain
            return factory.makeCompressor(threshold, ratio, attackTime, releaseTime, postgain);
        }
        case PCType::Undefined:
            errorMessage = "Command could not be read.";
            return nullptr;
    }
}

bool Interpreter::getDouble(const std::vector<std::string>& tokens, size_t i, double& d)
{
    if (i >= tokens.size()) {
        return false;
    }
    try {
        d = std::stod(tokens[i]); // stores str's value in d
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
    return true; // and returns true if successful
}