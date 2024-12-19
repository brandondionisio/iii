/*
 *     uarray2.c
 *     by nozden01 & bdioni01, 2/12/2024
 *     iii
 *
 *     Function implementations for the 2D Uarray data structure.
 */

#include "uarray2.h"

/********** UArray2_map_col_major ********
 *
 * Use: 
 *      This function parses through the given 2D-array in column major order
 *      and calls the given apply function which is then executed to every 
 *      element in the array. The apply function takes in the row and column
 *      indices of an element of the 2D-array, a pointer to the element, a 
 *      reference to the 2D-array, and a void pointer closure. The closure
 *      argument is a void pointer that will be passed into the apply function.
 * Parameters:
 *      UArray2_T arr:                              A 2D Uarray that will be 
 *                                                  traversed.
 *      void apply(int col, int row, UArray2_T arr, 
 *                      void *elem, void *closure): A function that will be
 *                                                  applied to each element of 
 *                                                  the 2D Uarray.
 *      void *closure:                              The closure argument that
 *                                                  will be passed into the 
 *                                                  apply function.
 * Return: 
 *      None.
 * Expects: 
 *      That arr is not NULL, and throws CRE if arr is NULL.
 * Notes: 
 *      None.
 *
 ************************/
void UArray2_map_col_major(UArray2_T arr,
                           void apply(int col,
                                      int row, 
                                      UArray2_T arr,
                                      void *elem,
                                      void *closure),
                           void *closure)
{
    assert(arr != NULL);
    int height = UArray2_height(arr);
    int width = UArray2_width(arr);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int *elem = (int *)UArray2_at(arr, i, j);
            apply(i, j, arr, elem, closure);
        }
    }
}

/********** UArray2_map_row_major ********
 *
 * Use: 
 *      This function parses through the given 2D-array in row major order and
 *      calls the given apply function which is then executed to every element
 *      in the array. The apply function takes in the row and column indices of
 *      an element of the 2D-array, a pointer to the element, a reference to the
 *      2D-array, and avoid pointer closure. The closure argument is a void
 *      pointer that will be passed into the apply function.
 * Parameters:
 *      UArray2_T arr:                              A 2D Uarray that will be 
 *                                                  traversed.
 *      void apply(int col, int row, UArray2_T arr, 
 *                      void *elem, void *closure): A function that will be
 *                                                  applied to each element of 
 *                                                  the 2D Uarray.
 *      void *closure:                              The closure argument that
 *                                                  will be passed into the 
 *                                                  apply function.  
 * Return: 
 *      None.
 * Expects: 
 *      That arr is not NULL, and throws CRE if arr is NULL.
 * Notes: 
 *      None.
 *
 ************************/
void UArray2_map_row_major(UArray2_T arr,
                           void apply(int col,
                                      int row,
                                      UArray2_T arr,
                                      void *elem,
                                      void *closure),
                           void *closure)
{
    assert(arr != NULL);
    int height = UArray2_height(arr);
    int width = UArray2_width(arr);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int *elem = (int *)UArray2_at(arr, j, i);
            apply(j, i, arr, elem, closure);
        }
    }
}

/********** UArray2_at ********
 *
 * Use: 
 *      This function accesses an element in the given 2D array represented by
 *      the UArray2_T type. It returns a void pointer to the element at the
 *      given row and column indices within the array.
 * Parameters:
 *      UArray2_T arr: The 2D Uarray from which the element is being retrieved.
 *      int col:       An integer representing the wanted column.
 *      int row:       An integer representing the wanted row.
 * Return: 
 *      None.
 * Expects: 
 *      The row and column indices are in the bounds of the 2D array. So, 
 *      row must be in the range of [0, height of 2D array), and col must be
 *      in range of [0, width of 2D array). Additionally, arr must not be 
 *      NULL. Throws CRE if any of these cases are not met. 
 * Notes: 
 *      None.
 *
 ************************/
void *UArray2_at(UArray2_T arr, int col, int row) 
{
    assert(arr != NULL);
    assert((col >= 0) && (row >= 0));
    assert((UArray2_width(arr) > col) && (UArray2_height(arr) > row));

    UArray_T column = *(UArray_T *)UArray_at(arr->uarr2, row);
    return UArray_at(column, col);
}

/********** UArray2_new ********
 *
 * Use: 
 *      Creates an uninitialized 2D array with the given row and column
 *      dimensions. Allocates the memory for the 2D array corresponding to the
 *      given size integer. Returns a UArray2_T pointer to the new 2D array.
 * Parameters:
 *      int col:  An integer representing the wanted column.
 *      int row:  An integer representing the wanted row.
 *      int size: An integer representing the size in bytes of the elements in 
 *                the Uarray to be made.      
 * Return: 
 *      None.
 * Expects: 
 *      That row is greater than or equal to 0. That col is greater than or 
 *      equal to 0. That size is greater than or equal to 0. Throws CRE if 
 *      any of these cases are not met. 
 * Notes: 
 *      This function allocates memory for the new 2D UArray and expects the
 *      client to free the memory with UArray2_free.
 *
 ************************/
UArray2_T UArray2_new(int col, int row, int size)
{
    assert((col >= 0) && (row >= 0) && (size > 0));

    UArray2_T UArray2;
    NEW(UArray2);
    /* Checking for a successful memory allocation. */
    assert(UArray2 != NULL);

    UArray_T rows = UArray_new(row, size);
    for (int i = 0; i < row; i++) { 
        *(UArray_T *)(UArray_at(rows, i)) = UArray_new(col, sizeof(int));
    }
    UArray2->uarr2 = rows;
    return UArray2;
}

/********** UArray2_height ********
 *
 * Use: 
 *      Finds and returns the height (an integer) of the given 2D array.
 * Parameters:
 *      UArray2_T arr: 2D Uarray that we are getting the height from.    
 * Return: 
 *      None.
 * Expects: 
 *      That arr is not NULL, and throws CRE if arr is NULL.
 * Notes: 
 *      None.
 *
 ************************/
int UArray2_height(UArray2_T arr)
{
    assert(arr != NULL);
    return UArray_length(arr->uarr2);
}

/********** UArray2_width ********
 *
 * Use:
 *      Finds and returns the width (an integer) of the given 2D array.
 * Parameters:
 *      UArray2_T arr: 2D Uarray that we are getting the width from.    
 * Return: 
 *      None.
 * Expects: 
 *      That arr is not NULL, and throws CRE if arr is NULL.
 * Notes: 
 *      None.
 *
 ************************/
int UArray2_width(UArray2_T arr)
{
    assert(arr != NULL);
    return UArray_length(*(UArray_T *)UArray_at(arr->uarr2, 0));
}

/********** UArray2_size ********
 *
 * Use:
 *      This function finds and returns the size (size_t type) of the pointer to
 *       the given 2D array.
 * Parameters:
 *      UArray2_T arr: 2D Uarray that we are getting the size from.    
 * Return: 
 *      None.
 * Expects: 
 *      That arr is not NULL, and throws CRE if arr is NULL.
 * Notes: 
 *      None.
 *
 ************************/
int UArray2_size(UArray2_T arr)
{
    assert(arr != NULL);
    return UArray_size(arr->uarr2);
}

/********** UArray2_free ********
 *
 * Use: 
 *      Frees the memory associated with the given 2D array via a pass to the
 *      address of a pointer to the 2D array.
 * Parameters:
 *      UArray2_T *arr: A pointer to the 2D Uarray to be freed.
 * Return: 
 *      None.
 * Expects: 
 *      That arr is not NULL and *arr is not NULL, and throws CRE if any of
 *      these cases are not met. 
 * Notes: 
 *      This function deals with the deallocation of a Uarray2 object made in 
 *      passed in. 
 *
 ************************/
void UArray2_free(UArray2_T *arr)
{
    assert((arr != NULL) && (*arr != NULL));
    int length = UArray_length((*arr)->uarr2);
    for (int i = 0; i < length; i++) {
        UArray_free((UArray_T *)(UArray_at((*arr)->uarr2, i)));
    }

    UArray_free(&((*arr)->uarr2));
    FREE(*arr);
}