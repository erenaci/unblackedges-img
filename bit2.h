/*
 * bit2.h
 * Andrea Foo (afoo01) and Erena Inoue(einoue01)
 * Comp40, HW2 iii
 * Due: February 10, 2020
 * Purpose: Interface file for the bit2 class
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <bit.h>

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

struct Bit2_T;
typedef struct Bit2_T *Bit2_T;

/* Bit2_T Bit2_new(int height, int width, int size)
 * Function: Creates a new 2D bit array with height and width. Makes sure that
 *					 the height and width is larger than 0. if not, CRE. The 2D bit2
 *					 array is really simply a 1D bit array in the actual implementation
 * Input: int width, int height
 * Output: A pointer to the newly created Bit 2D array
 */
Bit2_T Bit2_new(int width, int height);

/* int Bit2_width(Bit2_T array_2D)
 * Function: Finds the width of 2D Bit array. If pointer to array is null,
 * there will be a CRE.
 * Input: Bit2_T array_2D
 * Output: Width of 2D Bit array
 */
int Bit2_width(Bit2_T array2D);

/* int Bit2_height(Bit2_T array_2D)
 * Function: Finds the height of 2D Bit array. If pointer to array is null,
 * there will be a CRE.
 * Input: Bit2_T array_2D
 * Output: Height of 2D Bit array
 */
int Bit2_height(Bit2_T array2D);

/* int Bit2_put(Bit2_T array_2D, int x, int y, int marker)
 * Function: Find and returns the previous value of the specified bit.  If
 * pointer to array is null, there will be a CRE.
 * Input: Bit2_T array_2D, int x, int y, int marker
 * Output: returns the previous value of the specified bit
 */
int Bit2_put(Bit2_T array_2D, int x, int y, int marker);

/* int Bit2_get(Bit2_T array_2D, int x, int y)
 * Function: Returns the element at the specified location (x, y). If
 * pointer to array is null or x and y are out of bounds, there will be
 * a CRE.
 * Input: Bit2_T array_2D, int x, int y
 * Output: Returns the bit at the specified location
 */
int Bit2_get(Bit2_T array_2D, int x, int y);

/* void Bit2_map_col_major(Bit2_T array_2D, void apply(int x, int y,
 *                  Bit2_T array_2D, int curr, void *cl), void *cl)
 * Function: Uses the apply function to manipulate the bits in 2D array
 *           based on the column. Will raise CRE if pointer to array is null.
 * Input: Bit2_T array_2D, void apply(int x, int y, Bit2_T array_2D,
 *                                    int curr, void *cl), void *cl
 * Output: Nothing
 */
void Bit2_map_col_major(Bit2_T array_2D, void apply(int x, int y,
                      Bit2_T array_2D, int curr, void* cl), void *cl);


/* void Bit2_map_row_major(Bit2_T array_2D, void apply(int x, int y,
 *                  Bit2_T array_2D, int curr, void *cl), void *cl)
 * Function: Uses the apply function to manipulate the bits in 2D array
 *           based on the row. Will raise CRE if pointer to array is null.
 * Input: Bit2_T array_2D, void apply(int x, int y, Bit2_T array_2D,
 *                                    int curr, void *cl), void *cl
 * Output: Nothing
 */
void Bit2_map_row_major(Bit2_T array_2D, void apply(int x, int y,
                        Bit2_T array_2D, int curr, void* cl), void *cl);

/* Bit2_free(T *array)
 * Function: frees the memory of 2D bit array and pointer to the array. Will
 *					raise a CRE if pointer to the array and its pointer is null.
 * Parameters: Bit2_T *array
 * Returns: No returns
 */
void Bit2_free(Bit2_T *array2D);

#endif
