#include "wildcard.h"

// Wildcard implementation itself.
#ifdef __cplusplus
extern "C" {

#endif
bool wildcard(STR_TYPE const * pattern, STR_TYPE const* input)
{
    if (!pattern || !input)
        return false;

    int i, j;
    int inputLength = STR_LEN(input);
    int patternLength = STR_LEN(pattern);
    patternLength = patternLength > MAX_WILDCARD_LEN ? MAX_WILDCARD_LEN : patternLength;

    int state = 0;
    int nfaStateCount = patternLength + 1; // Extra 1 for accepting state of NFA.
    const int sizeInBytes = (nfaStateCount >> BIT_IN_CHAR_LOG2) + (nfaStateCount & 0x7 ? 1 : 0);

    // Only sizeInBytes bytes will be used for NFA.
    char nfaCurrStates[(MAX_WILDCARD_LEN + 1) / BITS_IN_CHAR] = {0};
    char nfaNextStates[(MAX_WILDCARD_LEN + 1) / BITS_IN_CHAR] = {0};

    char* pCurrStates = nfaCurrStates;
    char* pNextStates = nfaNextStates;
    char* pSwap;

    // NFA starts in zero state. Here and further: while corresponding pattern symbol
    // is equal STAR_CHARACTER, we do epsilon-transition to the next state.
    addState(pCurrStates, state);
    while (state < patternLength && pattern[state] == STAR_CHARACTER)
        addState(pCurrStates, ++state);

    for (i = 0; i < inputLength; i++)
    {
        for (j = 0; j < patternLength; j++)
        {
            state = j;
            if (!checkState(pCurrStates, state))
                continue;

            STR_TYPE c = pattern[state];

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

#ifdef __cplusplus
}
#endif
