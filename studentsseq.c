#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIM_NOTAS 10
#define N_METRICAS 5

int*** criaMatriz(int R, int C, int A);
void limpaMatriz(int R, int C, int A, void ***matriz);
void imprimeMatriz(int R, int C, int A, int ***matriz);
int* criaBucket(int size, int *array);
void limpaArray(void *array);
float **criaResultados(int size);
void limpaResultados(int size, void **resultados);
void verificaAlocacao(void* p);
float obterMin(int *array);
float obterMax(int *array);
float obterMediana(int size, int *array);
float obterMedia(int size, int *array);



int main(int argc, char const *argv[])
{
    int R, C, A, seed;
    R = C = A = seed = 0;

    scanf("%d", &R);
    scanf("%d", &C);
    scanf("%d", &A);
    scanf("%d", &seed);

    srand(seed);
    float **resultadosCidades = criaResultados(R*C);
    verificaAlocacao(resultadosCidades);
    float **resultadosRegioes = criaResultados(R);
    verificaAlocacao(resultadosRegioes);

    int ***M = criaMatriz(R, C, A);
    verificaAlocacao(M);
    imprimeMatriz(R, C, A, M);

    // calculando métricas por cidades
    int **bucketsCidades=(int**)calloc(C, sizeof(int*));
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            bucketsCidades[j] = criaBucket(A, M[i][j]);

            printf("bucket[%d][%d]\n", j, j+C*i);
            for (int k = 0; k < LIM_NOTAS+1; k++)
            {
                printf("%d", bucketsCidades[j][k]);
                if(k != LIM_NOTAS+1)printf(" ");
            }printf("\n");
            

            resultadosCidades[j + C*i][0] = obterMin(bucketsCidades[j]);
            resultadosCidades[j + C*i][1] = obterMax(bucketsCidades[j]);
            resultadosCidades[j + C*i][2] = obterMediana(A, bucketsCidades[j]);
            resultadosCidades[j + C*i][3] = obterMedia(A, bucketsCidades[j]);

            printf("min %.2f\n", resultadosCidades[j + C*i][0]);
            printf("max %.2f\n", resultadosCidades[j + C*i][1]);
            printf("mediana %.2f\n", resultadosCidades[j + C*i][2]);
            printf("media %.2f\n", resultadosCidades[j + C*i][3]);

        }   
        printf("\n");
    }


    for (int i = 0; i < C; i++) {free(bucketsCidades[i]); };
    free(bucketsCidades);
    

    limpaResultados(R*C, (void**)resultadosCidades);
    limpaResultados(R, (void**)resultadosRegioes);
    limpaMatriz(R, C, A, (void***)M);


    return 0;
}




int*** criaMatriz(int R, int C, int A) 
{
    int ***matriz = (int***)calloc(R, sizeof(int**));
    if (!matriz)
    {// não conseguiu alocar memória
        return NULL;
    }

    int upper = LIM_NOTAS; int lower = 0;
    for (int i = 0; i < R; i++)
    {
        matriz[i] = (int**)calloc(C, sizeof(int*));
        if (!matriz[i])
        {// não conseguiu alocar memória
            return NULL;
        }
        for (int j = 0; j < C; j++)
        {
            matriz[i][j] = (int*)calloc(A, sizeof(int));
            if (!matriz[i][j])
            {// não conseguiu alocar memória
                return NULL;
            }
            for (int k = 0; k < A; k++)
            {// gerando números aleatorios no intervalo [0,LIM_NOTAS]
                matriz[i][j][k] = (rand() %  (upper - lower + 1)) + lower;                 
            }       
        }   
    }
    return matriz;
}

void limpaMatriz(int R, int C, int A, void ***matriz) 
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            free(matriz[i][j]); matriz[i][j] = NULL;       
        }   
        free(matriz[i]); matriz[i]=NULL;
    }
    free(matriz); matriz=NULL;
}


void imprimeMatriz(int R, int C, int A, int ***matriz)
{
    for (int i = 0; i < R; i++)
    {
        printf("R : %d\n", i);
        for (int j = 0; j < C; j++)
        {
            printf("C : %d |", j);
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
    int *bucket = (int*)calloc(LIM_NOTAS+1, sizeof(int));
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

void limpaArray(void *array) {free(array); array = NULL;}

float **criaResultados(int size)
{
    float **resultados = (float**)calloc(size, sizeof(float*));
    if (!resultados) 
    {// não conseguiu alocar memória
        return NULL;
    }
    for (int i = 0; i < size; i++)
    {
        resultados[i] = (float*)calloc(N_METRICAS, sizeof(float));
        if (!resultados[i]) 
        {// não conseguiu alocar memória
            return NULL;
        }
    }
    return resultados;
}

void limpaResultados(int size, void **resultados)
{
    for (int i = 0; i < size; i++)
    {
        free(resultados[i]); resultados[i]=NULL;
    }
    free(resultados); resultados=NULL;   
}

void verificaAlocacao(void* p)
{
    if (!p)
    {
        printf("\nMemoria Nao Alocada!!\n");
        exit(0);
    }
    
}


float obterMin(int *array)
{// primeiro elemento com alguma frequência
// percorrendo o vetor da esquerda para a direita
    for (int i = 0; i < LIM_NOTAS+1; i++)
    {
        if (array[i]>0) return (float)i;
    }
    return 0;
}

float obterMax(int *array)
{// primeiro elemento com alguma frequência
// percorrendo o vetor da direita para a esquerda
    for (int i = LIM_NOTAS; i >=0; i--)
    {
        if (array[i]>0) return (float)i;
    }
    return 0;
}

float obterMediana(int size, int *array)
{
    int posicaoMediana = size/2;
    for (int i = 0; i < LIM_NOTAS; i++)
    {
        posicaoMediana -= array[i];
        if (posicaoMediana <= 0)
        {// caso numero par de elementos
            if (size %2 ==0)
            {
                int x = i;
                int y=0;
                for (int j = i+1; j < LIM_NOTAS; j++)
                {
                    if (array[j]>0) { y = j; j = LIM_NOTAS; }
                }
                return (x+y)/(float)2;
                
            } else
            {// numero impar de elementos
                for (int j = i+1; j < LIM_NOTAS; j++){ if (array[j]>0) {return j;} }
            }   
        }
    }
    return 0;
}



float obterMedia(int size, int *array)
{
    int soma = 0;
    for (int i = 0; i < LIM_NOTAS+1; i++)
    {
        soma += array[i]*i;
    }
    return soma/(float)size;
}
