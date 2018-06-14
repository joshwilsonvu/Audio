//
// Created by Josh Wilson on 5/12/17.
//

#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include "SystemEndianness.h"
#include "Sample_t.h"
#include <memory>
#include <set>
#include <string>
#include <fstream>

class AudioFile {

public:
    // Opens an AudioFile instance to the file specified by filename.
    //
    AudioFile(const std::string& filename);

    bool is_open() const;

    // post: returns number of bytes read
    size_t load(sample_t* buffer, size_t bufferSize);

    void write(sample_t* buffer);

    ~AudioFile();

    // the number of concurrent channels
    unsigned numChannels() const { return numChannels_; }

    // the number of samples on each channel per second
    unsigned sampleRate() const { return sampleRate_; }

    unsigned bytesPerSample() const { return bytesPerSample_; }

    unsigned samplesClipped() const { return numSamplesClipped_; }


private:

    /*enum class Type {
        // Currently, the only supported file type
        wav,
        // File types that can be converted to wav via SoX:
        aif,
        // Final type for strings that don't match the above
        Undefined
    };*/

    static std::string getType(const std::string& filename);
    static std::string getExtension(const std::string& filename, char delim = '.');
    static std::string getPath(const std::string& filename, char delim = '.');

    void readHeader();

    void writeHeader();

    sample_t getSample(char buff[]) const;

    void convertSample(sample_t sample, char buff[]) const;

    void clip() const;

    // Non-static members
    std::string type_; // The type of the file being read
    unsigned numChannels_ = 0, sampleRate_ = 0, bytesPerSample_ = 0,
            headerSize_ = 0;
    mutable unsigned numSamplesClipped_ = 0;
    std::string fname_;
    std::ifstream inFile_;
    std::ofstream outFile_;


    // Static members
    static std::set<std::string>* audioFileSet;
    static unsigned audioFileCount;
    const static std::string appendToProcessedFilename/*, appendToTemporaryFilename*/;

};

#endif //AUDIOFILE_H
