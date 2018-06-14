//
// Created by Josh Wilson on 4/27/17.
//

#include "../include/Factory.h"
#include "../include/Helpers.h"
#include <cmath>

Factory::Factory(double sampleRate, unsigned numChannels)
        :_sampleRate(sampleRate), _numChannels(numChannels)
{}


Gain* Factory::makeChangeGain(double gain)
{
    return new Gain(gain);
}

// Filter
Filter* Factory::makeLowpassFilter(double freq, double Q)
{
    double w0, cos_w0, alpha, a0;
    w0 = 2.0 * Helpers::PI * (freq / sampleRate());
    cos_w0 = std::cos(w0);
    alpha = std::sin(w0) * 0.5 / Q;
    a0 = 1.0 / (1.0 + alpha); // reciprocated, to normalize filter

    Filter* fil = new Filter(); // ProcessExecutor manages memory
    fil->b1 = (1.0 - cos_w0) * a0;
    fil->b0 = fil->b2 = fil->b1 * 0.5;
    fil->a1 = 2.0 * cos_w0 * a0;
    fil->a2 = (-1.0 + alpha) * a0;
    // a1 and a2 are negated, add all in filter function

    return fil;
}

Filter* Factory::makeHighpassFilter(double freq, double Q)
{
    double w0, cos_w0, alpha, a0;
    w0 = 2.0 * Helpers::PI * (freq / sampleRate());
    cos_w0 = std::cos(w0);
    alpha = std::sin(w0) * 0.5 / Q;
    a0 = 1.0 / (1.0 + alpha); // reciprocated, to normalize filter

    Filter* fil = new Filter(); // ProcessExecutor manages memory
    fil->b1 = (-1.0 - cos_w0) * a0;
    fil->b0 = fil->b2 = fil->b1 * -0.5;
    fil->a1 = 2.0 * cos_w0 * a0;
    fil->a2 = (-1.0 + alpha) * a0;
    // a1 and a2 are negated, add all in filter function

    return fil;
}

Filter* Factory::makeBandpassFilter(double freq, double Q)
{
    double w0, cos_w0, alpha, a0;
    w0 = 2.0 * Helpers::PI * (freq / sampleRate());
    cos_w0 = std::cos(w0);
    alpha = std::sin(w0) * 0.5 / Q;
    a0 = 1.0 / (1.0 + alpha); // reciprocated, to normalize filter

    Filter* fil = new Filter(); // ProcessExecutor manages memory
    fil->b0 = Q * alpha * a0; // constant skirt gain, peak gain = Q, drop Q for peak 0dB
    fil->b1 = 0.0;
    fil->b2 = -(fil->b0);
    fil->a1 = 2.0 * cos_w0 * a0;
    fil->a2 = (-1.0 + alpha) * a0;
    // a1 and a2 are negated, add all in filter function

    return fil;
}

Filter* Factory::makeAllpassFilter(double freq, double Q)
{
    double w0, cos_w0, alpha, a0;
    w0 = 2.0 * Helpers::PI * (freq / sampleRate());
    cos_w0 = std::cos(w0);
    alpha = std::sin(w0) * 0.5 / Q;
    a0 = 1.0 / (1.0 + alpha); // reciprocated, to normalize filter

    Filter* fil = new Filter(); // ProcessExecutor manages memory
    fil->b0 = (1.0 - alpha) * a0;
    fil->b1 = -2.0 * cos_w0 * a0;
    fil->b2 = 1.0;
    fil->a1 = -(fil->b1);
    fil->a2 = -(fil->b0);
    // a1 and a2 are negated, add all in filter function

    return fil;
}

Filter* Factory::makePeakFilter(double freq, double Q, double gain)
{
    double A, w0, cos_w0, alpha, a0;
    A = std::pow(10.0, gain / 40.0);
    w0 = 2.0 * Helpers::PI * (freq / sampleRate());
    cos_w0 = std::cos(w0);
    alpha = std::sin(w0) * 0.5 / Q;
    a0 = 1.0 / (1.0 + alpha / A); // reciprocated, to normalize filter

    Filter* fil = new Filter(); // ProcessExecutor manages memory
    fil->b0 = (1.0 + alpha * A) * a0;
    fil->b1 = -2.0 * cos_w0 * a0;
    fil->b2 = (1.0 - alpha * A) * a0;
    fil->a1 = -(fil->b1);
    fil->a2 = (-1.0 + alpha / A) * a0;
    // a1 and a2 are negated, add all in filter function

    return fil;
}

Filter* Factory::makeLowshelfFilter(double freq, double Q, double gain)
{
    double A, Aplus1, Aminus1, w0, cos_w0, alpha, a0;
    A = std::pow(10.0, gain / 40.0);
    Aplus1 = A + 1.0;
    Aminus1 = A - 1.0;
    w0 = 2.0 * Helpers::PI * (freq / sampleRate());
    cos_w0 = std::cos(w0);
    alpha = std::sin(w0) * 0.5 / Q;
    a0 = 1.0 / (Aplus1 + Aminus1 * cos_w0 + 2.0 * std::sqrt(A) * alpha); // reciprocated, to normalize filter

    Filter* fil = new Filter(); // ProcessExecutor manages memory
    fil->b0 = A * (Aplus1 - Aminus1 * cos_w0 + 2.0 * std::sqrt(A) * alpha) * a0;
    fil->b1 = 2.0 * A * (Aminus1 - Aplus1 * cos_w0) * a0;
    fil->b2 = fil->b0 - 4.0 * A * std::sqrt(A) * alpha * a0;
    fil->a1 = 2.0 * (Aminus1 + Aplus1 * cos_w0) * a0;
    fil->a2 = -(Aplus1 + Aminus1 * cos_w0 - 2.0 * std::sqrt(A) * alpha) * a0;
    // a1 and a2 are negated, add all in filter function

    return fil;
}

Filter* Factory::makeHighshelfFilter(double freq, double Q, double gain)
{
    double A, Aplus1, Aminus1, w0, cos_w0, alpha, a0;
    A = std::pow(10.0, gain / 40.0);
    Aplus1 = A + 1.0;
    Aminus1 = A - 1.0;
    w0 = 2.0 * Helpers::PI * (freq / sampleRate());
    cos_w0 = std::cos(w0);
    alpha = std::sin(w0) * 0.5 / Q;
    a0 = 1.0 / (Aplus1 - Aminus1 * cos_w0 + 2.0 * std::sqrt(A) * alpha); // reciprocated, to normalize filter

    Filter* fil = new Filter(); // ProcessExecutor manages memory
    fil->b0 = A * (Aplus1 + Aminus1 * cos_w0 + 2.0 * std::sqrt(A) * alpha) * a0;
    fil->b1 = -2.0 * A * (Aminus1 + Aplus1 * cos_w0) * a0;
    fil->b2 = fil->b0 - 4.0 * A * std::sqrt(A) * alpha * a0;
    fil->a1 = -2.0 * (Aminus1 - Aplus1 * cos_w0) * a0;
    fil->a2 = -(Aplus1 - Aminus1 * cos_w0 - 2.0 * std::sqrt(A) * alpha) * a0;
    // a1 and a2 are negated, add all in filter function

    return fil;
}

Compressor* Factory::makeCompressor(double threshold, double ratio, double attackTime,
                                   double releaseTime, double postgain)
{
    double attack, release, slope;
    attack = (attackTime > 0.0 ? exp(-1.0 / (sampleRate() * attackTime)) : 0);
    release = (releaseTime > 0.0 ? exp(-1.0 / (sampleRate() * releaseTime)) : 0);
    slope = (ratio >= 1.0 ? (1.0 - 1.0 / ratio) : 1.0); // ratio < 1 gives limiter
    postgain = Helpers::dbToAmp(postgain);
    return new Compressor(threshold, slope, attack, release, postgain);
    // ProcessExecutor manages memory
}
