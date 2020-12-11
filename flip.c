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

// Some basic defines taken from threaded_basics.c

// create a bitmask where bit at position n is set
#define BITMASK(n) (((uint128_t)1) << (n))

// check if bit n in v is set
#define BIT_IS_SET(v, n) (((v)&BITMASK(n)) == BITMASK(n))

// set bit n in v
#define BIT_SET(v, n) ((v) = (v) | BITMASK(n))

// clear bit n in v
#define BIT_CLEAR(v, n) ((v) = (v) & ~BITMASK(n))

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

    for (int i = 0; i < NROF_ELEMENTS; i++) {
        buffer[i] = ~0;
    }

    for (int progress = 2; progress < NROF_PIECES; progress++) {
        for (int i = 1; i < NROF_PIECES; i++) {
            if (i % progress == 0) {
                if (BIT_IS_SET(buffer[i/128], i%128)) {
                // flip it to 0
                    BIT_CLEAR(buffer[i/128], i%128);
                } else {
                // If bit at position n is not set, flip to 1
                    BIT_SET(buffer[i/128], i%128);
                }
            }
        }
    }

    for (int i = 0; i < NROF_ELEMENTS; i++) {
        for (int j = 0; j < 128; j++) {
            if (BIT_IS_SET(buffer[i], j) && (i*128 + j) < NROF_PIECES) {
                printf("%d\n", i*128 + j);
            }
        }
    }   

    return (0);
}

