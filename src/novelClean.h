#pragma once

#include "ioLib.h"

constexpr const char* CHAPTER_CODE_WORD = "Глава ";
constexpr size_t      MAX_LINE_LENGTH   = 128; 

void   skipLine   (unsigned char** currentSymbol);
size_t cleanNovel (unsigned char* inputFileBuffer, size_t inputFileSize, unsigned char* outputBuffer);
