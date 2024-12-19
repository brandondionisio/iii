/*
 *     sudoku.h
 *     by nozden01 & bdioni01, 2/12/2024
 *     iii
 *
 *     Contains struct and function declarations for the sudoku program.
 *     Includes libraries and files necessary for this program to function.
 */

#include <stdbool.h>
#include <pnmrdr.h>
#include <stdio.h>
#include <stdlib.h>
#include "uarray2.h"
#include "seq.h"

typedef struct Check {
        Seq_T line;
        bool OK;
        Seq_T box1;
        Seq_T box2;
        Seq_T box3;
} *Check;

bool check_sudoku(UArray2_T sudoku);
void read_and_set(FILE *inputfd, UArray2_T sudoku);
void row_col_check(int col, int row, UArray2_T arr, void *elem, void *closure);
int compare(const void *num1, const void *num2);
void box_check(int col, int row, UArray2_T arr, void *elem, void *closure);
bool check_helper(Seq_T seq, bool OK);
void free_helper(Seq_T box1,
                 Seq_T box2,
                 Seq_T box3,
                 Seq_T line,
                 UArray2_T sudoku_board);
bool check_if_valid(UArray2_T sudoku, Check sudoku_check);
void free_and_fail(UArray2_T sudoku, Pnmrdr_T p2, FILE *inputfd);
void get_rest_pixels(int col, int row, Pnmrdr_T p2);