/*
 *     uarray2.h
 *     by nozden01 & bdioni01, 2/12/2024
 *     iii
 *
 *     Contains struct and function declarations for the 2D Uarray data 
 *     structure. Includes libraries and files necessary for this structure to 
 *     function.
 */

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "uarray.h"
#include "mem.h"

typedef struct UArray2_T
{
        UArray_T uarr2;
} *UArray2_T;

void UArray2_map_col_major(UArray2_T arr,
                           void apply(int col,
                                      int row, 
                                      UArray2_T arr,
                                      void *elem,
                                      void *closure),
                           void *closure);
void UArray2_map_row_major(UArray2_T arr,
                           void apply(int col,
                                      int row,
                                      UArray2_T arr,
                                      void *elem,
                                      void *closure),
                           void *closure);
void *UArray2_at(UArray2_T arr, int col, int row);
UArray2_T UArray2_new(int col, int row, int size);
int UArray2_width(UArray2_T arr);
int UArray2_height(UArray2_T arr);
int UArray2_size(UArray2_T arr);
void UArray2_free(UArray2_T *arr);

#endif