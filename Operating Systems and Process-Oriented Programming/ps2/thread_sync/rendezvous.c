/**
 * Rendezvous
 *
 * Two threads executing chunks of work in a lock step - skeleton
 * code.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* abort(), [s]rand() */
#include <unistd.h>    /* sleep() */
#include <semaphore.h> /* sem_...() */
#include <pthread.h>   /* pthread_...() */

#define LOOPS 5
#define NTHREADS 3
#define MAX_SLEEP_TIME 3


/* TODO: Make the two threads perform their iterations in a
 * predictable way. Both should perform iteration 1 before iteration 2
 * and then 2 before 3 etc. */

sem_t mutexA;
sem_t mutexB;


void *
threadA(void *param __attribute__((unused)))
{
    int i;

    for (i = 0; i < LOOPS; i++) {
        
        sem_wait(&mutexA);
	printf("threadA --> %d iteration\n", i);
	sleep(rand() % MAX_SLEEP_TIME);
        sem_post(&mutexB);

    }

    pthread_exit(0);
}


void *
threadB(void *param  __attribute__((unused)))
{
    int i;

    for (i = 0; i < LOOPS; i++) {

        sem_wait(&mutexB);
	printf("threadB --> %d iteration\n", i);
	sleep(rand() % MAX_SLEEP_TIME);
        sem_post(&mutexA);
    }

    pthread_exit(0);
}

int
main()
{
    pthread_t tidA, tidB;
    sem_init(&mutexA,0,1);
    sem_init(&mutexB,0,1);
    srand(time(NULL));
    pthread_setconcurrency(3);

    if (pthread_create(&tidA, NULL, threadA, NULL) ||
	pthread_create(&tidB, NULL, threadB, NULL)) {
	perror("pthread_create");
	abort();
    }
    if (pthread_join(tidA, NULL) != 0 ||
        pthread_join(tidB, NULL) != 0) {
	perror("pthread_join");
	abort();
    }

    sem_destroy(&mutexA);
    sem_destroy(&mutexB);

    return 0;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "stroustrup"
 * End:
 */
