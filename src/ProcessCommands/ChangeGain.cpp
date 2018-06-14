//
// Created by Josh Wilson on 5/10/17.
//

#include "ProcessCommands/ChangeGain.h"
#include "Helpers.h"

void Gain::execute(sample_t* first, sample_t* last, sample_t* result)
{
    std::transform(first, last, result, [this] (sample_t sample) {
        return sample * gain;
    });
}

Gain::Gain(double gain_db): gain(Helpers::dbToAmp(gain_db))
{
}