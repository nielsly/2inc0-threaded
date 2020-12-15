/* 
 * Operating Systems <2INCO> Practical Assignment
 * Threaded Application
 *
 * Niels Gorter (1332678)
 * Ruben Wolters (1342355)
 *
 * Grading:
 * Students who hand in clean code that fully satisfies the minimum requirements will get an 8. 
 * Extra steps can lead to higher marks because we want students to take the initiative. 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h> // for perror()
#include <pthread.h>

#include "flip.h"
#include "semaphore.h"

// Some basic defines taken from threaded_basics.c

// create a bitmask where bit at position n is set
#define BITMASK(n) (((uint128_t)1) << (n))

// check if bit n in v is set
#define BIT_IS_SET(v, n) (((v)&BITMASK(n)) == BITMASK(n))

// set bit n in v
#define BIT_SET(v, n) ((v) = (v) | BITMASK(n))

// clear bit n in v
#define BIT_CLEAR(v, n) ((v) = (v) & ~BITMASK(n))

sem_t sem;
// int free_ids[NROF_THREADS];

static void *flip_thread(void *arg);
// int min(int a, int b);
// void free_id(int id);
// int take_id();

// typedef struct
// {
//     int i;
//     pthread_t id;
// } THREAD_ARGS;

int main(void)
{
    // TODO: start threads to flip the pieces and output the results
    // (see thread_test() and thread_mutex_test() how to use threads and mutexes,
    //  see bit_test() how to manipulate bits in a large integer)

    // Dev step 1

    // set all bits to 1

    /**
    * NROF_ELEMENTS: number of elements in the array
    */
    int NROF_ELEMENTS = (NROF_PIECES / 128) + 1;

    for (int i = 0; i < NROF_ELEMENTS; i++)
        buffer[i] = ~0;

    // for (int i = 0; i < NROF_THREADS; i++)
    //     free_ids[i] = i;

    // pthread_t threads[NROF_THREADS];
    //THREAD_ARGS parameters;
    sem_init(&sem, 0, NROF_THREADS);

    for (int i = 2; i < NROF_PIECES; i++)
    {
        sem_wait(&sem);
        pthread_t thread;
        //int id = take_id();

        // if (id != -1)
        // {
        int *parameter = malloc(sizeof(int));
        *parameter = i;

        if (pthread_create(&thread, NULL, flip_thread, parameter) != 0)
            perror("pthread_create() error");
        // }
        // else
        // {
        //     fprintf(stderr, "Thread did not exit before new thread was created");
        // }
    }

    // for (int i = 0; i < min(NROF_THREADS, NROF_PIECES - 2); i++)
    //     pthread_join(threads[i], NULL);

    for (int i = 0; i < NROF_THREADS; i++)
        sem_wait(&sem);

    sem_destroy(&sem);

    for (int i = 1; i < NROF_PIECES; i++)
        if (BIT_IS_SET(buffer[i / 128], i % 128))
            fprintf(stdout, "%d\n", i);

    return (0);
}

static void *flip_thread(void *arg)
{
    int *arguments;
    int i;

    arguments = (int *)arg; // proper casting before dereferencing (could also be done in one statement)
    i = *arguments;         // get the integer value of the pointer
    free(arg);              // we retrieved the integer value, so now the pointer can be deleted

    //printf("        %lx: thread started; parameter=%d\n", pthread_self(), j);
    for (int j = 1; i * j <= NROF_PIECES; j++)
    {
        if (BIT_IS_SET(buffer[i * j / 128], i * j % 128))
            // flip it to 0
            BIT_CLEAR(buffer[i * j / 128], i * j % 128);
        else
            // If bit at position n is not set, flip to 1
            BIT_SET(buffer[i * j / 128], i * j % 128);
    }

    //int id = (*arguments).id;
    //free_id(id);
    sem_post(&sem);
    pthread_detach(pthread_self());
    pthread_exit(NULL);
}

// int min(int a, int b)
// {
//     return a < b ? a : b;
// }

// //fifo free id
// void free_id(int id)
// {
//     for (int i = 0; i < NROF_THREADS; i++)
//         if (free_ids[i] == -1)
//         {
//             free_ids[i] = id;
//             return;
//         }
// }

// //fifo take id
// int take_id()
// {
//     int id;
//     if (free_ids[0] == -1)
//         return -1;
//     else
//         id = free_ids[0];

//     for (int i = 1; i < NROF_THREADS; i++)
//     {
//         if (free_ids[i] == -1)
//             break;
//         free_ids[i - 1] = free_ids[i];
//     }

//     free_ids[NROF_THREADS - 1] = -1;

//     return id;
// }