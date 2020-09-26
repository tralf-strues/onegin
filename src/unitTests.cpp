#include <assert.h>

#include "ioLib.h"
#include "novelClean.h"
#include "novelSort.h"
#include "unitTests.h"

static const size_t TITLE_MESSAGE_LENGTH = 49;

void testAll()
{
    testSwap               ();
    testQSort              ();
    testToLowerCase        ();
    testStrNumOfOccurrences();
    testIsCyrilicLetter    ();
    testIsPunctuationMark  ();
}

void printFunctionTitle(const char* functionTitle)
{
    assert(functionTitle != NULL);

    size_t length = strLength(functionTitle);

    consoleWriteChar('\n');
    for (size_t i = 0; i < TITLE_MESSAGE_LENGTH; i++)
        consoleWriteChar('-');
    consoleWriteChar('\n');

    for (size_t i = 0; i < (TITLE_MESSAGE_LENGTH - length) / 2; i++)
        consoleWriteChar(' ');
    consoleWriteLine(functionTitle);

    for (size_t i = 0; i < TITLE_MESSAGE_LENGTH; i++)
        consoleWriteChar('-');
    consoleWriteChar('\n');
}

void printTestResult(size_t testsPassed, size_t numberOfTests)
{
    if (testsPassed == numberOfTests)
        consoleWriteFormatted("All tests have been passed successfully (%d/%d)!\n", testsPassed, numberOfTests);
    else
        consoleWriteFormatted("Tests passed: %d/%d.\n", testsPassed, numberOfTests);
}

void printStringArray(string strings[], size_t size)
{
    consoleWriteString("{");
    for (size_t i = 0; i < size; i++)
        if (i == size - 1)
            consoleWriteFormatted("%s", strings[i].str);
        else
            consoleWriteFormatted("%s, ", strings[i].str);

    consoleWriteString("}");
}

// TESTING swap(void*, void*, size_t)
void testSwap()
{
    printFunctionTitle("Testing swap(void*, void*, size_t)");

    int value1 = 0;
    int value2 = 1;

    swapValues(&value1, &value2, sizeof(int));

    if (value1 == 1 && value2 == 0)
        printTestResult(1, 1);
    else
        printTestResult(0, 1);
}

// TESTING qsort(void*, size_t, size_t, size_t, int (*)(const void*, const void*))
static const size_t QSORT_TESTS_NUMBER = 3;

struct QSortTestCase
{
    string* input         = NULL;
    string* correctOutput = NULL;
};

void testQSort()
{
    printFunctionTitle("Testing qsort(void*, size_t, size_t, size_t, cmp)");

    QSortTestCase testCases[QSORT_TESTS_NUMBER] = {};

    // test case 1
    string input1         [3];
    string correctOutput1 [3];
    input1        [0] = string{(unsigned char*)"Горе",      4};
    input1        [1] = string{(unsigned char*)"zy**Арава", 9};
    input1        [2] = string{(unsigned char*)"(Школа)",   7};
    correctOutput1[0] = string{(unsigned char*)"zy**Арава", 9};
    correctOutput1[1] = string{(unsigned char*)"Горе",      4};
    correctOutput1[2] = string{(unsigned char*)"(Школа)",   7};
    testCases     [0] = QSortTestCase{ input1, correctOutput1 };

    // test case 2
    string input2         [3];
    string correctOutput2 [3];
    input2        [0] = string{(unsigned char*)"Привет",    6};
    input2        [1] = string{(unsigned char*)"-ui\"Ал\"", 7};
    input2        [2] = string{(unsigned char*)"- Ямб -",   7};
    correctOutput2[0] = string{(unsigned char*)"-ui\"Ал\"", 7};
    correctOutput2[1] = string{(unsigned char*)"Привет",    6};
    correctOutput2[2] = string{(unsigned char*)"- Ямб -",   7};
    testCases     [1] = QSortTestCase{ input2, correctOutput2 };

    // test case 3
    string input3         [3];
    string correctOutput3 [3];
    input3        [0] = string{(unsigned char*)"",          0};
    input3        [1] = string{(unsigned char*)"Merci ему", 9};
    input3        [2] = string{(unsigned char*)"Бараны",    6};
    correctOutput3[0] = string{(unsigned char*)"",          0};
    correctOutput3[1] = string{(unsigned char*)"Бараны",    6};
    correctOutput3[2] = string{(unsigned char*)"Merci ему", 9};
    testCases     [2] = QSortTestCase{ input3, correctOutput3 };

    for (size_t i = 0; i < QSORT_TESTS_NUMBER; i++)
        qsort(testCases[i].input, 0, 2, sizeof(string), (int (*)(const void*, const void*)) &strCmpForSortAlphabetically);

    size_t testsPassed = 0;
    for (size_t i = 0; i < QSORT_TESTS_NUMBER; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if (strCompare((const unsigned char*)testCases[i].input[j].str, (const unsigned char*)testCases[i].correctOutput[j].str) != 0)
            {
                consoleWriteFormatted("Test failed: output=");
                printStringArray     (testCases[i].input,         3);
                consoleWriteFormatted(", correct output=");
                printStringArray     (testCases[i].correctOutput, 3);
                consoleWriteFormatted("\n");
                testsPassed--;
                break;
            }
        }
        testsPassed++;
    }

    printTestResult(testsPassed, QSORT_TESTS_NUMBER);
}

//TESTING toLowerCase(unsigned char)
static const size_t TOLOWERCASE_TESTS_NUMBER = 4;

struct ToLowerCaseTestCase
{
    unsigned char input          = 0;
    unsigned char correctOutput = 0;
};

void testToLowerCase()
{
    printFunctionTitle("Testing toLowerCase(unsigned char)");

    ToLowerCaseTestCase testCases[TOLOWERCASE_TESTS_NUMBER] = {{'S', 's'}, {'Б', 'б'}, {' ', ' '}, {'у', 'у'}};
    size_t testsPassed   = 0;
    unsigned char output = 0;
    for (size_t i = 0; i < TOLOWERCASE_TESTS_NUMBER; i++)
    {
        output = toLowerCase(testCases[i].input);
        if (output != testCases[i].correctOutput)
            consoleWriteFormatted("Test failed: output='%c', correct output='%c'\n", output, testCases[i].correctOutput);
        else
            testsPassed++;
    }

    printTestResult(testsPassed, TOLOWERCASE_TESTS_NUMBER);
}

//TESTING strNumOfOccurrences(const char*, char)
static const size_t STR_NUM_OF_OCCURRENCES_TESTS_NUMBER = 4;

struct StrNumOfOccurrencesTestCase
{
    const char* inputStr       = NULL;
    char        inputSymbol    = 0;
    size_t      correctOutput  = 0;
};

void testStrNumOfOccurrences()
{
    printFunctionTitle("Testing strNumOfOccurrences(const char*, char)");

    StrNumOfOccurrencesTestCase testCases[STR_NUM_OF_OCCURRENCES_TESTS_NUMBER] = {{"Fundamental theorem of calculus", 't',  2},
                                                                                  {"И днем и ночью кот ученый все..", 'е',  3},
                                                                                  {"Hello,\n It's me\n I've been\n ", '\n', 3},
                                                                                  {"Hello,\n It's me\n I've been\n ", 'у',  0}};

    size_t testsPassed   = 0;
    size_t output        = 0;
    for (size_t i = 0; i < STR_NUM_OF_OCCURRENCES_TESTS_NUMBER; i++)
    {
        output = strNumOfOccurrences(testCases[i].inputStr, testCases[i].inputSymbol);
        if (output != testCases[i].correctOutput)
            consoleWriteFormatted("Test failed: output=%d, correct output='%d' (input = {\"%s\", '%c'})\n", 
                                  output, 
                                  testCases[i].correctOutput, 
                                  testCases[i].inputStr, 
                                  testCases[i].inputSymbol);
        else
            testsPassed++;
    }

    printTestResult(testsPassed, STR_NUM_OF_OCCURRENCES_TESTS_NUMBER);
}

// TESTING isCyrilicLetter(unsigned char)
static const size_t IS_CYRILIC_LETTER_TESTS_NUMBER = 4;

struct IsCyrilicLetterTestCase
{
    unsigned char input         = 0;
    int           correctOutput = 0;
};

void testIsCyrilicLetter()
{
    printFunctionTitle("Testing isCyrilicLetter(unsigned char)");

    IsCyrilicLetterTestCase testCases[IS_CYRILIC_LETTER_TESTS_NUMBER] = { {'У', 1}, {'\n', 0}, {'Z', 0}, {'ф', 1} };
    size_t testsPassed = 0;
    int    output      = 0;
    for (size_t i = 0; i < IS_CYRILIC_LETTER_TESTS_NUMBER; i++)
    {
        output = isCyrilicLetter(testCases[i].input);
        if (output != testCases[i].correctOutput)
            consoleWriteFormatted("Test failed: output=%d, correct output=%d (input = '%c')\n", 
                                  output, 
                                  testCases[i].correctOutput, 
                                  testCases[i].input);
        else
            testsPassed++;
    }

    printTestResult(testsPassed, IS_CYRILIC_LETTER_TESTS_NUMBER);
}

// TESTING isPunctuationMark(unsigned char)
static const size_t IS_PUNCTUATION_MARK_TESTS_NUMBER = 8;

struct IsPunctuationMarkTestCase
{
    unsigned char input         = 0;
    int           correctOutput = 0;
};

void testIsPunctuationMark()
{
    printFunctionTitle("Testing isPunctuationMark(unsigned char)");

    IsPunctuationMarkTestCase testCases[IS_PUNCTUATION_MARK_TESTS_NUMBER] = { {'У', 0}, {'\n', 0}, {'Z', 0}, {'ф', 0}, 
                                                                              {'.', 1}, {'\"', 1}, {'*', 1}, {'-', 1}};
    size_t testsPassed = 0;
    int    output      = 0;
    for (size_t i = 0; i < IS_PUNCTUATION_MARK_TESTS_NUMBER; i++)
    {
        output = isPunctuationMark(testCases[i].input);
        if (output != testCases[i].correctOutput)
            consoleWriteFormatted("Test failed: output=%d, correct output=%d (input = '%c')\n", 
                                  output, 
                                  testCases[i].correctOutput, 
                                  testCases[i].input);
        else
            testsPassed++;
    }

    printTestResult(testsPassed, IS_PUNCTUATION_MARK_TESTS_NUMBER);
}