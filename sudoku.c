/*
 *     sudoku.c
 *     by nozden01 & bdioni01, 2/12/2024
 *     iii
 *
 *     Function implementations for the sudoku program.
 *     Handles arguments, reads integers in from the file, checks if the
 *     sudoku board from the file is valid, exits the program in success
 *     or failure.
 */

#include "sudoku.h"

const int DIM1 = 9;
const int DIM2 = 9;
const int ELEMENT_SIZE = 8;
const int MAX_VALUE = 9;
const int MIN_VALUE = 1;
const int BOARD_HEIGHT = 9;
const int BOARD_WIDTH = 9;

/********** main ********
 *
 * Use: 
 *      Runs the sudoku program, returning EXIT_SUCCESS if the board given in
 *      is a solved sudoku puzzle, or EXIT_FAILURE otherwise.
 * Parameters:
 *      int argc:     The number of arguments on the command line.
 *      char *argv[]: Pointer to an array of arguments from the command line.
 * Return:
 *      EXIT_SUCCESS if the board given in is a solved sudoku puzzle, or
 *      EXIT_FAILURE otherwise.
 * Expects:
 *      Less than 3 arguments on the command line (throws a CRE if not).
 * Notes: 
 *      After main is run, the user knows whether or not the sudoku board 
 *      they passed in is a valid board or not.
 *
 ************************/
int main(int argc, char *argv[])
{
        FILE *fp;
        assert(argc < 3);

        UArray2_T sudoku_board = UArray2_new(DIM1, DIM2, ELEMENT_SIZE);

        /* Opens the file and calls necessary functions */
        if (argc == 1) {
                fp = stdin;
                read_and_set(fp, sudoku_board);
                bool validBoard = check_sudoku(sudoku_board);
                if (!validBoard) {
                        return EXIT_FAILURE;
                }
        } else {
                fp = fopen(argv[1], "r");
                assert(fp != NULL);
                read_and_set(fp, sudoku_board);
                bool validBoard = check_sudoku(sudoku_board);
                fclose(fp);
                if (!validBoard) {
                        return EXIT_FAILURE;
                }
        }
        return EXIT_SUCCESS;
}

/********** check_sudoku ********
 *
 * Use: 
 *      This takes in a 2D UArray representing the sudoku board, and runs all 
 *      the necessary checking functions to check if the given sudoku board is
 *      valid or not. 
 * Parameters:
 *      UArray2_T sudoku: A 2D UArray that holds the sudoku board to be checked.
 * Return: 
 *      True if the sudoku board is valid, false otherwise.
 * Expects: 
 *      That the allocated sudoku_check struct is not NULL 
 *      (throws a CRE if not).
 * Notes: 
 *      All memory allocation is contained within this function. The 
 *      deallocation of the closure is dealt with inside this function as well
 *      as check_if_valid, and all other deallocation (for sequences and 
 *      uarray2) is handled in the free_helper function. 
 *
 ************************/
bool check_sudoku(UArray2_T sudoku) 
{       
        Check sudoku_check;
        NEW(sudoku_check);
        /* Checking for a successful memory allocation. */
        assert(sudoku_check != NULL);
        sudoku_check->OK = true;

        sudoku_check->line = Seq_new(9);
        sudoku_check->box1 = Seq_new(9);
        sudoku_check->box2 = Seq_new(9);
        sudoku_check->box3 = Seq_new(9);

        UArray2_map_col_major(sudoku, row_col_check, sudoku_check);
        if (!check_if_valid(sudoku, sudoku_check)) {
                return false;
        } 
        UArray2_map_row_major(sudoku, row_col_check, sudoku_check);
        if (!check_if_valid(sudoku, sudoku_check)) {
                return false;
        }
        UArray2_map_row_major(sudoku, box_check, sudoku_check);
        if (!check_if_valid(sudoku, sudoku_check)) {
                return false;
        }

        free_helper(sudoku_check->box1, sudoku_check->box2, 
                    sudoku_check->box3, sudoku_check->line, sudoku);
        free(sudoku_check);
        return true;
}

/********** check_if_valid ********
 *
 * Use: 
 *      This function checks the OK value of the closure that indicates whether
 *      or not a row, column, or box in the sudoku board is valid. Additionally,
 *      this function deals with freeing all memory allocated from the
 *      sequences, uarray, and closure.
 * Parameters:
 *      UArray2_T sudoku:   A 2D Uarray that holds the sudoku board to be 
 *                          checked.
 *      Check sudoku_check: A struct that carries information to determine
 *                          the validity of the row, column, or box of the
 *                          sudoku board.
 * Return: 
 *      True if the OK is true, false otherwise. In other words, return true
 *      if the rows/columns/boxes are valid, false otherwise.
 * Expects: 
 *      That sudoku is not NULL (throws CRE if not).
 *      That sudoku_check is not NULL (throws CRE if not).
 * Notes:
 *      The deallocation of the closure is dealt with inside this function, 
 *      and all other deallocation (for sequences and uarray2) is handled in
 *      the freeHelper function. 
 *
 ************************/
bool check_if_valid(UArray2_T sudoku, Check sudoku_check)
{
        assert(sudoku != NULL);
        assert(sudoku_check != NULL);
        if (!sudoku_check->OK) {
                free_helper(sudoku_check->box1, sudoku_check->box2, 
                sudoku_check->box3, sudoku_check->line, sudoku);
                free(sudoku_check);
                return false;
        }
        return true;
}

/********** read_and_set ********
 *
 * Use: 
 *      Reads and sets the sudoku board from the file passed in. Exits the 
 *      program with EXIT_FAILURE if a correct input was given, but it does not
 *      fall under the correct sudoku board criteria. 
 * Parameters:
 *      FILE *inputfd:    A pointer to the input file that holds the sudoku 
 *                        board to be read in.
 *      UArray2_T sudoku: A 2D array that will hold the values of the sudoku
 *                        board passed in.
 * Return: 
 *      None.
 * Expects: 
 *      sudoku is not NULL (throws CRE if not).
 *      The Pnmrdr is able to read in from the given file properly (throws CRE
 *      if not).
 *      p2HeaderInfo.height == BOARD_HEIGHT and p2HeaderInfo.width ==
 *      BOARD_WIDTH (returns EXIT_FAILURE if not).
 *      Every number read in is between MIN_VALUE and MAX_VALUE.
 * Notes:
 *      This function uses the Pnmrdr functions to allow the program to read
 *      in the given pgm file. After this function is run, the sudoku board
 *      passed in from the file should be stored in the passed in 2D array.
 *      All memory allocation and deallocation is handled in this function. 
 *
 ************************/
void read_and_set(FILE *inputfd, UArray2_T sudoku)
{
        assert(sudoku != NULL);
        /* Get all the information. */
        Pnmrdr_T p2 = Pnmrdr_new(inputfd);
        assert(p2 != NULL);
        Pnmrdr_mapdata p2HeaderInfo = Pnmrdr_data(p2);

        /* Check for a is valid board, exiting program if it is not a 9x9. */
        if (((int) p2HeaderInfo.height != BOARD_HEIGHT) ||
            ((int) p2HeaderInfo.width != BOARD_WIDTH)) {
                free_and_fail(sudoku, p2, inputfd);
        }

        /* Check for a correct denominator, exiting program if it is not 9. */
        if ((int) p2HeaderInfo.denominator != MAX_VALUE) {
                free_and_fail(sudoku, p2, inputfd);
        }

        /* Loop through and set all the information into the 2D Uarray. */
        for (int i = 0; i < BOARD_HEIGHT; i++) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                        int num = Pnmrdr_get(p2);
                        /* Exit program if any digit in the board is greater
                        than 9 or less than 1. */
                        if (num < MIN_VALUE || num > MAX_VALUE) {
                                get_rest_pixels(j, i, p2);
                                free_and_fail(sudoku, p2, inputfd);
                        }
                        int *ptr = (int *)UArray2_at(sudoku, j, i);
                        *ptr = num;
                }
        }
        Pnmrdr_free(&p2);
}

/********** get_rest_pixels ******** 
 *
 * Use: 
 *      Gets the rest of the pixels when we read in a number that is not
 *      between 1 and 9 in order to be able to free Pnmrdr properly.
 * Parameters:
 *      int col:     An integer representing the current column of the sudoku 
 *                   board.
 *      int row:     An integer representing the current row of the sudoku 
 *                   board.
 *      Pnmrdr_T p2: Pnmrdr to be freed.
 * Return: 
 *      None.
 * Expects: 
 *      None.
 * Notes: 
 *      None.
 *
 ************************/
void get_rest_pixels(int col, int row, Pnmrdr_T p2)
{
        for (int i = col + 1; i < BOARD_WIDTH; i++) {
                int num = Pnmrdr_get(p2);        
                (void) num;
        }
        for (int i = row + 1; i < BOARD_HEIGHT; i++) {
                for (int j = 0; j < BOARD_WIDTH; j++) {
                        int num = Pnmrdr_get(p2);
                        (void) num;
                }
        }
}

/********** box_check ******** 
 *
 * Use: 
 *      Apply function that helps checks if a box in a sudoku board is valid.
 * Parameters:
 *      int col:       An integer representing the current column of the sudoku 
 *                     board.
 *      int row:       An integer representing the current row of the sudoku 
 *                     board.
 *      UArray2_T arr: A 2D UArray representing the sudoku board that is being
 *                     checked.
 *      void *elem:    The element at the current [row, column] in the sudoku 
 *                     board.
 *      void *closure: Void pointer to a closure that we expect to be of Check
 *                     struct type. This closure holds the boolean determining
 *                     if the boxes are valid along with the sequences holding
 *                     the contents of the boxes.
 * Return: 
 *      None.
 * Expects: 
 *      That the closure passed in is a Check object.
 *      That elem is between MIN_VALUE and MAX_VALUE, exiting the program with 
 *      EXIT_FAILURE if not.
 *      That col > 0 and col < WIDTH, exiting the program with EXIT_FAILURE if 
 *      not.
 * Notes: 
 *      None.
 *
 ************************/
void box_check(int col, int row, UArray2_T arr, void *elem, void *closure) 
{
        if (*(int *)elem < MIN_VALUE || *(int *)elem > MAX_VALUE) {
                exit(1);
        }
        if (col > BOARD_WIDTH || col < 0) {
                exit(1);
        }
        
        (void) row;
        (void) arr;
        Check sudoku_check = closure;

        /* Add element to respective row sequence that represents a box. */
        if (col/3 == 0) {
                Seq_addhi(sudoku_check->box1, elem);
        } else if (col/3 == 1) {
                Seq_addhi(sudoku_check->box2, elem);
        } else {
                Seq_addhi(sudoku_check->box3, elem);
        }

        /* When a row of boxes has been added to all the sequences, check 
        validity. */
        if (Seq_length(sudoku_check->box3) == 9) {
                sudoku_check->OK = check_helper(sudoku_check->box3, 
                                                sudoku_check->OK);
                sudoku_check->OK = check_helper(sudoku_check->box2, 
                                                sudoku_check->OK);
                sudoku_check->OK = check_helper(sudoku_check->box1, 
                                                sudoku_check->OK);
        }
}

/********** check_helper ******** 
 *
 * Use:
 *      A helper function for the two apply functions in this program. Copies
 *      over all elements in a sequence holding a row, column, or box in the
 *      sudoku board into an array, sorts it, and then checks that all the
 *      numbers 1 - 9 are correctly in the array. 
 * Parameters:
 *      bool OK:   A boolean representing whether or not the rows, columns, or
 *                 boxes are valid or not.
 *      Seq_T seq: A sequence representing the contents of a row, column, or
 *                 box in the sudoku board.
 * Return: 
 *      False if the row, column, or box in the sudoku board is not valid, 
 *      or the value of OK that was already passed in. 
 * Expects: 
 *      That seq != NULL, exiting the program with EXIT_FAILURE if 
 *      not.
 *      That Seq_length(seq) == 9, exiting the program with EXIT_FAILURE if 
 *      not.
 * Notes: 
 *      None.
 *
 ************************/
bool check_helper(Seq_T seq, bool OK) {
        /* Copies over all elements in the sequence into an array */
        if (seq == NULL && Seq_length(seq) != 9) {
                exit(1);
        }

        int arrToCheck[9];
        for (int i = 0; i < 9; i++) {
                int *num = Seq_remhi(seq);
                arrToCheck[i] = *num;
        }

        qsort(arrToCheck, 9, sizeof(int), compare);
        for (int i = 0; i < 9; i++) {
                if (arrToCheck[i] != i + 1) {
                        return false;
                }
        }

        return OK;
}

/********** row_col_check ******** 
 *
 * Use: 
 *      This is the apply function that checks the validity of the sudoku
 *      board. When the sequence that represents the row, column, or box in the
 *      sudoku board has a length on 9, it goes in and checks that the row,
 *      column, or box is valid.
 * Parameters:
 *      int col:       An integer representing the current column.
 *      int row:       An integer representing the current row.
 *      UArray2_T arr: The 2D Uarray from which we are checking the element
 *      void *elem:    A void pointer pointing to an element in the 2D array.
 *      void *closure: Void pointer to a closure that we expect to be of Check
 *                     struct type. This closure holds the boolean determining
 *                     if the boxes are valid along with the sequences holding
 *                     the contents of the rows and columns.
 * Return:
 *      None.
 * Expects:
 *      That elem is between MIN_VALUE and MAX_VALUE, exiting the program with 
 *      EXIT_FAILURE if not.
 * Notes:
 *      None.
 *
 ************************/
void row_col_check(int col, int row, UArray2_T arr, void *elem, void *closure) 
{
        if (*(int *)elem < MIN_VALUE || *(int *)elem > MAX_VALUE) {
                exit(1);
        }
        
        (void) col;
        (void) row;
        (void) arr;
        
        Check sudoku_check = closure;
        Seq_addhi(sudoku_check->line, elem);
        if (Seq_length(sudoku_check->line) == 9) {
                sudoku_check->OK = check_helper(sudoku_check->line, 
                                                sudoku_check->OK);
        }
}

/********** compare ******** 
 *
 * Use: 
 *      Compares the two elements passed into the function, determining whether
 *      or not one element is greater than the other.
 * Parameters:
 *      const void *num1: A void pointer to the first number.
 *      const void *num2: A void pointer to the second number.
 * Return: 
 *      1 if the first number passed in is greater than the second, -1 if the
 *      first number passed in is less than the second, and 0 otherwise, 
 *      meaning the numbers are equal.
 * Expects: 
 *      None
 * Notes: 
 *      This compare function is necessary for the qsort function used in the
 *      apply functions.
 *
 ************************/
int compare(const void *num1, const void *num2) {
        int a = *(int*) num1;  
        int b = *(int*) num2;  
        if (a > b) {
                return 1;
        } else if (a < b) {
                return -1;
        } else {
                return 0;
        }
}

/********** free_helper ******** 
 *
 * Use: 
 *      A helper function that frees all the sequences and the 2D Uarray being
 *      used in the program.
 * Parameters:
 *      Seq_T box1:             A sequence representing the first box in the 
 *                              board in a row.
 *      Seq_T box2:             A sequence representing the second box in the
 *                              board in a row.
 *      Seq_T box3:             A sequence representing the third box in the
 *                              board in a row.
 *      Seq_T line:             A sequence representing a row or column in the
 *                              board.
 *      UArray2_T sudoku_board: A 2D Uarray representing the sudoku board.
 * Return:
 *     None.
 * Expects:
 *      box1, box2, box3, line, and sudoku_board should not be NULL
 * Notes:
 *     This function deals with the deallocation of all sequences and Uarray 
 *     made in the program.
 *
 ************************/
void free_helper(Seq_T box1, Seq_T box2, Seq_T box3, Seq_T line, UArray2_T 
                sudoku_board)
{
        if (box1 != NULL) {
                Seq_free(&(box1));
        }
        if (box2 != NULL) {
                Seq_free(&(box2));
        }
        if (box3 != NULL) {
                Seq_free(&(box3));
        }
        if (line != NULL) {
                Seq_free(&(line));
        }
        if (sudoku_board != NULL) {
                UArray2_free(&sudoku_board);
        }
}

/********** free_and_fail ********
 *
 * Use: 
 *      Frees the memory associated with the given sudoku board and pnmrdr
 *      and closes the given input file in the case that the program fails.
 * Parameters:
 *      UArray2_T sudoku: Sudoku board to be freed.
 *      Pnmrdr_T p2:      Pnmrdr to be freed.
 *      FILE *inputfd:    Pointer to the input file that will be closed.
 * Return: 
 *      None.
 * Expects: 
 *      That sudoku is not NULL.
 *      That p2 is not NULL.
 * Notes: 
 *      None.
 *
 ************************/
void free_and_fail(UArray2_T sudoku, Pnmrdr_T p2, FILE *inputfd)
{
        if (sudoku != NULL) {
                UArray2_free(&sudoku);
        }
        Pnmrdr_free(&p2);
        fclose(inputfd);
        exit(1);
}