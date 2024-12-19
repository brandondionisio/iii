/*
 *     unblackedges.h
 *     by nozden01 & bdioni01, 2/12/2024
 *     iii
 *
 *     Contains struct and function declarations for the unblackedges program.
 *     Includes libraries and files necessary for this program to function.
 */

#include "bit2.h"
#include <stdbool.h>
#include <pnmrdr.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

typedef struct Index {
        int col;
        int row;
} *Index;

Bit2_T pbmread(FILE *inputfp);
void pbmwrite(Bit2_T bitmap);
void check_pixels(int col, int row, Bit2_T bitmap, int bit, void *stack);
void push_neighbors(int col, int row, Bit2_T bitmap, Stack_T neighbor_stack);
void push_neighbors_helper(int col, int row, Stack_T neighbor_stack);
void print_bitmap(int col, int row, Bit2_T bitmap, int bit, void *closure);