//
// Created by Josh Wilson on 5/12/17.
//

#ifndef SYSTEMENDIANNESS_H
#define SYSTEMENDIANNESS_H

// Enables use of IS_LITTLE_ENDIAN.

class { // acts like a constant boolean value
private:
    union { // assumes little endian system
        unsigned multibyte = 1;
        char lowbyte;
    };
public:
    operator bool() const {
        return lowbyte != 0;
    }
    bool operator! () const{
        return !bool(*this);
    }
} IS_LITTLE_ENDIAN; // static instance of anonymous class

#endif //SYSTEMENDIANNESS_H
