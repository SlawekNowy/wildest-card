// Wildest card is a tiny single-header wildcard matching library
// Written by:   Roman Shchekin aka QtRoS
// Licence:      MIT https://choosealicense.com/licenses/mit/
// Project home: https://github.com/QtRoS/wildest-card
// Version:      v1.2.1

#ifndef WILDCARD_H
#define WILDCARD_H

#ifdef __cplusplus
extern "C" {

#endif
#include <memory.h>
#include <stdbool.h>

// High level defines, can be castomized to wchar_t and wcslen for example.
#define STR_TYPE char
#define STR_LEN strlen
#define STAR_CHARACTER '*'
#define QUESTION_CHARACTER '?'

// Low level defines, used in compact NFA implementation.
#define BITS_IN_CHAR (sizeof(char) * 8)
#define BIT_IN_CHAR_LOG2 3
#define MAX_WILDCARD_LEN 255  // Only 2^n-1 values are acceptable, e.g. 7, 31, 1023 

// Very fast and compact bitwise NFA implementation.
#define addState(_bitArray, _pos) \
    { \
        char *bitArray = _bitArray; \
        int pos = _pos; \
        (bitArray)[(pos) / BITS_IN_CHAR] = (bitArray)[(pos) / BITS_IN_CHAR] | ( 1 << ((pos) % BITS_IN_CHAR )); \
    }

#define checkState(bitArray,pos) (bitArray[pos / BITS_IN_CHAR] & ( 1 << (pos % BITS_IN_CHAR )))

#define resetStates(_pStates, _size) memset(_pStates, 0, _size)

// Wildcard implementation itself.
inline bool wildcard(STR_TYPE const * pattern, STR_TYPE const* input)
{
    if (!pattern || !input)
        return false;
}

bool wildcard(STR_TYPE const * pattern, STR_TYPE const* input);
#ifdef __cplusplus
}
#endif
