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
// New Includes for Assignment 2
#include<clcg4.h>
#include<mpi.h>


/***************************************************************************/
/* Defines *****************************************************************/
/***************************************************************************/

#define ALIVE 1
#define DEAD  0

/***************************************************************************/
/* Global Vars *************************************************************/
/***************************************************************************/

// Bring over from your code
unsigned int **g_GOL_CELL=NULL;
unsigned int g_x_cell_size=0;
unsigned int g_y_cell_size=0;
unsigned int g_num_ticks=0;
double g_threshold=0.0;

/***************************************************************************/
/* Function Decs ***********************************************************/
/***************************************************************************/

// Bring over from your code
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
//    int i = 0;
    int mpi_myrank;
    int mpi_commsize;
// Example MPI startup and using CLCG4 RNG
    MPI_Init( &argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_commsize);
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_myrank);
    
// Init 16,384 RNG streams - each rank has an independent stream
    InitDefault();
    
// Note, use the mpi_myrank to select which RNG stream to use. 
    printf("Rank %d of %d has been started and a first Random Value of %lf\n", 
	   mpi_myrank, mpi_commsize, GenVal(mpi_myrank));
    
    MPI_Barrier( MPI_COMM_WORLD );
// Bring over rest from your code
    

// END -Perform a barrier and then leave MPI
    MPI_Barrier( MPI_COMM_WORLD );
    MPI_Finalize();
    return 0;
}

/***************************************************************************/
/* Other Functions - Bring over from your Assignment 1 *********************/
/***************************************************************************/
/***************************************************************************/
/* Function: allocate_and_init_cells ***************************************/
/***************************************************************************/

void allocate_and_init_cells()
{
    int i, j;
    // use "calloc" to allocate space for your cell matrix
    // use "drand48" to init the state of each grid cell once allocated.
    g_GOL_CELL = calloc(g_x_cell_size, sizeof(unsigned int*));
    if (!g_GOL_CELL)
    {
        perror("calloc failed");
        exit(1);
    }

    for (i = 0; i < g_x_cell_size; i++)
    {
        g_GOL_CELL[i] = calloc(g_y_cell_size, sizeof(unsigned int));
        if (!g_GOL_CELL[i])
        {
            perror("calloc failed");
            exit(1);
        }

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
            if (r > g_thresh_hold)
            {
                // random number greater than given threshold
                // use normal GOL rules
                apply_gol_rules(i, j);
            }
            else // otherwise choose a random state
                g_GOL_CELL[i][j] = (drand48() < .5) ? ALIVE : DEAD;
        }
    }
}

/***************************************************************************/
/* Function: output_final_cell_state ***************************************/
/***************************************************************************/

void output_final_cell_state()
{
    // print out in grid form 16 value per row of the g_GOL_CELL
    // This data will be used to create your graphs
    FILE *f;
    char filename[64];
    sprintf(filename, "gol-%d-%d-%d.out", g_x_cell_size, g_num_ticks, (int)(g_thresh_hold*100));
    f = fopen(filename, "w");
    if (!f)
    {
        perror("unable to open file");
        exit(1);
    }

    int i, j;
    for (i = 0; i < g_x_cell_size; i++)
    {
        char line[g_x_cell_size * 2 + 1];
        line[0]=0;
        for (j = 0; j < g_y_cell_size; j++)
        {
            char tmp[4];
            sprintf(tmp, "%d ", g_GOL_CELL[i][j]);
            strcat(line, tmp);

        }
        fprintf(f, "%s\n", line);
        printf("%s\n", line);
    }
    fclose(f);
}

/***************************************************************************/
/* Helper Functions ********************************************************/
/***************************************************************************/

/* apply the game of life rules to the cell in position (i,j) */
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

/* count alive neighbors */
int count_alive_neighbors(int i, int j)
{
    int total_alive = 0;
    int i_u = mod(i+1, (int)g_x_cell_size);
    int i_d = mod(i-1, (int)g_x_cell_size);
    int j_r = mod(j+1, (int)g_y_cell_size);
    int j_l = mod(j-1, (int)g_y_cell_size);
    // up, down, left, right neighbors
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

/* % doesn't work the way I want it to when a < 0
 * so use this mod function instead
 */
int mod (int a, int b)
{
    int rem = a % b;
    if(rem < 0)
        rem += b;
    return rem;
}
