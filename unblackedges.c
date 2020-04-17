/*
 * unblackedges.c
 * Andrea Foo (afoo01) and Erena Inoue (einoue01)
 * Comp40, HW2 iii
 * Due: February 10, 2020
 * Purpose: Implmentation file for the removing black edges using bit2
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <bit2.h>
#include <bit.h>
#include <pnmrdr.h>
#include <stack.h>

/* CONSTANTS for black and white pixels*/
const int BLACK_PIXEL = 1;
const int WHITE_PIXEL = 0;
const int STARTING_INDEX = 0;

/* struct that holds x coordinate and y coordinate of the pixel */
struct Location {
  int x;
  int y;
};
typedef struct Location Location;

/*FUNCTION PROTOTYPES */
Bit2_T pbmread(FILE *fp);
void pbmwrite(FILE *outputfp, Bit2_T bit_map);
void unblack_bitmap(Stack_T black_edge_pixels, Bit2_T bit_map);
void check_one_pixel(int x, int y, int curr, void *black_edge_pixels);
void write_to_file(FILE *outputfp, Bit2_T bit_map);
void check_neighbors(int x, int y, Bit2_T bit_map, Stack_T black_edge_pixels);
Location *create_new_Location(int x, int y);
void make_pixel_white(int x, int y, Bit2_T bit_map);
void check_all_edges(void *black_edge_pixels, Bit2_T bit_map);
void turn_neighbor_white(int x, int y, Bit2_T bit_map,
                          Stack_T black_edge_pixels);


/* int main(int argc, char *argv[])
 * Function: Main that will call all the necessary functions to unblack edges
 *           of the pbm image. Will exit with exit failure with a error
 *           statement. Will create a 2D bit map that will store the bit
 *           information of each pixel and then unblack the edges.
 * Input: int argc, char *argv[]
 * Output: None
 */
int main(int argc, char *argv[])
{
  if (argc == 1) {
      Bit2_T bit_map = pbmread(stdin);
      pbmwrite(stdout, bit_map);
      /* Checking to see if the pointer to be freed is NULL or not */
      assert(bit_map != NULL);
      Bit2_free(&bit_map);
  }
  else if (argc == 2) {
      FILE *fp = fopen(argv[1], "r");
      if (fp == NULL) {
          fprintf(stderr, "Could not open file %s for reading.\n", argv[1]);
          exit(EXIT_FAILURE);
      }
      Bit2_T bit_map = pbmread(fp);
      pbmwrite(stdout, bit_map);
      /* Checking to see if the pointer to be freed is NULL or not */
      assert(bit_map != NULL);
      Bit2_free(&bit_map);
  }
  else {
      fprintf(stderr, "ERROR: Too many files given.\n");
      exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}

/* Bit2_T pbmread(FILE *fp)
 * Function: Reads in the input file and covert it to 2D bit array. Will
 *           exit the program if the input file is not pbm.
 * Parameters: FILE *fp
 * Returns: The pointer to bit array
 */
Bit2_T pbmread(FILE *fp)
{
    Pnmrdr_T rdr;
    TRY
        rdr = Pnmrdr_new(fp);
    EXCEPT(Pnmrdr_Badformat);
        fprintf(stderr, "ERROR: Incorrect file format.\n");
        exit(EXIT_FAILURE);
    END_TRY;
    Pnmrdr_mapdata imgMap = Pnmrdr_data(rdr);
    Pnmrdr_maptype type = imgMap.type;
    if (type != Pnmrdr_bit){
        fprintf(stderr, "ERROR: Not Bit map\n");
        exit(EXIT_FAILURE);
    }
    /*Creates a new bit map given the height and width */
    Bit2_T bitArr = Bit2_new(imgMap.width, imgMap.height);

    /* loops through the pgm image to get the data of each pixel in the file */
    for(int y = 0; y < (int)imgMap.height; y++) {
      for(int x = 0; x < (int)imgMap.width; x++) {
         Bit2_put(bitArr, x, y, Pnmrdr_get(rdr));
      }
    }
    /* Checking to see if the pointer to be freed is NULL or not */
    assert(rdr != NULL);
    Pnmrdr_free(&rdr);
    fclose(fp);
    return bitArr;
}

/* void pbmwrite(FILE *outputfp, Bit2_T bit_map)
 * Function: calls all functions necessary to unblack edges by creating a
 *           stack, putting all the location of all the black pixels on the
 *           very edge of the pbm image into the stack and then calls
 *           unblack_bitmap to get rid of all the black pixel edges as well
 *           as well their neighbors.
 * Parameters: FILE *outputfp, Bit2_T bit_map
 * Returns: None
 */
void pbmwrite(FILE *outputfp, Bit2_T bit_map)
{
  assert(bit_map != NULL);
  /*create a stack to hold all the location of the black edge pixels */
  Stack_T black_edge_pixels = Stack_new();

  /* checks the very edges of the pbm image where x = 0 or x = width - 1 and
   * y = 0 or w = height - 1 and puts the location of all the black pixel
   * edges in the stack
   */
  check_all_edges(black_edge_pixels, bit_map);

  /* turns all of the black pixels in the stack to white and then checks
   * their neighbors and if the neighbors are black pixels, will push them onto
   * the stack and then turn those to white as well
   */
  unblack_bitmap(black_edge_pixels, bit_map);

  /*outputs the new bitmap to standard output */
  write_to_file(outputfp, bit_map);

  /* Checking to see if the pointer to be freed is NULL or not */
  assert(black_edge_pixels != NULL);

  /*frees the black edge pixel stack to prevent memory leaks */
  Stack_free(&black_edge_pixels);
}

/* void write_to_file(FILE *outputfp, Bit2_T bit_map)
 * Function: Writes the new bit map to standard output
 * Parameters: FILE *outputfp, Bit2_T bit_map
 * Returns: None
 */
void write_to_file(FILE *outputfp, Bit2_T bit_map)
{
  assert(bit_map != NULL);
  fprintf(outputfp, "P1\n");
  fprintf(outputfp, "%d %d\n", Bit2_width(bit_map), Bit2_height(bit_map));
  for (int y = 0; y < Bit2_height(bit_map); y++) {
    for (int x = 0; x < Bit2_width(bit_map); x++) {
      fprintf(outputfp, "%d", Bit2_get(bit_map, x, y));
    }
    fprintf(outputfp, "\n");
  }
  fclose(outputfp);
}

/* Location *create_new_Location(int x, int y)
 * Function: creates new pointer to Location struct on the heap to be added to
 *           black edge pixel stack later on. Will raise a CRE if memory could
 *            not be allocated properly.
 * Parameters: int x, int y
 * Returns: Location pointer
 */
Location *create_new_Location(int x, int y)
{
  /*create new location struct on the heap */
  Location *curr = malloc(sizeof(Location));
  assert(curr != NULL);
  curr->x = x;
  curr->y = y;
  return curr;
}

/* void check_one_pixel(int x, int y, int curr, void *black_edge_pixels)
 * Function: Checks one of the pixels at the given x and y location.
 *           If it is black, then it will be pushed onto the black edge pixel
 *           stack.
 * Parameters: int x, int y, int curr, void *black_edge_pixels
 * Returns: None
 */
void check_one_pixel(int x, int y, int curr, void *black_edge_pixels)
{
  /*check to make sure the stack is not null */
  assert(black_edge_pixels != NULL);
  if (curr == BLACK_PIXEL) {
    Location *curr = create_new_Location(x, y);
    Stack_push(black_edge_pixels, curr);
  }
}

/* void check_all_edges(void *black_edge_pixels, Bit2_T bit_map)
 * Function: Checks the four very edges (2 horizontal and 2 vertical) of the pbm
 *           image and pushes all the black pixels in the row or column onto
 *           the black edge pixels stack
 * Parameters: void *black_edge_pixels, Bit2_T bit_map
 * Returns: None
 */
void check_all_edges(void *black_edge_pixels, Bit2_T bit_map)
{
  assert((bit_map != NULL) && (black_edge_pixels != NULL));
  int width = Bit2_width(bit_map);
  int height = Bit2_height(bit_map);

  /* loops through the height and width to go through the 2 columns on the
   * very edge of the bit map and the 2 rows on the very edge of the bit map
   * and pushes all the black pixels onto the stack
   */
  for (int y = 0; y < height; y++) {
    check_one_pixel(STARTING_INDEX, y, Bit2_get(bit_map, STARTING_INDEX, y),
                    black_edge_pixels);
    check_one_pixel(width - 1, y, Bit2_get(bit_map, width - 1, y),
                    black_edge_pixels);
  }

  /*starts from 1 and ends 1 index before the end of the row to prevent
   * double counting
   */
  for (int x = 1; x < width - 1; x++) {
    check_one_pixel(x, STARTING_INDEX, Bit2_get(bit_map, x, STARTING_INDEX),
                    black_edge_pixels);
    check_one_pixel(x, height - 1, Bit2_get(bit_map, x, height - 1),
                    black_edge_pixels);
  }
}

/* void unblack_bitmap(Stack_T black_edge_pixels, Bit2_T bit_map)
 * Function: Holds a while loop that will keep looping until the black edge
 *           pixel is empty. Pops the top element location pointer from the
 *           stack and then turns the pixel at the given location into
 *           a white pixle in the bit map. Then, it checks the neighbors
 *           of that pixel to see if those pixels are black as well.
 * Parameters: Stack_T black_edge_pixels, Bit2_T bit_map
 * Returns: None
 */
void unblack_bitmap(Stack_T black_edge_pixels, Bit2_T bit_map)
{
  /* CRE if stack or bit map is null */
  assert((bit_map != NULL) && (black_edge_pixels != NULL));
  /* will keep looping until stack is empty */
  while (Stack_empty(black_edge_pixels) != 1) {
    Location * curr = (Location *)Stack_pop(black_edge_pixels);

    /*turns the pixel at given location into a white pixel */
    make_pixel_white(curr->x, curr->y,bit_map);

    /*check neighbors of curr location to see if those are black pixels */
    check_neighbors(curr->x, curr->y, bit_map, black_edge_pixels);

    /* Checking to see if the pointer to be freed is NULL or not */
    assert(curr != NULL);
    /*frees current location pointer to prevent memory leaks */
    free(curr);
  }
}

/* void check_neighbors(int x, int y, Bit2_T bit_map, Stack_T black_edge_pixels)
 * Function: checks neighbors of current location to see if those are black
 *            pixels. If they are, a new location struct will be created
 *            and pushed onto the stack and then that pixel will be turned
 *            white.
 * Parameters: int x, int y, Bit2_T bit_map, Stack_T black_edge_pixels
 * Returns: None
 */
void check_neighbors(int x, int y, Bit2_T bit_map, Stack_T black_edge_pixels)
{
  /* CRE if stack or bit map is null */
  assert((bit_map != NULL) && (black_edge_pixels != NULL));
  int height = Bit2_height(bit_map);
  int width = Bit2_width(bit_map);

  /*every if statement argument will either check to make sure that the
   * neighbor accessed is in bounds of the bit map and then it will check
   * to see if that specific neighboring pixel is black */
  if ((x + 1 < width - 1) && (y < height - 1) &&
      (y != 0) && (Bit2_get(bit_map, x + 1, y) == BLACK_PIXEL)) {
      turn_neighbor_white(x + 1, y, bit_map, black_edge_pixels);
  }
  if ((x > 1) && (y < height - 1) && (y != 0) &&
      (Bit2_get(bit_map, x - 1, y) == BLACK_PIXEL)) {
    turn_neighbor_white(x - 1, y, bit_map, black_edge_pixels);
  }
  if ((y + 1 < height - 1) && (x < width - 1) && (x != 0)
        && (Bit2_get(bit_map, x, y + 1) == BLACK_PIXEL)) {
    turn_neighbor_white(x, y + 1, bit_map, black_edge_pixels);
  }
  if ((y > 1) && (x < width - 1) && (x != 0)
      && (Bit2_get(bit_map, x, y - 1) == BLACK_PIXEL)) {
      turn_neighbor_white(x, y - 1, bit_map, black_edge_pixels);
  }
}

/* void turn_neighbor_white(int x, int y, Bit2_T bit_map,
                        Stack_T black_edge_pixels)
 * Function: Turns the neighbor pixel white and stores the location of that
 *           pixel in the black edge pixels stack.
 * Parameters: int x, int y, Bit2_T bit_map, Stack_T black_edge_pixels
 * Returns: None
 */
void turn_neighbor_white(int x, int y, Bit2_T bit_map,
                        Stack_T black_edge_pixels)
{
  /*check to make sure the stack and bit map are not null and that the
   *current x and y indices given are within bounds */
  assert((bit_map != NULL) && (black_edge_pixels != NULL));
  assert((x >= 0) && (y >= 0) && (x < Bit2_width(bit_map)) &&
          (y < Bit2_height(bit_map)));
  Location *curr = create_new_Location(x, y);
  Stack_push(black_edge_pixels, curr);
  make_pixel_white(x, y, bit_map);
}

/* void make_pixel_white(int x, int y, Bit2_T bit_map)
 * Function: Turns pixel at current location into a white pixel
 * Parameters: int x, int y, Bit2_T bit_map
 * Returns: None
 */
void make_pixel_white(int x, int y, Bit2_T bit_map)
{
  assert(bit_map != NULL);
  int temp = Bit2_put(bit_map, x, y, WHITE_PIXEL);
  (void)temp;
}
