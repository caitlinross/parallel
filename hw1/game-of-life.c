/***************************************************************************/
/* Template for Asssignment 1 **********************************************/
/* Caitlin Ross               **********************************************/
/***************************************************************************/

/***************************************************************************/
/* Includes ****************************************************************/
/***************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>
#include<time.h>

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

double g_threshold=0.0;

/***************************************************************************/
/* Function Decs ***********************************************************/
/***************************************************************************/

void allocate_and_init_cells();
void compute_one_tick();
void output_final_cell_state();
void apply_gol_rules(int i, int j);
int count_alive_neighbors(int i, int j);
int mod (int a, int b);

/***************************************************************************/
/* Function: Main **********************************************************/
/***************************************************************************/

int main(int argc, char *argv[])
{
    int i = 0;
    srand48(time(0));
    // insert your code to capture inputs from argc and argv
    // Argument 1 is size of X and Y cell dimensions
    // Argument 2 is the number of ticks
    // Argument 3 is the thresh hold percent 0%, 25%, 50%, 75% and 90%.
    // Convert strings to integer values using "atoi()"
    if (argc == 4)
    {
        //TODO add in error checking
        g_x_cell_size = atoi(argv[1]);
        g_y_cell_size = atoi(argv[1]);
        g_num_ticks = atoi(argv[2]);
        g_threshold = atoi(argv[3])*.01;
    }
    else
        perror("Expects 3 arguments! size, num_ticks, and threshold");

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
    int i, j;
    // use "calloc" to allocate space for your cell matrix
    // use "drand48" to init the state of each grid cell once allocated.
    g_GOL_CELL = calloc(g_x_cell_size, sizeof(unsigned int*));
    for (i = 0; i < g_x_cell_size; i++)
    {
        //TODO calloc error checking
        g_GOL_CELL[i] = calloc(g_y_cell_size, sizeof(unsigned int));
        for (j = 0; j < g_y_cell_size; j++)
        {
            double r = drand48();
            if (r < .5)
                g_GOL_CELL[i][j] = ALIVE;
            else
                g_GOL_CELL[i][j] = DEAD;
        }
    }
}

/***************************************************************************/
/* Function: compute_one_tick **********************************************/
/***************************************************************************/

void compute_one_tick()
{
    // iterate over X (outside loop) and Y (inside loop) dimensions of the g_GOL_CELL
    // Use drand48() for uniform distribution. It is already included in stdlib.h
    int i, j;
    for (i = 0; i < g_x_cell_size; i++)
    {
        for (j = 0; j < g_y_cell_size; j++)
        {
            double r = drand48();
            if (r > g_threshold)
            {
                // random number greater than given threshold
                // use normal GOL rules
                apply_gol_rules(i, j);
            }
            else // otherwise choose a random state
                g_GOL_CELL[i][j] = (drand48() > .5) ? ALIVE : DEAD;
        }
    }
}

/***************************************************************************/
/* Function: output_final_cell_state ***************************************/
/***************************************************************************/

void output_final_cell_state()
{
    // TODO send output to file
    // print out in grid form 16 value per row of the g_GOL_CELL
    // This data will be used to create your graphs
    int i, j;
    for (i = 0; i < g_x_cell_size; i++)
    {
        for (j = 0; j < g_y_cell_size; j++)
        {
            if (j == g_y_cell_size - 1)
                printf("%d\n", g_GOL_CELL[i][j]);
            else
                printf("%d,", g_GOL_CELL[i][j]);
        }
    }
}

/***************************************************************************/
/* Helper Functions ********************************************************/
/***************************************************************************/

void apply_gol_rules(int i, int j)
{
    int total_alive = count_alive_neighbors(i, j);
    if (g_GOL_CELL[i][j] == ALIVE)
    {
        if (total_alive < 2)
            g_GOL_CELL[i][j] = DEAD;
        else if (total_alive > 3)
            g_GOL_CELL[i][j] = DEAD;
    }
    else // dead
    {
        if (total_alive == 3)
            g_GOL_CELL[i][j] = ALIVE;
    }
}

int count_alive_neighbors(int i, int j)
{
    // count alive neighbors
    int total_alive = 0;
    int i_u = mod(i+1, (int)g_x_cell_size);
    int i_d = mod(i-1, (int)g_x_cell_size);
    int j_r = mod(j+1, (int)g_y_cell_size);
    int j_l = mod(j-1, (int)g_y_cell_size);
    if (g_GOL_CELL[i_u][j] == ALIVE)
        total_alive++;
    if (g_GOL_CELL[i_d][j] == ALIVE)
        total_alive++;
    if (g_GOL_CELL[i][j_r] == ALIVE)
        total_alive++;
    if (g_GOL_CELL[i][j_l] == ALIVE)
        total_alive++;

    // diagonal neighbors
    if (g_GOL_CELL[i_u][j_l] == ALIVE)
        total_alive++;
    if (g_GOL_CELL[i_d][j_r] == ALIVE)
        total_alive++;
    if (g_GOL_CELL[i_u][j_r] == ALIVE)
        total_alive++;
    if (g_GOL_CELL[i_d][j_l] == ALIVE)
        total_alive++;

    return total_alive;
}

int mod (int a, int b)
{
    int ret = a % b;
    if(ret < 0)
        ret+=b;
    return ret;
}
