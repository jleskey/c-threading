#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Default number of threads
#define NUM_THREADS 4

// Default array dimensions
#define NUM_ROWS 100
#define NUM_COLS 100

// Large array variables
double **large_array;
double large_array_size;

// NOTE: The user should be able to provide this value.
int total_threads = NUM_THREADS;

// Utility function declaration
void check_allocation(void *ptr);

void *transcendental_function_calc(void *t)
{
    long tid = (long)t;
    int total_calcs = 0;
    printf("Thread %ld of %d starting...\n", tid, total_threads);

    // We'll split up the rows between the threads.
    for (long i = tid; i < NUM_ROWS; i += total_threads)
    {
        for (long j = 0; j < NUM_COLS; j++)
        {
            total_calcs++;

            /* transcendtial function, oh fun*/
            large_array[i][j] = sin(i) * tan(j);
        }
    }

    printf("Thread %ld done. total_calcs=%d\n", tid, total_calcs);

    // Exit the thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    time_t the_time;
    clock_t measure_me;

    measure_me = clock();

    // total_threads override
    if (argc > 1)
    {
        total_threads = atoi(argv[1]);

        if (total_threads < 1)
        {
            fprintf(stderr, "Invalid thread count: %d", total_threads);
            exit(EXIT_FAILURE);
        }
        else if (total_threads > NUM_ROWS)
        {
            printf("Chosen thread count exceeds total number of rows. "
                   "Limiting to %d.\n",
                   NUM_ROWS);
            total_threads = NUM_ROWS;
        }
    }

    // Allocate thread array.
    pthread_t *threads = malloc(total_threads * sizeof(pthread_t));
    check_allocation(threads);

    // Set up thread structure.
    pthread_attr_t attr;

    large_array = (double **)malloc(NUM_ROWS * sizeof(double *));
    check_allocation(large_array);

    for (int i = 0; i < NUM_ROWS; i++)
    {
        large_array[i] = (double *)malloc(NUM_COLS * sizeof(double));
        check_allocation(large_array[i]);
    }

    large_array_size = NUM_ROWS * NUM_COLS;

    // Initialize thread structure.
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Create threads.
    for (long t = 0; t < total_threads; t++)
    {
        printf("Main: creating thread %ld\n", t);

        int rc = pthread_create(&threads[t], &attr,
                                transcendental_function_calc, (void *)t);

        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Free thread structure.
    pthread_attr_destroy(&attr);

    // Join threads.
    for (long t = 0; t < total_threads; t++)
    {
        void *status;

        int rc = pthread_join(threads[t], &status);

        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Main: completed join with thread %ld having a status of %ld\n",
                t, (long)status);
    }

    // Simple debugging loop if needed
    FILE *fp;
    fp = fopen("output.txt", "w");
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
            fprintf(fp, "%d %d = %f\n", i, j, large_array[i][j]);
    }
    fclose(fp);

    /* Release acquired memory for array */
    for (int i = 0; i < NUM_ROWS; i++)
        free(large_array[i]);
    free(large_array);

    // Release acquired memory for thread array.
    free(threads);

    /* Complete and show timing information for run */
    printf("Main: program completed. Exiting.\n");
    measure_me = clock() - measure_me;
    double total_time = ((double)measure_me) / CLOCKS_PER_SEC;
    printf("Took %f seconds to execute\n", total_time);

    // Close main thread.
    return EXIT_SUCCESS;
}

void check_allocation(void *ptr)
{
    if (ptr == NULL)
    {
        fprintf(stderr, "Could not allocate memory.\n");
        exit(EXIT_FAILURE);
    }
}
