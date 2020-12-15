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

#include "uint128.h"
#include "flip.h"
#include "semaphore.h"
#include <math.h>

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

static void *flip_thread(void *arg);
int min(int a, int b);

typedef struct
{
    int i;
    pthread_t id;
} THREAD_ARGS;

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
    {
        buffer[i] = ~0;
    }

    int current_id;
    pthread_t thread_id[NROF_THREADS];
    THREAD_ARGS parameters[NROF_THREADS];
    //sem_init(&sem, 0, NROF_THREADS);

    for (int i = 2; i < NROF_PIECES; i++)
    {
        //sem_wait(&sem);
        current_id = (i - 2) % NROF_THREADS;
        parameters[current_id].id = current_id;
        parameters[current_id].i = i;

        if (pthread_create(&thread_id[current_id], NULL, &flip_thread, (void *)&parameters) != 0)
        {
            perror("pthread_create() error");
        }
    }

    for (int i = 0; i < min(NROF_THREADS, NROF_PIECES - 2); i++)
    {
        pthread_join(thread_id[i], NULL);
    }

    //sem_destroy(&sem);

    for (int i = 0; i < NROF_PIECES; i++)
    {
        if (BIT_IS_SET(buffer[i / 128], i % 128))
        {
            //printf("%d\n", i);
        }
    }

    return (0);
}

static void *flip_thread(void *arg)
{
    // TODOperror("test1");
    THREAD_ARGS *arguments;
    int i;

    arguments = (THREAD_ARGS *)arg; // proper casting before dereferencing (could also be done in one statement)
    i = (*arguments).i;             // get the integer value of the pointer
    //free(arg);                      // we retrieved the integer value, so now the pointer can be deleted

    //printf("        %lx: thread started; parameter=%d\n", pthread_self(), j);
    // for (int i = 1; i * j <= NROF_PIECES; i++)
    // {

    //     if (BIT_IS_SET(buffer[i * j / 128], i * j % 128))
    //     {
    //         // flip it to 0
    //         BIT_CLEAR(buffer[i * j / 128], i * j % 128);
    //     }
    //     else
    //     {
    //         // If bit at position n is not set, flip to 1
    //         BIT_SET(buffer[i * j / 128], i * j % 128);
    //     }
    // }
    buffer[i / 128] = buffer[i / 128] ^ BITMASK(i % 128);
    //sem_post(&sem);
    return (0); // you can also use pthread_exit(rtnval);
}

int min(int a, int b)
{
    return a < b ? a : b;
}