#include <stdio.h>
#include <pthread.h>
#define NUM_THREADSI 10  // número de threads de incremento
#define NUM_THREADSD 15  // número de threads de decremento

int count = 0;
pthread_mutex_t mut;
pthread_cond_t cond;

void *decrement( void *id ) {
    int p = (int *)id;
    // adquire o mutex
    pthread_mutex_lock( &mut );
    // enquanto o contador for zero, espera
    // (caso outro thread seja mais rapido...)
    if( count == 0 )
	pthread_cond_wait( &cond, &mut );
    count--;
    printf( "DEC #%d - %d\n", p, count );
    pthread_mutex_unlock( &mut );  // libera o mutex
    pthread_exit(NULL);
}

void *increment( void *id ) {
    int p = (int *)id;
    pthread_mutex_lock( &mut );
        count++;
        printf( "INC #%d - %d\n", p, count );
        pthread_cond_signal( &cond );
    pthread_mutex_unlock( &mut );
    pthread_exit(NULL);
}

int main( int argc, char *argv[] ) {
    pthread_t threadsi[NUM_THREADSI];
    pthread_t threadsd[NUM_THREADSD];
    int rc, t;

    pthread_mutex_init( &mut,NULL );
    pthread_cond_init( &cond, NULL );

    for( t = 0; t < NUM_THREADSI; t++ ) {
        printf( "Criando thread incremento %d\n", t );
        rc = pthread_create(&threadsi[t],NULL,increment,(void *)t );
    }
    for( t = 0; t < NUM_THREADSD; t++ ) {
        printf( "Criando thread decremento %d\n", t );
        rc = pthread_create(&threadsd[t],NULL,decrement,(void *)t );
    }
    sleep(12);     // va dormir por 2 segundos
    for( t = 0; t < NUM_THREADSD - NUM_THREADSI; t++ ) {
        printf( "Criando mais threads de incremento %d\n", t );
        rc = pthread_create(&threadsi[t],NULL,increment,(void *)t );
    }
    // aguarda todos os threads terminarem
    for( t = 0; t<NUM_THREADSD;t++) pthread_join(threadsd[t], NULL);
    for( t = 0; t<NUM_THREADSI;t++) pthread_join(threadsi[t],NULL);

    printf( "Valor final: %d\n", count );

    pthread_cond_destroy( &cond );
    pthread_mutex_destroy( &mut );
    pthread_exit( NULL );
}

