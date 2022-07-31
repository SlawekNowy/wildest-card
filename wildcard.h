// Wildest card is a tiny single-header wildcard matching library
// Written by:   Roman Shchekin aka QtRoS
// Licence:      MIT https://choosealicense.com/licenses/mit/
// Project home: https://github.com/QtRoS/wildest-card
// Version:      v1.0

#include <memory.h>
#include <stdbool.h>

// High level defines, can be castomized to wchar_t and wcslen for example.
#define STR_TYPE char
#define STR_LEN strlen
#define STAR_CHARACTER '*'
#define QUESTION_CHARACTER '?'

// Low level defines, used in compact NFA implementation.
#define BITS_IN_CHAR (sizeof(char) * 8)
#define MAX_WILDCARD_LEN 256

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
bool wildcard(STR_TYPE* pattern, STR_TYPE* input)
{
    if (!pattern || !input)
        return false;
    
    char nfaCurrStates[MAX_WILDCARD_LEN / BITS_IN_CHAR];
    char nfaNextStates[MAX_WILDCARD_LEN / BITS_IN_CHAR];

    char* pCurrStates = nfaCurrStates;
    char* pNextStates = nfaNextStates;
    char* pSwap;

    int i, j;
    int patternLength = STR_LEN(pattern);
    int inputLength = STR_LEN(input);
    int nfaStateCount = patternLength + 1; // Extra 1 for accepting state of NFA.
    int state = 0;

    int sizeInBytes = (nfaStateCount >> 3) + (nfaStateCount & 0x7 ? 1 : 0);
    resetStates(pCurrStates, sizeInBytes);
    resetStates(pNextStates, sizeInBytes);

    // NFA starts in zero state. Here and further: while corresponding pattern symbol
    // is equal STAR_CHARACTER, we do epsilon-transition to the next state. 
    addState(pCurrStates, state);
    while (state < patternLength && pattern[state] == STAR_CHARACTER)
        addState(pCurrStates, ++state);

    for (i = 0; i < inputLength; i++)
    {
        for (j = 0; j < BITS_IN_CHAR * sizeInBytes; j++)
        {
            state = j;
            if (!checkState(pCurrStates, state))
                continue;
            
            STR_TYPE c = pattern[state];

            // Should not be possible, but still worth checking.
            if (state >= patternLength)
                continue;

            if (c == STAR_CHARACTER)
            {
                addState(pNextStates, state);
                while (state < patternLength && pattern[state] == STAR_CHARACTER)
                    addState(pNextStates, ++state);
            }
            else if (c == QUESTION_CHARACTER || input[i] == c)
            {
                addState(pNextStates, ++state);
                while (state < patternLength && pattern[state] == STAR_CHARACTER)
                    addState(pNextStates, ++state);
            }
        }

        pSwap = pCurrStates;
        pCurrStates = pNextStates;
        pNextStates = pSwap;
        
        resetStates(pNextStates, sizeInBytes);
    }

    bool result = checkState(pCurrStates, patternLength); // Check if NFA is in accepting state.
    return result;
}
