#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
#include <locale.h>

#include "ioLib.h"
#include "novelClean.h"
#include "novelSort.h"
#include "unitTests.h"

constexpr const char* INPUT_DEFAULT_FILENAME  = "res/onegin_raw_input.txt";
constexpr const char* OUTPUT_DEFAULT_FILENAME = "res/onegin_output.txt";
constexpr size_t      TITLE_MESSAGE_LENGTH    = 100;

//-----------------------------------------------------------------------------
//! Asks to choose an option from first to last from the user (e.g. '0'-'3').
//! If input is incorrect asks to enter option again.
//!
//! @param [in]  first  
//! @param [in]  last   
//!
//! @return option from first to last.
//-----------------------------------------------------------------------------
char getOption(char first, char last)
{
    assert(first < last);

    consoleWriteFormatted("  Choose an option (%c-%c): ", first, last);
    char option = (char) consoleNextChar();

    while(option < first || option > last)
    {
        consoleWriteFormatted("  Incorrect input. Please enter a number from %c to %c: ", first, last);
        consoleMoveToNextLine();
        option = (char) consoleNextChar();
    }

    consoleMoveToNextLine();

    return option;
}

//-----------------------------------------------------------------------------
//! Asks user to enter two filenames.
//!
//! @param [in]   message1  
//! @param [in]   message2
//! @param [in]   defaultFilename1
//! @param [in]   defaultFilename2
//! @param [out]  filename1
//! @param [out]  filename2
//!
//! @return 0 if there was no error and non-zero value otherwise.
//-----------------------------------------------------------------------------
int requestTwoFilenames(const char* message1,         const char* message2, 
                        const char* defaultFilename1, const char* defaultFilename2,
                        char**      filename1,        char**      filename2)
{
    if (message1         == NULL || message2         == NULL ||
        defaultFilename1 == NULL || defaultFilename2 == NULL ||
        filename1        == NULL || filename2        == NULL)
        return -1;
    
    // choosing file 1
    consoleWriteFormatted("%s", message1);
    consoleWriteFormatted("  [0] default: \"%s\"\n", defaultFilename1);
    consoleWriteFormatted("  [1] custom\n");
    char optionFile1 = getOption('0', '1');
    if (optionFile1 == '1')
    {
        consoleWriteFormatted("\n~Enter filename (the file must be inside \"res/\" folder): ");
        consoleNextLine(*(char**)filename1, strLength(*(char**)filename1) + 1);
        consoleMoveToNextLine();
    }

    // choosing file 2
    consoleWriteFormatted("%s", message2);
    consoleWriteFormatted("  [0] default: \"%s\"\n", defaultFilename2);
    consoleWriteFormatted("  [1] custom\n");
    char optionFile2 = getOption('0', '1');
    if (optionFile2 == '1')
    {
        consoleWriteFormatted("\n~Enter filename (the file must be inside \"res/\" folder): ");
        consoleNextLine(*filename2, strLength(*filename2) + 1);
        consoleMoveToNextLine();
    }

    *filename1 = optionFile1  == '0' ? (char*) defaultFilename1 : *filename1;
    *filename2 = optionFile2  == '0' ? (char*) defaultFilename2 : *filename2;

    return 0;
}

//-----------------------------------------------------------------------------
//! Writes message inside bars of size TITLE_MESSAGE_LENGTH to outputFile.
//!
//! @param [in]  outputFile
//! @param [in]  message
//-----------------------------------------------------------------------------
void writeTitleMessage(File* outputFile, const char* message)
{
    assert(outputFile != NULL);
    assert(message    != NULL);
    
    size_t length = strLength(message);
    
    writeChar(outputFile, '\n');
    for (size_t i = 0; i < TITLE_MESSAGE_LENGTH; i++)
        writeChar(outputFile, '=');
    writeChar(outputFile, '\n');

    for (size_t i = 0; i < (TITLE_MESSAGE_LENGTH - length) / 2; i++)
        writeChar(outputFile, ' ');
    writeLine(outputFile, message);

    for (size_t i = 0; i < TITLE_MESSAGE_LENGTH; i++)
        writeChar(outputFile, '=');
    writeChar(outputFile, '\n');
    writeChar(outputFile, '\n');
}

//-----------------------------------------------------------------------------
//! Prints strings from strIndex to outputFile.
//!
//! @param [in]  outputFile
//! @param [in]  strIndex
//! @param [in]  numberOfLines
//-----------------------------------------------------------------------------
void printStringBuffer(File* outputFile, string* strIndex, size_t numberOfLines)
{
    assert(outputFile    != NULL);
    assert(strIndex      != NULL);
    assert(numberOfLines != NULL);

    for(size_t i = 0; i < numberOfLines; i++)
    {
        writeBufferToFile(outputFile, sizeof(unsigned char), strIndex[i].length, strIndex[i].str);
        writeChar(outputFile, '\n');
    }
}

//-----------------------------------------------------------------------------
//! Initializes strIndex based on stringBuffer.
//!
//! @param [out] strIndex
//! @param [in]  stringBuffer
//! @param [in]  stringBufferSize
//-----------------------------------------------------------------------------
void initializeStrIndex(string* strIndex, unsigned char* stringBuffer, size_t stringBufferSize)
{
    assert(strIndex != NULL);

    strIndex[0].str      = stringBuffer;
    size_t currentLine   = 1;
    size_t lastLineStart = 0;

    size_t i = 1;
    for (; i < stringBufferSize - 1; i++)
    {
        if (stringBuffer[i - 1] == '\n' && stringBuffer[i] != '\n')
        {
            strIndex[currentLine].str = &stringBuffer[i];
            strIndex[currentLine - 1].length = i - lastLineStart - 1;
            lastLineStart = i;
            currentLine++;
        }
    }

    strIndex[currentLine - 1].length = i - lastLineStart - 1;
}

//-----------------------------------------------------------------------------
//! Sorts strIndex and writes the result to outputFile. If useDefaultQSort is 0
//! then uses qsort from novelSort.h otherwise uses standard qsort. 
//!
//! @param [out] strIndex
//! @param [in]  numberOfLines
//! @param [out] outputFile
//! @param [in]  compare
//! @param [in]  message
//! @param [in]  useDefaultQSort
//-----------------------------------------------------------------------------
void sort(string* strIndex, size_t numberOfLines, File* outputFile, 
          int (*compare)(const void* value1, const void* value2),
          const char* message, int useDefaultQSort)
{
    setStringTermination('\n');
    if (useDefaultQSort)
        qsort((void*) strIndex, numberOfLines, sizeof(string), compare);
    else
        qsort((void*) strIndex, 0, numberOfLines - 1, sizeof(string), compare);
    setStringTermination('\0');

    writeTitleMessage(outputFile, message);
    printStringBuffer(outputFile, strIndex, numberOfLines);
}

//-----------------------------------------------------------------------------
//! Main dialog. If printOriginal == 0, then doesn't print original raw novel,
//! otherwise prints it.
//!
//! @param [in]  printOriginal
//-----------------------------------------------------------------------------
void dialogMain(bool printOriginal)
{
    char* inputFileName  = (char*) calloc(MAX_LINE_LENGTH, sizeof(char));
    char* outputFileName = (char*) calloc(MAX_LINE_LENGTH, sizeof(char));

    requestTwoFilenames("\n~What file do you want to clean?\n", "\n~Into what file to write output?\n",
                        INPUT_DEFAULT_FILENAME,                 OUTPUT_DEFAULT_FILENAME,
                        &inputFileName,                         &outputFileName);

    File* inputFile  = openFile(inputFileName,  'r');
    File* outputFile = openFile(outputFileName, 'w');
    assert(inputFile  != NULL);
    assert(outputFile != NULL);

    struct stat inputFileStat;
    stat((char*)inputFileName, &inputFileStat);
    size_t inputFileSize = (size_t) inputFileStat.st_size;

    unsigned char* inputFileBuffer = (unsigned char*) calloc(inputFileSize, sizeof(unsigned char));
    unsigned char* stringBuffer    = (unsigned char*) calloc(inputFileSize, sizeof(unsigned char));
    assert(readBufferFromFile(inputFile, sizeof(unsigned char), inputFileSize, inputFileBuffer) != FILE_END);
    assert(inputFileBuffer != NULL && stringBuffer != NULL);

    closeFile(inputFile);
    if (inputFileName  != INPUT_DEFAULT_FILENAME)
        free(inputFileName);

    // printing original novel
    if (printOriginal)
    {
        writeTitleMessage(outputFile, "Original novel");
        writeBufferToFile(outputFile, sizeof(char), inputFileSize, inputFileBuffer);
    }

    size_t stringBufferSize = cleanNovel         (inputFileBuffer, inputFileSize, stringBuffer);
    size_t numberOfLines    = strNumOfOccurrences((const char*) stringBuffer, '\n');

    // initializing strIndex
    string* strIndex = (string*) calloc(numberOfLines, sizeof(string));
    assert(strIndex != NULL);
    initializeStrIndex(strIndex, stringBuffer, stringBufferSize);

    writeTitleMessage(outputFile, "Cleaned novel");
    printStringBuffer(outputFile, strIndex, numberOfLines);

    sort(strIndex, numberOfLines, outputFile, 
        (int (*)(const void*, const void*)) &strCmpForSortAlphabetically, 
         "Alphabetically sorted novel", 0);
    sort(strIndex, numberOfLines, outputFile, 
        (int (*)(const void*, const void*)) &strCmpForSortReversely, 
         "Reversely sorted novel", 1);

    closeFile(outputFile);

    consoleWriteFormatted("\nEverything has been successfully generated!\n");

    if (outputFileName != OUTPUT_DEFAULT_FILENAME)
        free(outputFileName);
}

//-----------------------------------------------------------------------------
//! Starts the 'user interface'
//-----------------------------------------------------------------------------
void dialogStart()
{
    consoleWriteFormatted("\n=================================================\n");
    consoleWriteFormatted("             What do you want to do?             \n");
    consoleWriteFormatted("  [0] generate an alphabetically and a reversly  \n");
    consoleWriteFormatted("      sorted novels                              \n");
    consoleWriteFormatted("  [1] generate an alphabetically and a reversly  \n");
    consoleWriteFormatted("      sorted novels + print the original novel   \n");
    consoleWriteFormatted("  [2] test program                               \n");
    consoleWriteFormatted("  [3] EXIT                                       \n");
    consoleWriteFormatted("=================================================\n");
    consoleWriteFormatted("=================================================\n\n");

    char mode = getOption('0', '3');
    switch(mode)
    {
        case '0':
            dialogMain(0);
        break;

        case '1':
            dialogMain(1);
        break;

        case '2':
            testAll();
            dialogStart();
        break;

        case '3':
            return;
        break;

        default:
            return;
        break;
    }
}

//-----------------------------------------------------------------------------
//! main function of the program.
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    int testingMode = 0;

    for (size_t i = 0; i < argc; i++)
        if (strCompare((const unsigned char*)argv[i], (const unsigned char*)"-t") == 0)
            testingMode = 1;

    if (testingMode)
        testAll();
    else
        dialogStart();

    return 0;
}