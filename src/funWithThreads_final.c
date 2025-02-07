/* TODO: Add library for pthreads */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* TODO: define a default of 4 threads */

/* Size of array to work with */
#define NUM_ROWS 100
#define NUM_COLS 100

double **large_array;
double large_array_size;

/* TODO: declare variabel that holds a default
 * number of threads or user supplied value
 * from command line
 */
int total_threads = 1;

void *transcendental_function_calc(void *t)
{
    int i;
    long tid;
    tid = (long)t;
    int total_calcs = 0;
    printf("Thread %ld of %d starting...\n", tid, total_threads);

    /* TODO: Only have thread i work it's cells of the array
     * For example, thread 0 would work on 0, 4, 8, 12, etc.
     * thread 1 would work on 1, 5, 9, 13, etc.
     */
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            if (1)
            {
                total_calcs++;

                /* Debugging aid remove in starter
                //large_array[i][j] = NUM_COLS * i+j;

                /* transcendtial function, oh fun*/
                large_array[i][j] = sin(i) * tan(j);
            }
        }
    }

    printf("Thread %ld done. total_calcs=%d\n", tid, total_calcs);

    /* TODO: exit each thread here */
}

int main(int argc, char *argv[])
{
    time_t the_time;
    clock_t measure_me;

    measure_me = clock();

    /* TODO: Dynamically allocate array of threads
     * as indicated on the command line or using
     * the default define of NUM_THREADS 4
     */

    /* TODO: Create a pthread attribute structure variable to control
     * the overall operation of threads in this application
     */

    int rc;
    long t;
    void *status;

    large_array = (double **)malloc(NUM_ROWS * sizeof(double *));
    for (int i = 0; i < NUM_ROWS; i++)
        large_array[i] = (double *)malloc(NUM_COLS * sizeof(double));

    large_array_size = NUM_ROWS * NUM_COLS;

    /* TODO: Initialize and set thread detached attribute
     * use state joinable
     */

    /* TODO:
     * Create each thread in a for loop and have it call the
     * transcendental_function_calc using only the loop
     * counter as the single parameter
     */
    for (t = 0; t < total_threads; t++)
    {
        printf("Main: creating thread %ld\n", t);

        // TODO: something here

        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* TODO:
     * Free attribute and wait for the other threads
     * Do not forget to release resources
     */

    // TODO: something here pthread related
    for (t = 0; t < total_threads; t++)
    {

        // TODO: something here too pthread related
        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Main: completed join with thread %ld having a status of %ld\n", t, (long)status);
    }

    // Simple debugging loop if needed
    FILE *fp;
    fp = fopen("output.txt", "w");
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_ROWS; j++)
            fprintf(fp, "%d %d = %f\n", i, j, large_array[i][j]);
    }
    fclose(fp);

    /* Release acquired memory for array */
    free(large_array);

    /* TODO: Release acquired memory for thread array */

    /* Complete and show timing information for run */
    printf("Main: program completed. Exiting.\n");
    measure_me = clock() - measure_me;
    double total_time = ((double)measure_me) / CLOCKS_PER_SEC;
    printf("Took %f seconds to execute\n", total_time);

    /* TODO: Close main thread */
}
