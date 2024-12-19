/*
 *     bit2.h
 *     by nozden01 & bdioni01, 2/12/2024
 *     iii
 *
 *     Struct and function declarations for the 2D bitmap interface.
 *     Includes all the necessary files and libraries for the interface to
 *     function.
 */

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bit.h"
#include "mem.h"

typedef struct Bit2_T
{
        Bit_T bitarr2;
        int rows;
        int columns;
} *Bit2_T;

int Bit2_width(Bit2_T bitmap);
int Bit2_height(Bit2_T bitmap);
Bit2_T Bit2_new(int col, int row);
int Bit2_put(Bit2_T bitmap, int col, int row, int bit);
int Bit2_get(Bit2_T bitmap, int col, int row);
void Bit2_map_col_major(Bit2_T bitmap,
                        void apply(int col,
                                   int row,
                                   Bit2_T bitmap,
                                   int bit,
                                   void *closure),
                        void *closure);
void Bit2_map_row_major(Bit2_T bitmap,
                        void apply(int col,
                                   int row,
                                   Bit2_T bitmap,
                                   int bit,
                                   void *closure),
                        void *closure);
void Bit2_free(Bit2_T *bitmap);

#endif