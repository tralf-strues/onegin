#include <assert.h>

#include "ioLib.h"
#include "novelSort.h"

//-----------------------------------------------------------------------------
//! Swaps two values of size valueSize with each other.
//!
//! @param [out]  value1  
//! @param [out]  value2
//! @param [in]   valueSize
//-----------------------------------------------------------------------------
void swapValues(void* value1, void* value2, size_t valueSize)
{
    assert(value1    != NULL);
    assert(value2    != NULL);
    assert(valueSize != 0);

    for (size_t k = 0; k < valueSize; k++)
    {
        char temp = *((char*)value1 + k);
        *((char*)value1 + k) = *((char*)value2 + k);
        *((char*)value2 + k) = temp;
    }
}

//-----------------------------------------------------------------------------
//! Partitioning function for quick sort. Chooses the last elements as pivot. 
//!
//! @param [out]  values  
//! @param [in]   left
//! @param [in]   right
//! @param [in]   valueSize
//! @param [in]   compare
//! 
//! @return index of the pivot.
//-----------------------------------------------------------------------------
size_t qsortPartition(void* values, size_t left, size_t right, size_t valueSize, int (*compare)(const void* value1, const void* value2))
{
    size_t indexOfResultPivot = left;
    void* pivot = (char*)values + right * valueSize;
    for (size_t i = left; i < right; i++)
    {
        if (compare(((char*)values + i * valueSize), pivot) < 0)
        {
            swapValues((void*)((char*)values + indexOfResultPivot * valueSize), (void*)((char*)values + i * valueSize), valueSize);
            indexOfResultPivot++;
        }
    }

    swapValues((void*)((char*)values + indexOfResultPivot * valueSize), (void*)((char*)values + right * valueSize), valueSize);

    return indexOfResultPivot;
}

//-----------------------------------------------------------------------------
//! Sorts values using compare function. Which returns positive number if
//! value1 > value2, negative if value1 < value2 and 0 if they are equal.
//!
//! @param [out]  values  
//! @param [in]   left
//! @param [in]   right
//! @param [in]   valueSize
//! @param [in]   compare
//-----------------------------------------------------------------------------
void qsort(void* values, size_t left, size_t right, size_t valueSize, int (*compare)(const void* value1, const void* value2))
{
    if (left >= right)
        return;

    size_t mid = qsortPartition(values, left, right, valueSize, compare);
    if (mid != 0)
    {
        qsort(values, left, mid - 1L, valueSize, compare);
    }

    qsort(values, mid + 1L, right, valueSize, compare);

}

//-----------------------------------------------------------------------------
//! Alphabetical string comparator for qsort. 
//!
//! @param [in]  str1  
//! @param [in]  str2
//! 
//! @return positive number if str1 > str2, negative if str1 < str2 and 0 if 
//!         they are equal.
//-----------------------------------------------------------------------------
int strCmpForSortAlphabetically(void *str1, void *str2)
{	
    unsigned char* ptr1 = (*((string*) str1)).str;
    unsigned char* ptr2 = (*((string*) str2)).str;

    unsigned char strTermination = getStringTermination();
    while(true)
    {
        if (isPunctuationMark(*ptr1) || isLatinLetter(*ptr1))
        {
            ptr1++;
            continue;
        }

        if (isPunctuationMark(*ptr2) || isLatinLetter(*ptr2))
        {
            ptr2++;
            continue;
        }

        if (*ptr1 == strTermination || *ptr2 == strTermination || toLowerCase(*ptr1) != toLowerCase(*ptr2))
            break;

        ptr1++;
        ptr2++;
    }

    return toLowerCase(*ptr1) - toLowerCase(*ptr2);
}

//-----------------------------------------------------------------------------
//! Reverse string comparator for qsort. 
//!
//! @param [in]  str1  
//! @param [in]  str2
//! 
//! @return positive number if str1 > str2, negative if str1 < str2 and 0 if 
//!         they are equal.
//-----------------------------------------------------------------------------
int strCmpForSortReversely(void *str1, void *str2)
{	
    unsigned char* ptr1 = (*((string*) str1)).str + (*((string*) str1)).length - 1;
    unsigned char* ptr2 = (*((string*) str2)).str + (*((string*) str2)).length - 1;

    unsigned char strTermination = getStringTermination();
    while(true)
    {
        if (isPunctuationMark(*ptr1) || isLatinLetter(*ptr1))
        {
            ptr1--;
            continue;
        }

        if (isPunctuationMark(*ptr2) || isLatinLetter(*ptr2))
        {
            ptr2--;
            continue;
        }

        if (*ptr1 == strTermination || *ptr2 == strTermination || toLowerCase(*ptr1) != toLowerCase(*ptr2))
            break;

        ptr1--;
        ptr2--;
    }

    return toLowerCase(*ptr1) - toLowerCase(*ptr2);
}
