#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

void trocaCrescente(int index1, int index2, int *array) //Troca 2 valores de posição para ficarem em ordem crescente no array.
{
    if (array[index2] < array[index1])
    {
        int temp = array[index2];
        array[index2] = array[index1];
        array[index1] = temp;
    }
}
void trocaDecrescente(int index1, int index2, int *array) //Troca 2 valores de posição para ficarem em ordem crescente no array.
{
    if (array[index1] < array[index2])
    {
        int temp = array[index2];
        array[index2] = array[index1];
        array[index1] = temp;
    }
}

void bitonicSortFromBitonicSequence(int startIndex, int lastIndex, int dir, int *ar) // Cria um array crescente ou decrescente dependendo da direção de entrada.
{
    if (dir == 1) // Ordenação crescente
    {
        int cont = 0; //contador para sabermos quais elementos já foram trocados de posição
        int n_elements = lastIndex - startIndex + 1;
        for (int j = n_elements / 2; j > 0; j /= 2)
        {
            cont = 0;
            for (int i = startIndex; i + j <= lastIndex; i++)
            {
                if (cont < j)
                {
                    trocaCrescente(i, i + j, ar);
                    cont++;
                }
                else
                {
                    cont = 0;
                    i += j - 1;
                }
            }
        }
    }
    else // Ordenação decrescente
    {
        int cont = 0;
        int n_elements = lastIndex - startIndex + 1;
        for (int j = n_elements / 2; j > 0; j /= 2)
        {
            cont = 0;
            for (int i = startIndex; i <= (lastIndex - j); i++)
            {
                if (cont < j)
                {
                    trocaDecrescente(i, i + j, ar);
                    cont++;
                }
                else
                {
                    cont = 0;
                    i += j - 1;
                }
            }
        }
    }
}

void bitonicSequenceGenerator(int startIndex, int lastIndex, int *array) // Cria uma sequência bitonica, ou seja, a primeira metade crescente e a segunda decrescente
{
    int n_elements = lastIndex - startIndex + 1;
    for (int j = 2; j <= n_elements; j *= 2)
    {
        #pragma omp parallel for num_threads(4)
        for (int i = 0; i < n_elements; i += j)
        {
            if (((i / j) % 2) == 0)
            {
                bitonicSortFromBitonicSequence(i, i + j - 1, 1, array);
            }
            else
            {
                bitonicSortFromBitonicSequence(i, i + j - 1, 0, array);
            }
        }
    }
}

int main()
{
    int size = 1024;
    int array[size];
    srand(time(NULL));
    //gera o vetor aleatório
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % size;
    }
    
    //algoritmo de ordenação
    bitonicSequenceGenerator(0, size - 1, array);

    //printa o vetor de saida
    for(int i = 0; i < size; i++)
    {
        printf("[%d]:%d ", i, array[i]);
    }
}
