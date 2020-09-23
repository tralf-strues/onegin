#include "novelClean.h"

#include<stdlib.h>

//-----------------------------------------------------------------------------
//! Moves currentSymbol pointer to the first character of the next line. 
//!
//! @param [out]  currentSymbol  
//-----------------------------------------------------------------------------
void skipLine(unsigned char** currentSymbol)
{
    while (**currentSymbol != '\n')
        (*currentSymbol)++;

    (*currentSymbol)++;
}

//-----------------------------------------------------------------------------
//! Cleans novel of garbage. Skips lines that don't contain any cyrilic letters
//! and lines "Глава ...".
//!
//! @param [in]   inputFileBuffer  
//! @param [in]   inputFileSize
//! @param [out]  outputBuffer
//! 
//! @return number of characters in outputBuffer.
//-----------------------------------------------------------------------------
size_t cleanNovel(unsigned char* inputFileBuffer, size_t inputFileSize, unsigned char* outputBuffer)
{
    if (inputFileBuffer == NULL || outputBuffer == NULL)
        return 1;

    unsigned char* currentSymbol              = inputFileBuffer;
    unsigned char* currentOutputSymbol        = outputBuffer;

    bool           isThereCyrilicLetterInLine = 0;
    unsigned char* currentLineStart           = outputBuffer;
    unsigned char* versePointer               = NULL;
    unsigned char* newLinePointer             = NULL;
    while (currentSymbol - inputFileBuffer < inputFileSize)
    {
        isThereCyrilicLetterInLine = 0;
        currentLineStart           = currentSymbol;

        // don't add empty lines
        if (*currentSymbol == '\n')
        {
            currentSymbol++;
            continue;
        }

        versePointer   = (unsigned char*) strFind((const char*)currentLineStart, 
                                                  (const char*)CHAPTER_CODE_WORD, 
                                                  MAX_LINE_LENGTH);
        newLinePointer = (unsigned char*) strFind((const char*)currentLineStart, 
                                                  (const char*)"\n",             
                                                  MAX_LINE_LENGTH);
        if (versePointer != NULL && newLinePointer != NULL && versePointer < newLinePointer)
        {
            skipLine(&currentSymbol);
            continue;
        }

        while (*currentSymbol != '\n')
        {
            if (isCyrilicLetter(*currentSymbol))
                isThereCyrilicLetterInLine = 1;

            *currentOutputSymbol = *currentSymbol;
            currentOutputSymbol++;
            currentSymbol++;
        }

        if (!isThereCyrilicLetterInLine)
            currentOutputSymbol = currentOutputSymbol - (currentSymbol - currentLineStart);
        else
            *(currentOutputSymbol++) = '\n';

        currentSymbol++;
    }

    *(currentOutputSymbol - 1) = '\n';
    *currentOutputSymbol = '\0';

    return currentOutputSymbol - outputBuffer + 1;
}