#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

#define TAMANHOBUFFER 5
#define TAMANHOMIDIA 20

// inicializacao dos semaforos
pthread_mutex_t  mutexBuffer; // proteger buffer
sem_t cheio, vazio;

// inicializacao do buffer
int buffer[TAMANHOBUFFER];
int quantBytesEscritos = 0;
int quantBytesLidos = 0;

void *consumidor(void *id) {

    while (quantBytesLidos < TAMANHOMIDIA) {
        sem_wait(&cheio);  // down - decremento
        
        // Pegar a posicao do buffer que sera lida
        int posicao = quantBytesLidos % TAMANHOBUFFER;
    
        // Retirar dados do buffer
        pthread_mutex_lock(&mutexBuffer);
        printf("* CONSUMIDOR: Info retirada do buffer na posicao %d: %d\n\n", posicao, buffer[posicao]);
        pthread_mutex_unlock(&mutexBuffer);
        
        sleep(1);     // va dormir por 1 segundo
        quantBytesLidos++;
        sem_post(&vazio);  // up - incremento
    }    
    pthread_exit(NULL);
}



void *produtor( void *id ) {
    
    while( quantBytesEscritos < TAMANHOMIDIA ) {
        // se o buffer estiver cheio 
        // aguarda um sinal do consumidor
        sem_wait(&vazio);  // down - decremento
        
        // Pegar a posicao do buffer que sera modificada
        int posicao = quantBytesEscritos % TAMANHOBUFFER;
       
        // Modificar o buffer
        pthread_mutex_lock(&mutexBuffer);
            buffer[posicao] = quantBytesEscritos;
            printf("#   PRODUTOR: Info colocada no buffer na posicao %d: %d\n", posicao, buffer[posicao]);
        pthread_mutex_unlock(&mutexBuffer);
        
        quantBytesEscritos++; 
        sem_post(&cheio);  // up - incremento
    }
    pthread_exit(NULL);
}


int main( int argc, char *argv[] ) {
    pthread_t tConsumidor, tProdutor;

    printf( "Inicializando Semaforos e mutex\n");
    sem_init(&cheio, 0, 0);
    sem_init(&vazio, 0, TAMANHOBUFFER);
    pthread_mutex_init( &mutexBuffer, NULL );
    
    printf( "Criando thread produtor\n");
    int rc = pthread_create(&tProdutor, NULL, produtor, NULL);
    printf( "Criando thread consumidor\n");
    rc = pthread_create(&tConsumidor, NULL, consumidor, NULL);

    // aguarda todos os threads terminarem
    pthread_join(tProdutor, NULL);
    pthread_join(tConsumidor, NULL);

    printf("Processamento terminado: %d - %d\n", quantBytesEscritos, quantBytesLidos);

    pthread_mutex_destroy( &mutexBuffer );
    sem_destroy( &cheio );
    sem_destroy( &vazio );
    pthread_exit( NULL );
}


