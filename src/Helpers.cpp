//
// Created by Josh Wilson on 4/27/17.
//

#include "../include/Helpers.h"
#include "../include/SystemEndianness.h"
#include <cmath>

double Helpers::dbToAmp(double db)
{
    return std::pow(10.0, db / 20.0);
}

double Helpers::ampToDb(double amp)
{
    return 20.0 * std::log10(std::fabs(amp));
}

double Helpers::clamp(double arg, double low, double high)
{
    return arg < high ? (arg < low ? low : arg) : high;
}

int Helpers::clamp(int arg, int low, int high)
{
    return arg < high ? (arg < low ? low : arg) : high;
}

unsigned int Helpers::getUnsigned(char buff[], int size)
{
    union {
        unsigned u = 0;
        char c[sizeof(u)];
    } uni;
    for (long i = size - 1; i >= 0; i--) {
        uni.u <<= 8;
        uni.c[IS_LITTLE_ENDIAN ? 0 : sizeof(uni.u) - 1] = buff[i];
    }
    return uni.u;
}

int Helpers::getSigned(char *buff, int size)
{
    union {
        int s = 0;
        char c[sizeof(s)];
    } uni;
    for (int i = size - 1; i >= 0; --i) {
        *reinterpret_cast<unsigned int*>(&uni.s) <<= 8;
        uni.c[IS_LITTLE_ENDIAN ? 0 : sizeof(uni.s) - 1] = buff[i];
    }
    // if number is two's complement negative
    if (uni.s & 1u << (8 * size - 1)) {
        for (int i = size; i < (int)sizeof(uni.s); ++i) {
            uni.c[IS_LITTLE_ENDIAN ? i : sizeof(uni.s) - 1 - i] = ~static_cast<unsigned char>(0);
            // 1s, sign extension
        }
    }
    return uni.s;
}