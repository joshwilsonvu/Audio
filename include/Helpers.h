//
// Created by Josh Wilson on 4/27/17.
//

#ifndef HELPERS_H
#define HELPERS_H

namespace Helpers {

    constexpr double PI = 3.14159265359;

    double dbToAmp(double db);

    double ampToDb(double amp);

    double clamp(double arg, double low, double high);

    int clamp(int arg, int low, int high);

    unsigned int getUnsigned(char buff[], int size);

    int getSigned(char buff[], int size);

}

#endif //HELPERS_H
