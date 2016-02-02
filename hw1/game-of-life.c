/***************************************************************************/
/* Template for Asssignment 1 **********************************************/
/* Your Name Here             **********************************************/
/***************************************************************************/

/***************************************************************************/
/* Includes ****************************************************************/
/***************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>

/***************************************************************************/
/* Defines *****************************************************************/
/***************************************************************************/

#define ALIVE 1
#define DEAD  0

/***************************************************************************/
/* Global Vars *************************************************************/
/***************************************************************************/

unsigned int **g_GOL_CELL=NULL;

unsigned int g_x_cell_size=0;
unsigned int g_y_cell_size=0;

unsigned int g_num_ticks=0;

double g_thresh_hold=0.0;

/***************************************************************************/
/* Function Decs ***********************************************************/
/***************************************************************************/

void allocate_and_init_cells();
void compute_one_tick();
void output_final_cell_state();

/***************************************************************************/
/* Function: Main **********************************************************/
/***************************************************************************/

int main(int argc, char *argv[])
{
  int i = 0;

  // insert your code to capture inputs from argc and argv
  // Argument 1 is size of X and Y cell dimensions
  // Argument 2 is the number of ticks
  // Argument 3 is the thresh hold percent 0%, 25%, 50%, 75% and 90%.
  // Convert strings to integer values using "atoi()"

  allocate_and_init_cells();

  for(i = 0; i < g_num_ticks; i++)
    {
      compute_one_tick();
    }

  output_final_cell_state();

  return 0;
}

/***************************************************************************/
/* Function: allocate_and_init_cells ***************************************/
/***************************************************************************/

void allocate_and_init_cells()
{
  // use "calloc" to allocate space for your cell matrix
  // use "drand48" to init the state of each grid cell once allocated.
}

/***************************************************************************/
/* Function: compute_one_tick **********************************************/
/***************************************************************************/

void compute_one_tick()
{
  // iterate over X (outside loop) and Y (inside loop) dimensions of the g_GOL_CELL
  // Use drand48() for uniform distribution. It is already included in stdlib.h
}


/***************************************************************************/
/* Function: output_final_cell_state ***************************************/
/***************************************************************************/

void output_final_cell_state()
{
  // print out in grid form 16 value per row of the g_GOL_CELL
  // This data will be used to create your graphs
}
