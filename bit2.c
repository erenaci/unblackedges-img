/*
 * bit2.c
 * Andrea Foo (afoo01) and Erena Inoue(einoue01)
 * Comp40, HW2 iii
 * Due: February 10, 2020
 * Purpose: Implementation file for the bit2 class
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bit2.h"
#include <assert.h>
#include <bit.h>


/*
 * struct of Bit2_T that will hold the width, and height of the bit array and a
 * a pointer to the one dimensional bit array.
 */
struct Bit2_T {
  int width;
  int height;
  Bit_T array1D;
};

Bit2_T Bit2_new(int width, int height)
{
	Bit2_T new_2D_array;
  /*Make sure that the height and width is larger than 0, if not, CRE */
	assert((height > 0) && (width > 0));
	/*calculate length of the large 1D bit array to give the appearence as if
	 * it were a 2D array
	 */
	int length = height * width;
	/*create space for the unboxed array on the heap */
  new_2D_array = malloc(sizeof(*new_2D_array));
  assert(new_2D_array != NULL);

	/*create a new one dimension bit vector array*/
	Bit_T new_array_1D = Bit_new(length);
  new_2D_array -> width = width;
  new_2D_array -> height = height;
	new_2D_array -> array1D = new_array_1D;

	return new_2D_array;
}

int Bit2_width(Bit2_T array_2D)
{
  /* Makes sure that the pointer to the array is not NULL, if it is, CRE */
  assert(array_2D != NULL);
	return array_2D -> width;
}


int Bit2_height(Bit2_T array_2D)
{
  /* Makes sure that the pointer to the array is not NULL, if it is, CRE */
  assert(array_2D != NULL);
  return array_2D -> height;
}


int Bit2_get(Bit2_T array_2D, int x, int y)
{
  /* Makes sure that the pointer to the array is not NULL and x and y are within
   * bounds otherwise, CRE
	 */
	assert(array_2D != NULL);
	int array_width = array_2D -> width;
	int array_height = array_2D -> height;
  assert((x >= 0) && (x < array_width) && (y < array_height) && (y >= 0));
  int index = array_width * y + x; /* calculating index of 1D array */
  return Bit_get(array_2D -> array1D, index);
}


int Bit2_put(Bit2_T array_2D, int x, int y, int marker)
{
  /* Makes sure that the pointer to the array is not NULL and x and y are within
   bounds otherwise, CRE */
  assert(array_2D != NULL);
	int array_width = array_2D -> width;
	int array_height = array_2D -> height;
  assert((x >= 0) && (x < array_width) && (y < array_height) && (y >= 0));
  int index = array_width * y + x; /* calculating index of 1D array */
  return Bit_put(array_2D -> array1D, index, marker);
}


void Bit2_map_col_major(Bit2_T array_2D, void apply(int x, int y,
	     									Bit2_T array_2D, int curr, void *cl), void *cl)
{
  /* Makes sure that the pointer to the array is not NULL, if it is, CRE */
  assert(array_2D != NULL);
  int height = array_2D -> height;
  int width = array_2D -> width;
  /*loops through the entire 2D array column by column and calls function on
  * each element in the 2D array */
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
        apply(x, y, array_2D, Bit2_get(array_2D, x, y), &cl);
    }
  }
}


void Bit2_map_row_major(Bit2_T array_2D, void apply(int x, int y,
												Bit2_T array_2D, int curr, void *cl), void *cl)
{
  /* Makes sure that the pointer to the array is not NULL, if it is, CRE */
  assert(array_2D != NULL);
  int height = array_2D -> height;
  int width = array_2D -> width;
  /*loops through the entire 2D array row by row and calls function on
  * each element in the 2D array */
	for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        apply(x, y, array_2D, Bit2_get(array_2D, x, y), &cl);
    }
  }
}

void Bit2_free(Bit2_T *array_2D)
{
  /* Makes sure that the pointer array and its pointer are not NULL,
   if it is, CRE */
  assert(array_2D != NULL && *array_2D != NULL);
  /*frees the Uarray to prevent memory leaks */
	Bit_free(&((*array_2D) ->array1D));
  /*frees the pointer to the Uarray */
  free(*array_2D);
}
