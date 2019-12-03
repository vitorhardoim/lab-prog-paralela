#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define SIZE 1024


void merge(int vetor[], int comeco, int meio, int fim) {

	int com1 = comeco, com2 = meio+1, comAux = 0, tam = fim-comeco+1;

	int *vetAux;

	vetAux = (int*)malloc(tam * sizeof(int));


	while(com1 <= meio && com2 <= fim){

    	if(vetor[com1] < vetor[com2]) {

        	vetAux[comAux] = vetor[com1];

        	com1++;

    	} else {

        	vetAux[comAux] = vetor[com2];

        	com2++;

    	}

    	comAux++;

	}


	while(com1 <= meio){

    	vetAux[comAux] = vetor[com1];

    	comAux++;

    	com1++;

	}


	while(com2 <= fim) {

    	vetAux[comAux] = vetor[com2];

    	comAux++;

    	com2++;

	}


	for(comAux = comeco; comAux <= fim; comAux++){

    	vetor[comAux] = vetAux[comAux-comeco];

	}

    

	free(vetAux);

}


void mergeSort(int vetor[], int comeco, int fim){

	if (comeco < fim) {

    	int meio = (fim+comeco)/2;


    	mergeSort(vetor, comeco, meio);

    	mergeSort(vetor, meio+1, fim);

    	merge(vetor, comeco, meio, fim);

	}

}

int array[SIZE];

int main(int argc, char **argv){
	int meu_rank, np, tam;
    	tam = SIZE/np;

    	int parte[tam];
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    
	if(meu_rank == 0){
		for(int i = 0; i < 1024; i++){
       			array[i] = random()%1024;
		}
	}

	MPI_Scatter(array, tam, MPI_INT, parte, tam, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	mergeSort(parte, 0, tam-1);
	
	MPI_Gather(parte, tam, MPI_INT, array, tam, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	if(meu_rank == 0) {
		
		mergeSort(array, 0, 1023);
		
		printf("Vetor ordenado: \n");
		for(int j = 0; j < 1024; j++) {
			
			printf("%d ", array[j]);
			
		}
			
		printf("\n");
		printf("\n");
			
			
	}
	

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();	 
		
}

