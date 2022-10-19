#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIM_NOTAS 100
#define N_METRICAS 5

int*** criaMatriz(int R, int C, int A);
void limpaMatriz(int R, int C, int A, int ***matriz);
void imprimeMatriz(int R, int C, int A, int ***matriz);
int* criaBucket(int size, int *array);
void liberaArray(int *array);
int **criaResultados(int size);
void liberaResultados(int size, int **resultados);


int*** criaMatriz(int R, int C, int A) 
{
    int ***matriz = (int***)malloc(R*sizeof(int**));
    if (!matriz)
    {// não conseguiu alocar memória
        return NULL;
    }
    for (int i = 0; i < R; i++)
    {
        matriz[i] = (int**)malloc(C*sizeof(int*));
        if (!matriz[i])
        {// não conseguiu alocar memória
            return NULL;
        }
        for (int j = 0; j < C; j++)
        {
            matriz[i][j] = (int*)malloc(A*sizeof(int));
            if (!matriz[i][j])
            {// não conseguiu alocar memória
                return NULL;
            }
            for (int k = 0; k < A; k++)
            {
                matriz[i][j][k] = rand() % LIM_NOTAS; // gerando números aleatorios no intervalo [0,LIM_NOTAS]                
            }       
        }   
    }
    return matriz;
}

void limpaMatriz(int R, int C, int A, int ***matriz) 
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            free(matriz[i][j]);       
        }   
        free(matriz[i]);
    }
    free(matriz);
}


void imprimeMatriz(int R, int C, int A, int ***matriz)
{
    for (int i = 0; i < R; i++)
    {
        printf("R : %d\n", i);
        for (int j = 0; j < C; j++)
        {
            printf("C : %d ", j);
            for (int k = 0; k < A; k++)
            {
                printf("%d", matriz[i][j][k]);                
                if( k != A-1) printf(" ");    
            }
            printf("\n");
        }   
        printf("\n");
    }
}


int* criaBucket(int size, int *array) 
{
    int *bucket = (int*)malloc(LIM_NOTAS*sizeof(int));
    if (!bucket) 
    {// não conseguiu alocar memória
        return NULL;
    }
    
    for (int i = 0; i < size; i++)
    {
        bucket[array[i]]++;
    }
    
    return bucket;
}

void liberaArray(int *array) {free(array);}


int **criaResultados(int size)
{
    int **resultados = (int**)malloc(size*sizeof(int*));
    if (!resultados) 
    {// não conseguiu alocar memória
        return NULL;
    }
    for (int i = 0; i < size; i++)
    {
        resultados[i] = (int*)malloc(N_METRICAS*sizeof(int));
        if (!resultados[i]) 
        {// não conseguiu alocar memória
            return NULL;
        }
    }
    
}

void liberaResultados(int size, int **resultados)
{
    for (int i = 0; i < size; i++)
    {
        free(resultados[i]);
    }
    free(resultados);    
}


int main(int argc, char const *argv[])
{
    int R, C, A, seed;
    R = C = A = seed = 0;

    scanf("%d", &R);
    scanf("%d", &C);
    scanf("%d", &A);
    scanf("%d", &seed);

    srand(seed);
    int ***M = criaMatriz(R, C, A);
    imprimeMatriz(R, C, A, M);
    


    
    limpaMatriz(R, C, A, M);


    return 0;
}
