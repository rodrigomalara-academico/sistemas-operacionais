#include <stdio.h>
#include <pthread.h>
//#include <sys/types.h>
//#include <unistd.h>
#include <signal.h>

#define GREEN  1
#define YELLOW 2
#define RED    3

#define PRESSED 4
#define NOTPRESSED 5

// Mutex para proteger semaforo de 3 estados
pthread_mutex_t mut;
int semaph = RED; // semáforo começa vermelho

pthread_mutex_t mutctrlc;
int ctrlc = NOTPRESSED; // ver se ctrl-C foi pressionado

pthread_cond_t cond;

pthread_t threadsg, thready, threadr;


void *toGreen( void *id ) {
    // thread que muda para verde
    // So executa a mudanca para verde se o semaforo estiver vermelho
    int localctrlc;
    do {
        pthread_mutex_lock( &mutctrlc );
        localctrlc = ctrlc;
        pthread_mutex_unlock( &mutctrlc );

        pthread_mutex_lock( &mut );
        while( semaph != RED ) pthread_cond_wait( &cond, &mut );
        semaph = GREEN;
        printf( "Verde\n" );
        sleep( 1 ); // deixa verde por 5 segundos
        pthread_mutex_unlock( &mut );
        pthread_cond_signal( &cond );

    } while ( localctrlc != PRESSED );
    pthread_exit(NULL);
}

void *toYellow( void *id ) {
    // thread que muda para amarelo
    // So executa a mudanca para amarelo se o semaforo estiver verde
    int localctrlc;
    do {
        pthread_mutex_lock( &mutctrlc );
        localctrlc = ctrlc;
        pthread_mutex_unlock( &mutctrlc );

        pthread_mutex_lock( &mut );
        while( semaph != GREEN ) pthread_cond_wait( &cond, &mut );
        semaph = YELLOW;
        printf( "Amarelo\n" );
        sleep( 1 ); // deixa amarelo por 1 segundo
        pthread_mutex_unlock( &mut );
        pthread_cond_signal( &cond );

    } while ( localctrlc != PRESSED );
    pthread_exit(NULL);
}

void *toRed( void *id ) {
    // thread que muda para vermelho
    // So executa a mudanca para vermelho se o semaforo estiver amarelo
    int localctrlc;
    do {
        pthread_mutex_lock( &mutctrlc );
        localctrlc = ctrlc;
        pthread_mutex_unlock( &mutctrlc );

        pthread_mutex_lock( &mut );
        while( semaph != YELLOW ) pthread_cond_wait( &cond, &mut );
        semaph = RED;
        printf( "Vermelho\n" );
        sleep( 1 ); // deixa vermelho por 3 segundos
        pthread_mutex_unlock( &mut );
        pthread_cond_signal( &cond );

    } while ( localctrlc != PRESSED );
    pthread_exit(NULL);
}

void sig_int(void) {
    fprintf( stderr, "Usuario teclou control+C: calma que o programa termina sozinho\n" );
    ctrlc = PRESSED;
}

int main( int argc, char *argv[] ) {
    int rc;

	pthread_mutex_init( &mut,NULL );
	pthread_mutex_init( &mutctrlc,NULL );
	pthread_cond_init( &cond, NULL );

    signal(SIGINT, (void *)sig_int);

    printf( "Programa semaforo. Tecle Ctrl-C para terminar\n" );
    
    printf( "Criando thread verde\n" );
	rc = pthread_create( &threadsg, NULL, toGreen, (void *)GREEN );

    printf( "Criando thread amarelo\n" );
	rc = pthread_create( &thready, NULL, toYellow, (void *)YELLOW );

    printf( "Criando thread vermelho\n" );
	rc = pthread_create( &threadr, NULL, toRed, (void *)RED );

	pthread_exit( NULL );

	pthread_cond_destroy( &cond );
	pthread_mutex_destroy( &mut );
	pthread_mutex_destroy( &mutctrlc );

}
