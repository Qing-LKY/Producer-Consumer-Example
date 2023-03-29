#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "args.h"

typedef struct Pool {
    int limit;
    int cnt;
} pool_t;

pool_t pools;
pthread_mutex_t lock;
sem_t full, empty;

void init_global() {
    int ret;
    pools.limit = pool_siz;
    pools.cnt = 0;
    ret = pthread_mutex_init(&lock, NULL);
    if (ret != 0) {
        perror("mutex-init");
        exit(EXIT_FAILURE);
    }
    ret = sem_init(&full, 0, 0);
    if (ret != 0) {
        perror("sem-init(full)");
        exit(EXIT_FAILURE);
    }
    ret = sem_init(&empty, 0, pool_siz);
    if (ret != 0) {
        perror("sem-init(empty)");
        exit(EXIT_FAILURE);
    }
}

void *producer(void *arg) {
    while (1) {
        sem_wait(&empty);
        pthread_mutex_lock(&lock);
        fprintf(out_fd, "Producer: %d->%d\n", pools.cnt, pools.cnt + 1);
        pools.cnt++;
        pthread_mutex_unlock(&lock);
        sem_post(&full);
        usleep(rand() % 100000);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&lock);
        fprintf(out_fd, "Consumer: %d->%d\n", pools.cnt, pools.cnt - 1);
        pools.cnt--;
        pthread_mutex_unlock(&lock);
        sem_post(&empty);
        usleep(rand() % 100000);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t pt, ct;
    parse_args(argc, argv); 
    init_global();
    if (pthread_create(&pt, NULL, producer, NULL) != 0) {
        perror("Create producer");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&ct, NULL, consumer, NULL) != 0) {
        perror("Create consumer");
        exit(EXIT_FAILURE);
    }
    sleep(ti);
    return 0;
}