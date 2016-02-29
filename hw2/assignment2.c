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

#define BEFORE_TAG 0
#define AFTER_TAG 1

/***************************************************************************/
/* Global Vars *************************************************************/
/***************************************************************************/

// Bring over from your code
unsigned int **g_GOL_CELL=NULL;
unsigned int g_x_cell_size=0;
unsigned int g_y_cell_size=0;
unsigned int g_num_ticks=0;
double g_threshold=0.0;

unsigned int *ghost_row_before = NULL;
unsigned int *ghost_row_after = NULL;

/***************************************************************************/
/* Function Decs ***********************************************************/
/***************************************************************************/

// Bring over from your code
void allocate_and_init_cells(int num_rows, int rank);
void compute_one_tick(int num_rows, int rank, int world_size);
void output_final_cell_state(int num_rows, int rank, int world_size);
void apply_gol_rules(int i, int j, int num_rows);
int count_alive_neighbors(int i, int j, int num_rows);
int mod (int a, int b);


/***************************************************************************/
/* Function: Main **********************************************************/
/***************************************************************************/

int main(int argc, char *argv[])
{
    int i = 0;
    int mpi_myrank;
    int mpi_commsize;
    double start_time, end_time;

// Example MPI startup and using CLCG4 RNG
    MPI_Init( &argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_commsize);
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_myrank);
    if (mpi_myrank == 0)
        start_time = MPI_Wtime();
    
// Init 16,384 RNG streams - each rank has an independent stream
    InitDefault();
    
// Note, use the mpi_myrank to select which RNG stream to use. 
    printf("Rank %d of %d has been started and a first Random Value of %lf\n", 
	   mpi_myrank, mpi_commsize, GenVal(mpi_myrank));
    
    MPI_Barrier( MPI_COMM_WORLD );
// Bring over rest from your code
    if (argc == 4)
    {
        if (atoi(argv[1]) > 0)
        {
            g_x_cell_size = atoi(argv[1]);
            g_y_cell_size = atoi(argv[1]);
        }
        else
        {
            perror("Grid size should be an integer larger than 0");
            exit(1);
        }
        if (atoi(argv[2]) > 0)
            g_num_ticks = atoi(argv[2]);
        else
        {
            perror("Number of ticks should be an integer larger than 0");
            exit(1);
        }
        if (atoi(argv[3]) >= 0)
            g_threshold = atoi(argv[3])*.01;
        else
        {
            perror("Threshold should be an integer greater than or equal to 0");
            exit(1);
        }
    }
    else
    {
        perror("Expects 3 arguments! size, num_ticks, and threshold");
        exit(1);
    }

    allocate_and_init_cells(g_y_cell_size / mpi_commsize, mpi_myrank);

    for(i = 0; i < g_num_ticks; i++)
    {
        compute_one_tick(g_y_cell_size / mpi_commsize, mpi_myrank, mpi_commsize);
    }

    //output_final_cell_state(g_y_cell_size / mpi_commsize, mpi_myrank, mpi_commsize);

// END -Perform a barrier and then leave MPI
    MPI_Barrier( MPI_COMM_WORLD );
    if (mpi_myrank == 0)
    {
        end_time = MPI_Wtime();
        printf("Run time: %f seconds\n", end_time-start_time);
    }
    MPI_Finalize();
    return 0;
}

/***************************************************************************/
/* Other Functions - Bring over from your Assignment 1 *********************/
/***************************************************************************/
/***************************************************************************/
/* Function: allocate_and_init_cells ***************************************/
/***************************************************************************/

void allocate_and_init_cells(int num_rows, int rank)
{
    // initialize my own rows of universe
    int i, j;
    g_GOL_CELL = calloc(num_rows, sizeof(unsigned int*));
    if (!g_GOL_CELL)
    {
        perror("calloc failed");
        MPI_Finalize();
        exit(1);
    }

    for (i = 0; i < num_rows; i++)
    {
        g_GOL_CELL[i] = calloc(g_x_cell_size, sizeof(unsigned int));
        if (!g_GOL_CELL[i])
        {
            perror("calloc failed");
            MPI_Finalize();
            exit(1);
        }

        for (j = 0; j < g_x_cell_size; j++)
        {
            // use a different RNG seed per row (based on absolute row number)
            double r = GenVal(rank * num_rows + i);
            if (r < .5)
                g_GOL_CELL[i][j] = ALIVE;
            else
                g_GOL_CELL[i][j] = DEAD;
        }
    }

    // allocate memory for ghost rows
    ghost_row_before = calloc(g_x_cell_size, sizeof(unsigned int));
    if (!ghost_row_before)
    {
        perror("calloc failed");
        MPI_Finalize();
        exit(1);
    }
    ghost_row_after = calloc(g_x_cell_size, sizeof(unsigned int));
    if (!ghost_row_after)
    {
        perror("calloc failed");
        MPI_Finalize();
        exit(1);
    }
    

}

/***************************************************************************/
/* Function: compute_one_tick **********************************************/
/***************************************************************************/

void compute_one_tick(int num_rows, int rank, int world_size)
{
    // need to send/receive to rank++ and rank-- (handle end cases as well)
    MPI_Request rb, sb, ra, sa;
    MPI_Status status;
    MPI_Irecv(ghost_row_before, g_x_cell_size, MPI_UNSIGNED, MPI_ANY_SOURCE, BEFORE_TAG,
            MPI_COMM_WORLD, &rb);
    MPI_Irecv(ghost_row_after, g_x_cell_size, MPI_UNSIGNED, MPI_ANY_SOURCE, AFTER_TAG,
            MPI_COMM_WORLD, &ra);

    // before sending, copy to temp buffer for non blocking sends  
    unsigned int tmp_before[g_x_cell_size];
    unsigned int tmp_after[g_x_cell_size];
    memcpy(&tmp_before, g_GOL_CELL[0], sizeof(unsigned int)*g_x_cell_size);
    memcpy(&tmp_after, g_GOL_CELL[num_rows-1], sizeof(unsigned int)*g_x_cell_size);
    MPI_Isend(tmp_before, g_x_cell_size, MPI_UNSIGNED, mod(rank-1, world_size), BEFORE_TAG, 
           MPI_COMM_WORLD, &sb); 
    MPI_Isend(tmp_after, g_x_cell_size, MPI_UNSIGNED, mod(rank+1, world_size), AFTER_TAG, 
           MPI_COMM_WORLD, &sa); 

    // need to wait to receive 1st ghost row
    MPI_Wait(&rb, &status);

    // update matrices
    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        if (i == num_rows-1) // make sure we have final ghost row
            MPI_Wait(&ra, &status);
        for (j = 0; j < g_x_cell_size; j++)
        {
            double r = GenVal(rank * num_rows + i);
            if (r > g_threshold)
            {
                // random number greater than given threshold
                // use normal GOL rules
                apply_gol_rules(i, j, num_rows);
            }
            else // otherwise choose a random state
                g_GOL_CELL[i][j] = (GenVal(rank * num_rows + i) < .5) ? ALIVE : DEAD;
        }
    }
}

/***************************************************************************/
/* Function: output_final_cell_state ***************************************/
/***************************************************************************/

void output_final_cell_state(int num_rows, int rank, int world_size)
{
    // print out in grid form 16 value per row of the g_GOL_CELL
    // This data will be used to create your graphs
    FILE *f;
    char filename[64];
    sprintf(filename, "gol-%d-%d-%d-%d-rank%d.out", g_x_cell_size, g_num_ticks, (int)(g_threshold*100), world_size, rank);
    f = fopen(filename, "w");
    if (!f)
    {
        perror("unable to open file");
        MPI_Finalize();
        exit(1);
    }

    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        char line[g_x_cell_size * 2 + 1];
        line[0]=0;
        for (j = 0; j < g_x_cell_size; j++)
        {
            char tmp[4];
            sprintf(tmp, "%d ", g_GOL_CELL[i][j]);
            strcat(line, tmp);

        }
        fprintf(f, "%s\n", line);
    }
    fclose(f);
}

/***************************************************************************/
/* Helper Functions ********************************************************/
/***************************************************************************/

/* apply the game of life rules to the cell in position (i,j) */
void apply_gol_rules(int i, int j, int num_rows)
{
    int total_alive = count_alive_neighbors(i, j, num_rows);
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
int count_alive_neighbors(int i, int j, int num_rows)
{
    int total_alive = 0;
    int i_u = mod(i+1, num_rows);
    int i_d = mod(i-1, num_rows);
    int j_r = mod(j+1, (int)g_x_cell_size);
    int j_l = mod(j-1, (int)g_x_cell_size);
    if (i == 0)
    {
        // up, down, left, right neighbors
        if (g_GOL_CELL[i_u][j] == ALIVE)
            total_alive++;
        if (ghost_row_before[j] == ALIVE)
            total_alive++;
        if (g_GOL_CELL[i][j_r] == ALIVE)
            total_alive++;
        if (g_GOL_CELL[i][j_l] == ALIVE)
            total_alive++;

        // diagonal neighbors
        if (g_GOL_CELL[i_u][j_l] == ALIVE)
            total_alive++;
        if (ghost_row_before[j_r] == ALIVE)
            total_alive++;
        if (g_GOL_CELL[i_u][j_r] == ALIVE)
            total_alive++;
        if (ghost_row_before[j_l] == ALIVE)
            total_alive++;
    }
    else if (i == num_rows-1)
    {
        // up, down, left, right neighbors
        if (ghost_row_after[j] == ALIVE)
            total_alive++;
        if (g_GOL_CELL[i_d][j] == ALIVE)
            total_alive++;
        if (g_GOL_CELL[i][j_r] == ALIVE)
            total_alive++;
        if (g_GOL_CELL[i][j_l] == ALIVE)
            total_alive++;

        // diagonal neighbors
        if (ghost_row_after[j_l] == ALIVE)
            total_alive++;
        if (g_GOL_CELL[i_d][j_r] == ALIVE)
            total_alive++;
        if (ghost_row_after[j_r] == ALIVE)
            total_alive++;
        if (g_GOL_CELL[i_d][j_l] == ALIVE)
            total_alive++;
    }
    else
    {
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
    }


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
