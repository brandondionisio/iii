/*
 *     bit2.c
 *     by nozden01 & bdioni01, 2/12/2024
 *     iii
 *
 *     Function implementations for the 2D bitmap interface.
 */

#include "bit2.h"

/************** Bit2_width ************
 *
 * Use:
 *      Returns the width of the given 2D bitmap.
 * Parameters:
 *      Bit2_T bitmap: Bitmap that we are getting the width from.
 * Return:
 *      Integer that represents the width of the bitmap.
 * Expects:
 *      That bitmap is not NULL (throws a CRE if not).
 *      The bitmap has been properly initialized.
 * Notes:
 *      None.
 *
 ************************/
int Bit2_width(Bit2_T bitmap) 
{    
        assert(bitmap != NULL);
        return bitmap->columns;
}

/************** Bit2_height ************
 *
 * Use:
 *      Returns the height of the given 2D bitmap.
 * Parameters:
 *      Bit2_T bitmap: Bitmap that we are getting the height from.
 * Return:
 *      Integer that represents the height of the bitmap.
 * Expects:
 *      That bitmap is not NULL (throws a CRE if not).
 *      The bitmap has been properly initialized.
 * Notes:
 *      None.
 * 
 ************************/
int Bit2_height(Bit2_T bitmap) 
{
        assert(bitmap != NULL);
        return bitmap->rows;
}

/************** Bit2_new ************
 *
 * Use:
 *      Allocates a new instance of a 2D bitmap with the dimensions of the
 *      given column and row integers.
 * Parameters:
 *      int col: Integer representing how many columns (width) the new 2D
 *               should have.
 *      int row: Integer representing how many rows (height) the new 2D
 *               should have.
 * Return:
 *      Pointer to the new 2D bitmap.
 * Expects:
 *      col > 0 (throws a CRE if not)
 *      row > 0 (throws a CRE if not)
 * Notes:
 *      This function allocates memory for the new 2D bitmap and expects the
 *      client to free the memory with Bit2_free.
 *
 ************************/
Bit2_T Bit2_new(int col, int row) 
{
        assert((col >= 0) && (row >= 0));
        Bit2_T Bit2;
        NEW(Bit2);
        /* Checking for successful memory allocation. */
        assert(Bit2 != NULL);

        Bit2->rows = row;
        Bit2->columns = col;
        Bit2->bitarr2 = Bit_new(row * col);
        return Bit2;
}

/************** Bit2_put ************
 *
 * Use:
 *      Puts the given bit into the given bitmap at the given indices. Returns
 *      the bit that has been replaced.
 * Parameters:
 *      Bit2_T bitmap: Bitmap that we are putting a new bit in.
 *      int col:       Integer representing the column index of the bit we are
 *                     putting in.
 *      int row:       Integer representing the row index of the bit we are
 *                     putting in.
 *      int bit:       Integer value of the bit that we are putting in.
 * Return:
 *      Integer representing the value of the replaced bit.
 * Expects:
 *      That bitmap is not NULL (throws a CRE if not).
 *      col >= 0 (throws a CRE if not).
 *      row >= 0 (throws a CRE if not).
 *      bit == 1 or bit == 0 (throws a CRE if not).
 *      That the bitmap has been properly initialized.
 * Notes:
 *      None.
 *
 ************************/
int Bit2_put(Bit2_T bitmap, int col, int row, int bit)
{
        assert(bitmap != NULL);
        assert((col >= 0) && (row >= 0));
        assert((bit == 1) || (bit == 0));
        return Bit_put(bitmap->bitarr2, (bitmap->columns) * (row) + col, bit);
}

/************** Bit2_get ************
 *
 * Use:
 *      Gets the bit value of the bit from the given bitmap with the given
 *      indices.
 * Parameters:
 *      Bit2_T bitmap: Bitmap from which we are getting the bit.
 *      int col:       Integer representing the column index of the bit we are
 *                     retrieving.
 *      int row:       Integer representing the row index of the bit we are
 *                     retrieving.
 * Return:
 *      Integer representing a bit value.
 * Expects:
 *      That bitmap is not NULL (throws a CRE if not).
 *      col >= 0 (throws a CRE if not).
 *      row >= 0 (throws a CRE if not).
 *      That the bitmap has been properly initialized.
 * Notes:
 *      None.
 *
 ************************/
int Bit2_get(Bit2_T bitmap, int col, int row) 
{
        assert(bitmap != NULL);
        assert((col >= 0) && (row >= 0));
        return Bit_get(bitmap->bitarr2, (bitmap->columns) * (row) + col);
}

/*********** bit2_map_col_major *********
 *
 * Use:
 *      This function parses through the given 2D bitmap in column major order
 *      and calls the given apply function on every element. The apply function
 *      takes in the row and column indices of a bit, a reference to a 2D
 *      bitmap, the value of the bit, and a void pointer closure. The closure
 *      argument is a void pointer that will be passed into the apply function.
 * Parameters:
 *      Bit2_T bitmap:             2D bitmap that will be traversed.
 *      void apply(int col,
 *                 int row,
 *                 Bit2_T bitmap,
 *                 int bit,        
 *                 void *closure): Function that will be called on each
 *                                 element of the 2D bitmap.
 *      void *closure:             Void pointer closure that will be passed
 *                                 into the apply function.
 * Return:
 *      None.
 * Expects:
 *      That bitmap is not NULL (throws a CRE if not).
 *      Bitmap is properly initialized.
 * Notes:
 *      None.
 *
 ************************/
void Bit2_map_col_major(Bit2_T bitmap,
                        void apply(int col,
                                   int row,
                                   Bit2_T bitmap,
                                   int bit,
                                   void *closure),
                        void *closure)
{
        assert(bitmap != NULL);
        for (int i = 0; i < bitmap->columns; i++) {
                for (int j = 0; j < bitmap->rows; j++) {
                        int bit = Bit2_get(bitmap, i, j);
                        apply(i, j, bitmap, bit, closure);
                }
        }
}

/*********** bit2_map_row_major *********
 *
 * Use:
 *      This function parses through the given 2D bitmap in row major order
 *      and calls the given apply function on every element. The apply function
 *      takes in the row and column indices of a bit, a reference to a 2D
 *      bitmap, the value of the bit, and a void pointer closure. The closure
 *      argument is a void pointer that will be passed into the apply function.
 * Parameters:
 *      Bit2_T bitmap:                 2D bitmap that will be traversed.
 *      void apply(int col,
 *                 int row,           
 *                 Bit2_T bitmap,      
 *                 int bit,
 *                 void *closure):     Function that will be called on each
 *                                     element of the 2D bitmap.
 *      void *closure:                 Void pointer closure that will be passed
 *                                     into the apply function.
 * Return:
 *      None.
 * Expects:
 *      That bitmap is not NULL (throws a CRE if not).
 *      Bitmap is properly initialized.
 * Notes:
 *      None.
 *
 ************************/
void Bit2_map_row_major(Bit2_T bitmap,
                        void apply(int col,
                                   int row,
                                   Bit2_T bitmap,
                                   int bit,
                                   void *closure),
                        void *closure)
{
        assert(bitmap != NULL);
        for (int i = 0; i < bitmap->rows; i++) {
                for (int j = 0; j < bitmap->columns; j++) {
                        int bit = Bit2_get(bitmap, j, i);
                        apply(j, i, bitmap, bit, closure);
                }
        }
}

/************** Bit2_free ************
 *
 * Use:
 *      Frees the memory associated with the given 2D bitmap via a pass to the
 *      address of a pointer to the 2D bitmap.
 * Parameters:
 *      Bit2_T *bitmap: 2D bitmap to be freed.
 * Return:
 *      None.
 * Expects:
 *      That bitmap is not NULL (throws a CRE if not).
 *      That the pointer to the bitmap is not NULL (throws a CRE if not).
 * Notes:
 *      None.
 *
 ************************/
void Bit2_free(Bit2_T *bitmap)
{
        assert((bitmap != NULL) && (*bitmap != NULL));
        Bit_free(&(*bitmap)->bitarr2);
        FREE(*bitmap);
}