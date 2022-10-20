#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#define LIM_NOTAS 100
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
float obterDesvioPadrao(int size, float avg, int *array);



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
    float **resultadosGerais = criaResultados(1);
    verificaAlocacao(resultadosGerais);


    int ***M = criaMatriz(R, C, A);
    verificaAlocacao(M);
    imprimeMatriz(R, C, A, M);

    // calculando métricas por cidades
    int **bucketsCidades=(int**)calloc(R*C, sizeof(int*));

    // TODO - modularizar,
    
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            bucketsCidades[j+C*i] = criaBucket(A, M[i][j]);
            resultadosCidades[j + C*i][0] = obterMin(bucketsCidades[j + C*i]);
            resultadosCidades[j + C*i][1] = obterMax(bucketsCidades[j + C*i]);
            resultadosCidades[j + C*i][2] = obterMediana(A, bucketsCidades[j + C*i]);
            resultadosCidades[j + C*i][3] = obterMedia(A, bucketsCidades[j + C*i]);
            resultadosCidades[j + C*i][4] = obterDesvioPadrao(A, resultadosCidades[j + C*i][3], bucketsCidades[j + C*i]);
        }   
    }

    // TODO Modularizar 
    int **bucketsRegioes=(int**)calloc(R, sizeof(int*));
    for (int i = 0; i < R; i++)
    {
        bucketsRegioes[i] = (int*)calloc(LIM_NOTAS+1, sizeof(int));
        for (int j = 0; j < C; j++)
        {
            for (int k = 0; k < LIM_NOTAS+1; k++)
                bucketsRegioes[i][k] += bucketsCidades[j + C*i][k];
        }        
        resultadosRegioes[i][0] = obterMin(bucketsRegioes[i]);
        resultadosRegioes[i][1] = obterMax(bucketsRegioes[i]);
        resultadosRegioes[i][2] = obterMediana(C*A, bucketsRegioes[i]);
        resultadosRegioes[i][3] = obterMedia(C*A, bucketsRegioes[i]);
        resultadosRegioes[i][4] = obterDesvioPadrao(C*A, resultadosRegioes[i][3], bucketsRegioes[i]);
    }
    
    // métricas finais
    int **bucketGeral=(int**)calloc(1, sizeof(int*));
    bucketGeral[0] = (int*)calloc(LIM_NOTAS+1, sizeof(int));
    for (int i = 0; i < R; i++)
    {
        for (int k = 0; k < LIM_NOTAS+1; k++)
            bucketGeral[0][k] += bucketsRegioes[i][k];
    }        
    resultadosGerais[0][0] = obterMin(bucketGeral[0]);
    resultadosGerais[0][1] = obterMax(bucketGeral[0]);
    resultadosGerais[0][2] = obterMediana(R*C*A, bucketGeral[0]);
    resultadosGerais[0][3] = obterMedia(R*C*A, bucketGeral[0]);
    resultadosGerais[0][4] = obterDesvioPadrao(R*C*A, resultadosGerais[0][3], bucketGeral[0]);

    for (int i = 0; i < R*C; i++) {free(bucketsCidades[i]); };
    for (int i = 0; i < R; i++) {free(bucketsRegioes[i]); };
    free(bucketGeral[0]);

    free(bucketsCidades);
    free(bucketsRegioes);
    free(bucketGeral);

    printf("\n");
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
            printf(
                "Reg %d - Cid %d: menor: %.0f, maior: %.0f, mediana %.2f, média: %.2f e DP: %.2f\n",
                i, j, resultadosCidades[j + C*i][0], resultadosCidades[j + C*i][1], 
                resultadosCidades[j + C*i][2], resultadosCidades[j + C*i][3], 
                resultadosCidades[j + C*i][4]
            );        
    }

    printf("\n");
    for (int i = 0; i < R; i++)
        printf(
            "Reg %d: menor: %.0f, maior: %.0f, mediana %.2f, média: %.2f e DP: %.2f\n",
            i, resultadosRegioes[i][0], resultadosRegioes[i][1], 
            resultadosRegioes[i][2], resultadosRegioes[i][3], 
            resultadosRegioes[i][4]
        );

    printf("\n");
    printf(
        "Brasil: menor: %.0f, maior: %.0f, mediana %.2f, média: %.2f e DP: %.2f",
        resultadosGerais[0][0], resultadosGerais[0][1], 
        resultadosGerais[0][2], resultadosGerais[0][3], 
        resultadosCidades[0][4]
    );
    printf("\n");

    limpaResultados(R*C, (void**)resultadosCidades);
    limpaResultados(R, (void**)resultadosRegioes);
    limpaResultados(1, (void**)resultadosGerais);
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
                if(posicaoMediana == 0)
                {
                    int x = i;
                    int y=0;
                    for (int j = i+1; j < LIM_NOTAS; j++)
                    {
                        if (array[j]>0) { y = j; j = LIM_NOTAS; }
                    }
                    return (x+y)/(float)2;
                }
                return i;
            } else
            {// numero impar de elementos
                if (posicaoMediana == 0)
                {
                    for (int j = i+1; j < LIM_NOTAS+1; j++){ if (array[j]>0) {return j;} }                
                }
                return i;
                
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

float obterDesvioPadrao(int size, float avg, int *array)
{
    float soma = 0;
    for (int i = 0; i < LIM_NOTAS+1; i++)
    {
        soma += ((float)i - avg)*((float)i - avg)*array[i];
    }
    return sqrt(soma/size);
}