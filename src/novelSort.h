#pragma once

#include <stdlib.h>

struct string
{
    unsigned char* str    = NULL;
    size_t         length = 0;
};

void   swapValues                  (void* value1, void* value2, size_t valueSize);
size_t qsortPartition              (void*  values, size_t left, 
                                    size_t right,  size_t valueSize, 
                                    int (*compare)(const void* value1, const void* value2));
void   qsort                       (void*  values, size_t left, 
                                    size_t right,  size_t valueSize, 
                                    int (*compare)(const void* value1, const void* value2));
int    strCmpForSortAlphabetically (void *str1, void *str2);
int    strCmpForSortReversely      (void *str1, void *str2);


