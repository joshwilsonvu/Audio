//
// Created by Josh Wilson on 5/12/17.
//

#include "../include/AudioFile.h"
#include "../include/Helpers.h"
#include <vector>

// Class type static member initialized out of line.
std::set<std::string> *AudioFile::audioFileSet = nullptr;
unsigned AudioFile::audioFileCount = 0;
const std::string AudioFile::appendToProcessedFilename = "_processed";
//const std::string AudioFile::appendToTemporaryFilename = "_temp";

AudioFile::AudioFile(const std::string &filename)
:fname_(filename)
{
    if (audioFileCount++ == 0) {
        audioFileSet = new std::set<std::string>{
                "wav",
                "8svx",
                "aif",
                "aifc",
                "aiff",
                "aiffc",
                "al",
                "amb",
                "au",
                "avr",
                "caf",
                "cdda",
                "cdr",
                "cvs",
                "cvsd",
                "cvu",
                "dat",
                "dvms",
                "f32",
                "f4",
                "f64",
                "f8",
                "fap",
                "flac",
                "fssd",
                "gsm",
                "gsrt",
                "hcom",
                "htk",
                "ima",
                "ircam",
                "la",
                "lpc",
                "lpc10",
                "lu",
                "mat",
                "mat4",
                "mat5",
                "maud",
                "nist",
                "ogg",
                "paf",
                "prc",
                "pvf",
                "raw",
                "s1",
                "s16",
                "s2",
                "s24",
                "s3",
                "s32",
                "s4",
                "s8",
                "sb",
                "sd2",
                "sds",
                "sf",
                "sl",
                "sln",
                "smp",
                "snd",
                "sndfile",
                "sndr",
                "sndt",
                "sou",
                "sox",
                "sph",
                "sw",
                "txw",
                "u1",
                "u16",
                "u2",
                "u24",
                "u3",
                "u32",
                "u4",
                "u8",
                "ub",
                "ul",
                "uw",
                "vms",
                "voc",
                "vorbis",
                "vox",
                "w64",
                "wavpcm",
                "wv",
                "wve",
                "xa",
                "xi"
        };
        // Initialize the set with the supported types
    }

    type_ = getType(fname_);
    if (type_.empty()) {
        throw std::invalid_argument("Filename is not a valid file type.");
    } else if (type_ != "wav") {
        // "foo.aif" - getExtension() and getPath() guaranteed to succeed
        std::string newFilename = getPath(fname_) + ".wav";
        std::string commandLine("./sox " + fname_ + ' ' + newFilename);
        // "./sox foo.aif foo.wav"
        std::system(commandLine.c_str());

        inFile_.open(newFilename, std::ios_base::binary); // "foo.wav"
        outFile_.open(getPath(fname_) + appendToProcessedFilename
                      + ".wav",
                      std::ios_base::binary); // "foo_processed.wav"
        fname_ = newFilename;
    } else {
        inFile_.open(fname_, std::ios_base::binary); // "foo.wav"
        outFile_.open(getPath(fname_) + appendToProcessedFilename + ".wav",
                      std::ios_base::binary); // "foo_processed.wav"
    }
    readHeader();
    writeHeader();
}

AudioFile::~AudioFile()
{
    inFile_.close();
    outFile_.close();

    if (type_ != "wav") {
        std::string fname_path = getPath(fname_);

        std::string originalWav = fname_path + ".wav"; // "foo.wav"

        std::string processedWav = fname_path + appendToProcessedFilename + ".wav";
        // "foo_processed.wav"

        std::string processedExt = fname_path + appendToProcessedFilename + '.' + type_;
        // "foo_processed.aif"

        std::string commandLine = "./sox " + processedWav + " " + processedExt;
        // "./sox foo_processed.wav foo_processed.aif"

        if (std::system(commandLine.c_str())) { // creates file from temporary processed file
            std::remove(processedExt.c_str()); // removes file if sox fails
        }
        std::remove(originalWav.c_str()); // removes temporary file copied from original
        std::remove(processedWav.c_str()); // removes temporary processed file
    }

    if (--audioFileCount == 0) {
        delete audioFileSet;
        audioFileSet = nullptr;
    }
}

bool AudioFile::is_open() const
{
    return inFile_.is_open() && bool(inFile_);
    //return sampleRate() != 0;
}

size_t AudioFile::load(sample_t *buffer, size_t bufferSize)
{
    // read into charBuffer from inFile_ and writes data as sample_t to buffer
    std::vector<char> charBuffer(bufferSize * bytesPerSample_);
    inFile_.read(charBuffer.data(), charBuffer.size());
    size_t lastSampleCount = (size_t)inFile_.gcount() / bytesPerSample_;

    for (size_t i = 0; i < lastSampleCount; i++) {
        buffer[i] = getSample(charBuffer.data() + bytesPerSample_ * i);
    }
    return lastSampleCount;
}

void AudioFile::write(sample_t *buffer)
{
    size_t lastRemainderBytes = (size_t)inFile_.gcount() % bytesPerSample_;
    size_t lastSampleCount = (size_t)inFile_.gcount() / bytesPerSample_;

    std::vector<char> charBuffer(lastSampleCount * bytesPerSample_);
    // converts sample_t back to char and write to char buffer
    for (size_t i = 0; i < lastSampleCount; i++) {
        convertSample(buffer[i], charBuffer.data() + bytesPerSample_ * i);
    }

    // writes charBuffer to outFile
    outFile_.write(charBuffer.data(), charBuffer.size());
    if (lastRemainderBytes != 0) {
        std::vector<char> zeroBuff(lastRemainderBytes, 0);
        outFile_.write(zeroBuff.data(), zeroBuff.size());
    }
}

sample_t AudioFile::getSample(char buff[]) const
{
    return sample_t(Helpers::getSigned(buff, bytesPerSample_))
             / (1l << (bytesPerSample_ * 8 - 1));
    // to floating-point sample
}

void AudioFile::convertSample(sample_t sample, char buff[]) const
{
    union {
        int l;
        char c[sizeof(l)];
    } uni;
    int factor = 1 << (bytesPerSample_ * 8 - 1);
    uni.l = (int)(factor * sample); // int = int * sample_t
    if (uni.l >= factor) {
        uni.l = factor - 1;
        clip();
    } else if (uni.l < -factor) {
        uni.l = -factor;
        clip();
    }
    for (size_t i = 0; i < bytesPerSample_; i++) {
        buff[i] = uni.c[IS_LITTLE_ENDIAN ? 0 : sizeof(uni.l) - 1];
        // gets least significant byte
        uni.l >>= 8;
    }
}

void AudioFile::clip() const
{
    ++numSamplesClipped_;
}

void AudioFile::readHeader()
{
    char headerBuffer[4] = {};
    headerSize_ = 36;
    // The "RIFF" Chunk
    inFile_.read(headerBuffer, 4);
    if (!(headerBuffer[0] == 0x52 && headerBuffer[1] == 0x49
          && headerBuffer[2] == 0x46 && headerBuffer[3] == 0x46)) // "RIFF"
    {
        throw std::invalid_argument("File does not start with \"RIFF\".");
    }
    inFile_.ignore(4); // ignore chunk size

    inFile_.read(headerBuffer, 4);
    if (!(headerBuffer[0] == 0x57 && headerBuffer[1] == 0x41
          && headerBuffer[2] == 0x56 && headerBuffer[3] == 0x45)) // "WAVE
    {
        throw std::invalid_argument("File does not contain \"WAVE\".");
    }

    // The "fmt" Sub-Chunk
    inFile_.read(headerBuffer, 4);
    if (!(headerBuffer[0] == 0x66 && headerBuffer[1] == 0x6D
          && headerBuffer[2] == 0x74 && headerBuffer[3] == 0x20)) // "fmt "
    {
        throw std::invalid_argument("File does not contain \"fmt \".");
    }

    inFile_.read(headerBuffer, 4);
    size_t fmtChunkSize = Helpers::getUnsigned(headerBuffer, 4);
    if (fmtChunkSize < 16) {
        throw std::invalid_argument("File does not contain enough format information.");
    }

    inFile_.ignore(2); // ignore audio format
    //AUDIO_FORMAT = Helpers::getUnsigned(headerBuffer, 2);

    // Sets numChannels_
    inFile_.read(headerBuffer, 2);
    numChannels_ = Helpers::getUnsigned(headerBuffer, 2);

    // Sets sampleRate_
    inFile_.read(headerBuffer, 4);
    sampleRate_ = Helpers::getUnsigned(headerBuffer, 4);

    inFile_.ignore(4); // ignore byte rate
    //BYTE_RATE = Helpers::getUnsigned(headerBuffer, 4);

    inFile_.ignore(2); // ignore block align
    //BLOCK_ALIGN = Helpers::getUnsigned(headerBuffer, 2);

    // Sets bytesPerSample_
    inFile_.read(headerBuffer, 2);
    bytesPerSample_ = Helpers::getUnsigned(headerBuffer, 2)/8;

    if (fmtChunkSize > 16) {
        inFile_.ignore(fmtChunkSize - 16); //ignore rest of format chunk
        headerSize_ += fmtChunkSize - 16;
    }

    inFile_.read(headerBuffer, 4);
    headerSize_ += 4;
    if (headerBuffer[0] == 0x66 && headerBuffer[1] == 0x61
        && headerBuffer[2] == 0x63 && headerBuffer[3] == 0x74) // "fact"
    {
        inFile_.read(headerBuffer, 4); //ignore fact chunk if exists
        inFile_.ignore(Helpers::getUnsigned(headerBuffer, 4));
        headerSize_ += inFile_.gcount() + 8;
        inFile_.read(headerBuffer, 4);
    }

    // The "data" Chunk
    if (!(headerBuffer[0] == 0x64 && headerBuffer[1] == 0x61
          && headerBuffer[2] == 0x74 && headerBuffer[3] == 0x61)) // "data"
    {
        throw std::invalid_argument("File does not contain \"data\".");
    }

    inFile_.ignore(4); // ignore num frames
    //NUM_FRAMES = getLittleEndian(headerBuffer, 4) / BYTES_PER_SAMPLE / NUM_CHANNELS;
    headerSize_ += 4;
}

void AudioFile::writeHeader()
{
    std::ifstream inFileLocal;
    inFileLocal.open(fname_.c_str(), std::ios_base::binary);
    if (!inFileLocal.good()) {
        throw std::invalid_argument("Unable to open file.");
    }

    std::vector<char> headerBuffer(headerSize_);
    inFileLocal.read(headerBuffer.data(), headerSize_);
    outFile_.write(headerBuffer.data(), headerSize_);
}

std::string AudioFile::getType(const std::string& filename)
{
    std::string ext = getExtension(filename); // get extension
    if (ext.empty()) {
        return ext; // invalid type
    }
    return bool(audioFileSet->count(ext)) ? ext : std::string();
    // return a valid type extension or an empty string
}

std::string AudioFile::getExtension(const std::string& filename, char delim)
{
    size_t pos = filename.rfind(delim);
    if (pos >= filename.length()) {
        return std::string(); // empty string if invalid filename
    }
    std::string ext = filename.substr(pos+1); // get extension
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower); // lowercase
    return ext;
}

std::string AudioFile::getPath(const std::string& filename, char delim)
{
    size_t pos = filename.rfind(delim);
    if (pos >= filename.length()) {
        return filename; // unmodified string if invalid filename
    }
    std::string path = filename.substr(0, pos); // get path
    std::transform(path.begin(), path.end(), path.begin(), ::tolower); // lowercase
    return path;
}