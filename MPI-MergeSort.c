#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    int L[n1], R[n2]; 
  
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
    
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    while (i < n1) 
    { 
	    arr[k] = L[i]; 
        i++; 
        k++; 
    } 
    while (j < n2) 
    { 
	    arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 

void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l+(r-l)/2;   
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
        merge(arr, l, m, r); 
    } 
}

void printArray (int arr[], int n) 
{ 
    for (int i = 0; i < n; i++) 
        printf("%d ", arr[i]); 
    printf("\n"); 
}

int main(int argc, char* argv[]){
    int my_rank;
	int np; // número de processos
	int n=1024; // número de elementos
	MPI_Status status;

    MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	int r=n/np;
	if(my_rank == 0){
		
        int vet[n];
     
	    srand(time(NULL));
		//gera o vetor aleatório
		for (int i = 0; i < n; i++)	vet[i] = rand() % n;
		
        for(int i = 1; i<4;i++){
            MPI_Send(vet+(r*i),r,MPI_INT,i,0,MPI_COMM_WORLD);
        }

        int resp[n];
        int aux[r];
        for(int i = 0; i<r; i++) resp[i] = vet[i]; 
        mergeSort(resp,0,r-1);

		printf("Rank: %d\n", my_rank);
		printArray(resp, r);
		printf("\n");

		printf("Merge 0:\n");
		printArray(resp, r);
		printf("\n");

        for(int i = 1; i<4; i++){
            MPI_Recv(aux,r,MPI_INT,i,0,MPI_COMM_WORLD,&status);
            for(int j = 0; j < r; j++) resp[r*i+j] = aux[j];
            merge(resp, 0, r*i, r*i+r-1);
			
			printf("Merge %d: \n", i);
			printArray(resp, r*i + r);
			printf("\n");
        }
		printf("Resultado:\n");
		printArray(resp,n);
    }else{
    	int vet[r];
        MPI_Recv(vet,r,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        mergeSort(vet,0,r-1);

		printf("Rank: %d\n", my_rank);
		printArray(vet, r);
		printf("\n");

        MPI_Send(vet,r,MPI_INT,0,0,MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
