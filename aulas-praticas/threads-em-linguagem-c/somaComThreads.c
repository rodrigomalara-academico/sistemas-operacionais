#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h> 

#define NUM_THREADS 8
#define TAM_VETOR 1000000

int elem[TAM_VETOR];
long somaTotal;
pthread_mutex_t mut;

void *SomaElementos(void *id){
	int i = 0;
	long somaParcial = 0;
	for(i = id; i < TAM_VETOR; i = i + NUM_THREADS)
		somaParcial = somaParcial + elem[i];
	
	//pthread_mutex_lock(&mut);
	somaTotal = somaTotal + somaParcial;
	//pthread_mutex_unlock(&mut);
	
	printf("Soma parcial %d - thread %d\n", somaParcial, id);
	
	pthread_exit(NULL);
}

void main (int argc, char *argv[])
 {
 	pthread_t threads[NUM_THREADS];
	int rc, t, i;
	clock_t tempoIni, tempoFim;
	tempoIni = clock();
	
 	for(i = 0; i < TAM_VETOR; i++){
 		//elem[i] = rand()%100;
 		elem[i] = 1;
 	}
 	pthread_mutex_init(&mut, NULL);
 	
	for (t = 0; t < NUM_THREADS; t++){
		printf("Criando Threads %d \n", t);
		rc = pthread_create(&threads[t], NULL, SomaElementos, (void*) t);
		if (rc){
			printf("ERRO: cod. retorno de prhread_create(): %d\n", rc);
			exit(-1);
		}
	}
	for (t = 0; t < NUM_THREADS; t++){
		rc = pthread_join(threads[t], NULL);
	}
	
	pthread_mutex_destroy(&mut);
	
	printf("Resultado final: %d\n\n",somaTotal);
	tempoFim = clock();
	
	float diff = ((float)(tempoFim - tempoIni) / 1000000.0F ) * 1000;   
    printf("Tempo de execucao: %f ms\n",diff);   

	system("PAUSE");
}

