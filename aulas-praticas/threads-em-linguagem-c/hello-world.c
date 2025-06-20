#include <pthread.h> // https://pastebin.com/cYnxQfTz
#include <stdio.h>
#include <processthreadsapi.h>
#define NUM_THREADS 10

void *PrintHello(void *numFor) {
  printf("\tthread %d: Hello World!\n", numFor);
  pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    int rc, t;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_init(&attr);
    for(t = 0; t < NUM_THREADS; t++) {
        fprintf(stdout, "Criando thread %d\n", t);
        rc = pthread_create(&threads[t], &attr, PrintHello, (void *)t);
        if (rc) {
            fprintf(stderr, "ERRO; cód. retorno de pthread_create(): %d\n", rc);
            exit(-1);
        }
    }
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}

