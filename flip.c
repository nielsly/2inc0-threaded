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
#include "semaphore.h" //for counting semaphore, to avoid busy waiting

// Some things taken from threaded_basics.c

// create a bitmask where bit at position n is set
#define BITMASK(n) (((uint128_t)1) << (n))

// check if bit n in v is set
#define BIT_IS_SET(v, n) (((v)&BITMASK(n)) == BITMASK(n))

// set bit n in v
#define BIT_SET(v, n) ((v) = (v) | BITMASK(n))

// clear bit n in v
#define BIT_CLEAR(v, n) ((v) = (v) & ~BITMASK(n))

#define NROF_BITS 128

sem_t sem;
sem_t buf_sem;

static void *flip_thread(void *arg);

int main(void)
{
    // TODO: start threads to flip the pieces and output the results
    // (see thread_test() and thread_mutex_test() how to use threads and mutexes,
    //  see bit_test() how to manipulate bits in a large integer)

    // Dev step 1

    /**
    * NROF_ELEMENTS: number of elements in the array
    */
    int NROF_ELEMENTS = (NROF_PIECES / NROF_BITS) + 1;

    // set all bits to 1

    for (int i = 0; i < NROF_ELEMENTS; i++)
        buffer[i] = ~0;

    // initialize counting semaphores, to track thread count and if buffer is being accessed
    sem_init(&sem, 0, NROF_THREADS);
    sem_init(&buf_sem, 0, 1);

    // we will flip each piece
    for (int i = 2; i < NROF_PIECES; i++)
    {
        // decrement semaphore counter once it is above 0
        sem_wait(&sem);

        // create thread, as the parameter we send the current piece
        int *parameter = malloc(sizeof(int));
        *parameter = i;
        pthread_t thread;

        if (pthread_create(&thread, NULL, flip_thread, parameter) != 0)
            // throw error if thread creation is blocked
            perror("pthread_create() error");
    }

    // wait until all threads are finished by decrementing the semaphore once possible NROF_THREADS times
    // this also works if less than NROF_THREADS pieces are used, as then the semaphore is not 0 when entering the loop
    for (int i = 0; i < NROF_THREADS; i++)
        sem_wait(&sem);

    // destroy the semaphores as we do not need them anymore
    sem_destroy(&buf_sem);
    sem_destroy(&sem);

    // loop over all pieces
    for (int i = 1; i < NROF_PIECES; i++)
        // print if square
        if (BIT_IS_SET(buffer[i / NROF_BITS], i % NROF_BITS))
            fprintf(stdout, "%d\n", i);

    return (0);
}

static void *flip_thread(void *arg)
{
    int *arguments, i;
    arguments = (int *)arg; // proper casting before dereferencing (could also be done in one statement)
    i = *arguments;         // get the integer value of the pointer
    free(arg);              // we retrieved the integer value, so now the pointer can be deleted

    for (int j = 1; i * j <= NROF_PIECES; j++)
    {
        // lock buffer
        sem_wait(&buf_sem);
        if (BIT_IS_SET(buffer[i * j / NROF_BITS], i * j % NROF_BITS))
            // flip it to 0
            BIT_CLEAR(buffer[i * j / NROF_BITS], i * j % NROF_BITS);
        else
            // If bit at position n is not set, flip to 1
            BIT_SET(buffer[i * j / NROF_BITS], i * j % NROF_BITS);
        // unlock buffer
        sem_post(&buf_sem);
    }

    // increment semaphore, indicating a new thread can be created
    sem_post(&sem);
    // detach self
    pthread_detach(pthread_self());
    // kill self
    pthread_exit(NULL);
}