/*
 *     unblackedges.c
 *     by nozden01 & bdioni01, 2/12/2024
 *     iii
 *
 *     Function implementations for the unblackedges program.
 *     Handles arguments, reads bits in from the file, unblacks the black
 *     edge pixels, outputs the new pbm file.
 */

#include "unblackedges.h"

/*************** main ***************
 *
 * Use:
 *      Main driver of the program, handles arguments, determines program's
 *      success.
 * Return:
 *      0 if executed to completion -- non-zero integer otherwise.
 * Parameters:
 *      int argc:     Number of arguments in program call.
 *      char *argv[]: Pointer to an array of arguments.
 * Expects:
 *      Two arguments or less.
 *      File that exists.
 * Notes:
 *      Will throw a CRE if more than 2 arguments are provided
 *      Will throw a CRE if the given file is NULL.
 *      Will return EXIT_SUCCESS if the program runs to completion
 *
 ************************/
int main(int argc, char *argv[])
{
        FILE *fp;
        /* Asserting that there are not too many args. */
        assert(argc < 3);
        
        /* Opens the file and calls necessary functions. */
        if (argc == 1) {
                fp = stdin;
        } else {
                fp = fopen(argv[1], "r");
                assert(fp != NULL);
        }
        Bit2_T bitmap = pbmread(fp);
        pbmwrite(bitmap);

        fclose(fp);

        return EXIT_SUCCESS;
}

/*************** pbmwrite ***************
 *
 * Use:
 *      Reads from the given input file and returns the bitmap that it reads
 *      in.
 * Return:
 *      A 2-D bitmap.
 * Parameters:
 *      FILE *inputfp: input file from which we read in the bitmap.
 * Expects:
 *      Inputfp is a portable bitmap file.
 *      The Pnmrdr is able to read in from the given file properly.
 *      The P1 header read in from inputfp has a height greater than 0 and
 *      and a width greater than 0.
 * Notes:
 *      Throws a CRE if the Pnmrdr is not allocated correctly.
 *      All memory allocation and deallocation is contained within this 
 *      function.
 *
 ************************/
Bit2_T pbmread(FILE *inputfp) 
{
        /* Getting all the information from the P1 file. */
        Pnmrdr_T p1 = Pnmrdr_new(inputfp);
        assert(p1 != NULL);
        Pnmrdr_mapdata p1HeaderInfo = Pnmrdr_data(p1);

        assert((p1HeaderInfo.height > 0) && (p1HeaderInfo.width > 0));

        Bit2_T ourBitmap = Bit2_new(p1HeaderInfo.width, p1HeaderInfo.height);

        for (unsigned int i = 0; i < p1HeaderInfo.height; i++) {
                for (unsigned int j = 0; j < p1HeaderInfo.width; j++) {
                        int bit = Pnmrdr_get(p1);
                        int num = Bit2_put(ourBitmap, j, i, bit);
                        (void) num;
                }
        }
        Pnmrdr_free(&p1);

        return ourBitmap;
}

/************** pbmwrite *****************
 *
 * Use:
 *      With a given bitmap, creates a new stack, unblacks all black edges,
 *      and prints out the PBM file contents to stdout. Frees all memory
 *      associated with the stack and the given bitmap.
 * Return:
 *      None.
 * Parameters:
 *      Bit2_T bitmap: 2-D bitmap which we read in from the input
 *                     file/stream.
 * Expects:
 *      Bitmap to be nonempty.
 * Notes:
 *      Main runner of the unblackedges program.
 *      Relies heavily on the call of other functions in this file.
 *      All memory allocation and deallocation is contained within this 
 *      function.
 *
 ************************/
void pbmwrite(Bit2_T bitmap)
{
        Stack_T neighbors = Stack_new();
        Bit2_map_row_major(bitmap, check_pixels, neighbors);
        printf("P1\n%d %d\n", Bit2_width(bitmap), Bit2_height(bitmap));
        Bit2_map_row_major(bitmap, print_bitmap, NULL);
        Stack_free(&neighbors);
        Bit2_free(&bitmap);
}

/************** check_pixels *****************
 *
 * Use:
 *      Apply function for unblacking all the black edges associated with
 *      a given bitmap. Given information on a certain bit in the given
 *      bitmap, checks if that pixel is a black edge and uses a stack to
 *      unblack all adjacent black pixels.
 * Return:
 *      None.
 * Parameters:
 *      int col:       Integer of the column index of a bit.
 *      int row:       Integer of a row index of a bit.
 *      Bit2_T bitmap: 2-D bitmap which was read in from the input file.
 *      int bit:       Integer representing the value of a bit.
 *      void *stack:   Void pointer of a closure which expects a stack.
 *                     that will hold the neighbors to be checked.
 * Expects:
 *      Closure is a Hanson stack.
 *      [0 < col < bitmap width).
 *      [0 < row < bitmap height).
 * Notes:
 *      All memory allocation and deallocation is contained within this 
 *      function.
 *
 ************************/
void check_pixels(int col, int row, Bit2_T bitmap, int bit, void *stack)
{
        Stack_T neighbor_stack = stack;
        if ((bit == 1) && (col == 0 ||
                           row == 0 ||
                           row == Bit2_height(bitmap) - 1 ||
                           col == Bit2_width(bitmap) - 1)) {
                
                /* Change pixel to white. */
                int num = Bit2_put(bitmap, col, row, 0);
                (void) num;

                /* Add neighbors to stack. */
                push_neighbors(col, row, bitmap, neighbor_stack);

                while (!Stack_empty(neighbor_stack)) {
                        Index topbit = (Index)Stack_pop(neighbor_stack);
                        Bit2_put(bitmap, topbit->col, topbit->row, 0);
                        push_neighbors(topbit->col, topbit->row,
                                       bitmap, neighbor_stack);
                        free(topbit);
                }
        }
}

/************** push_neighbors ***************
 *
 * Use:
 *      Given a column and row index of a bit from the given bitmap, pushes
 *      all the possible neighbors of that bit onto the given stack. The
 *      neighbors are represented as pointers to structs of column and row
 *      integers and are allocated in this function using malloc.
 * Return:
 *      None.
 * Parameters:
 *      int col:                Integer of the column index of a bit.
 *      int row:                Integer of a row index of a bit.
 *      Bit2_T bitmap:          2-D bitmap that was read in from the input file.
 *      Stack_T neighbor_stack: Void pointer of a closure which expects a stack
 *                              that will hold the neighbors to be checked.
 * Expects:
 *      [0 < col < bitmap width)
 *      [0 < row < bitmap height)
 * Notes:
 *      May CRE if malloc of bit index structs fail.
 *      All memory allocation and deallocation is contained within this 
 *      function.
 *
 ************************/
void push_neighbors(int col, int row, Bit2_T bitmap, Stack_T neighbor_stack)
{
        if ((row != 0) && (Bit2_get(bitmap, col, row - 1) == 1)) {
                /* Add the top neighbor. */
                push_neighbors_helper(col, row - 1, neighbor_stack);
        }
        if ((col != Bit2_width(bitmap) - 1) &&
            (Bit2_get(bitmap, col + 1, row) == 1)) {
                /* Add the right neighbor. */
                push_neighbors_helper(col + 1, row, neighbor_stack);
        }
        if ((row != Bit2_height(bitmap) - 1) &&
            (Bit2_get(bitmap, col, row + 1) == 1)) {
                /* Add the bottom neighbor. */
                push_neighbors_helper(col, row + 1, neighbor_stack);
        }
        if ((col != 0) && (Bit2_get(bitmap, col - 1, row) == 1)) {
                /* Add the left neighbor. */
                push_neighbors_helper(col - 1, row, neighbor_stack);
        }
}

/************** push_neighbors_helper ***************
 *
 * Use:
 *      Given a column and row index of a neighbor bit from the given bitmap,
 *      pushes the struct containing the column and row index of the bit onto
 *      the given stack.
 * Return:
 *      None.
 * Parameters:
 *      int col:                Integer of the column index of a bit.
 *      int row:                Integer of a row index of a bit.
 *      Bit2_T bitmap:          2-D bitmap that was read in from the input file.
 *      Stack_T neighbor_stack: Void pointer of a closure which expects a stack
 *                              that will hold the neighbors to be checked.
 * Expects:
 *      Neighbor_stack is not NULL (throws a CRE if not).
 *      [0 < col < bitmap width)
 *      [0 < row < bitmap height)
 *      The bit at the given indices is an edge black pixel.
 * Notes:
 *      May throw a CRE if malloc of bit index structs fail.
 *
 ************************/
void push_neighbors_helper(int col, int row, Stack_T neighbor_stack)
{
        assert(neighbor_stack != NULL);
        Index bitindex = malloc(sizeof(*bitindex));
        assert (bitindex != NULL);
        bitindex->col = col;
        bitindex->row = row;
        Stack_push(neighbor_stack, bitindex);
}

/************** print_bitmap *****************
 *
 * Use:
 *      Apply function for printing the contents of a given bitmap in row major
 *      order. Prints the given bit and creates a new line if the given column
 *      index of the bit is equal to the width of the bitmap.
 * Return:
 *      None.
 * Parameters:
 *      int col:        Integer of the column index of a bit.
 *      int row:        Integer of a row index of a bit (not used).
 *      Bit2_T bitmap:  2-D bitmap which was read in from the input file.
 *      int bit:        Integer representing the value of a bit.
 *      void *closure:  Closure argument for the apply function to
 *                      work (not used).
 * Expects:
 *      Bitmap is not NULL (Bit2_width throws a CRE if not).
 * Notes:
 *      Bitmap being NULL is handled by the Bit2_width function.
 *
 ************************/
void print_bitmap(int col, int row, Bit2_T bitmap, int bit, void *closure)
{
        (void) row;
        (void) closure;
        if (col != Bit2_width(bitmap) - 1) {
                printf("%d ", bit);
        } else {
                printf("%d\n", bit);
        }
}