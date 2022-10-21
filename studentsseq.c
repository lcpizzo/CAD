#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
void obterResultadosCidades(int R, int C, int A, int ***M, int **bucketsCidades, float **resultadosCidades);
void calculaMetricas(int index, int n_elem, float **resultados, int **buckets);
void obterResultadosRegioes(int R, int C, int A, float **resultadosRegioes, int ** bucketsCidades, int **bucketsRegioes);
void imprimeResultados(int R, int C, int A, float **resultadosCidades, float **resultadosRegioes, float **resultadosGerais);

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
    assert(resultadosCidades);
    float **resultadosRegioes = criaResultados(R);
    assert(resultadosRegioes);
    float **resultadosGerais = criaResultados(1);
    assert(resultadosGerais);

    int ***M = criaMatriz(R, C, A);
    imprimeMatriz(R, C, A, M);

    int **bucketsCidades=(int**)calloc(R*C, sizeof(int*));
    assert(bucketsCidades);
    obterResultadosCidades(R, C, A, M, bucketsCidades, resultadosCidades);

    int **bucketsRegioes=(int**)calloc(R, sizeof(int*));
    assert(bucketsRegioes);
    obterResultadosRegioes(R, C, A, resultadosRegioes, bucketsCidades, bucketsRegioes);

    int **bucketGeral=(int**)calloc(1, sizeof(int*));
    bucketGeral[0] = (int*)calloc(LIM_NOTAS+1, sizeof(int));
    for (int i = 0; i < R; i++)
    {
        for (int k = 0; k < LIM_NOTAS+1; k++)
            bucketGeral[0][k] += bucketsRegioes[i][k];
    }        
    calculaMetricas(0, R*C*A, resultadosGerais, bucketGeral);

    for (int i = 0; i < R*C; i++) {free(bucketsCidades[i]); };
    for (int i = 0; i < R; i++) {free(bucketsRegioes[i]); };
    free(bucketGeral[0]);

    free(bucketsCidades);
    free(bucketsRegioes);
    free(bucketGeral);

    imprimeResultados(R, C, A, resultadosCidades, resultadosRegioes, resultadosGerais);

    limpaResultados(R*C, (void**)resultadosCidades);
    limpaResultados(R, (void**)resultadosRegioes);
    limpaResultados(1, (void**)resultadosGerais);
    limpaMatriz(R, C, A, (void***)M);

    return 0;
}

int*** criaMatriz(int R, int C, int A) 
{
    int ***matriz = (int***)calloc(R, sizeof(int**));
    // não conseguiu alocar memória
    assert(matriz);
    int upper = LIM_NOTAS; int lower = 0;
    for (int i = 0; i < R; i++)
    {
        matriz[i] = (int**)calloc(C, sizeof(int*));
        // não conseguiu alocar memória
        assert(matriz[i]);
        for (int j = 0; j < C; j++)
        {
            matriz[i][j] = (int*)calloc(A, sizeof(int));
            // não conseguiu alocar memória
            assert(matriz[i][j]);
            // gerando números aleatorios no intervalo [0,LIM_NOTAS]
            for (int k = 0; k < A; k++) 
                matriz[i][j][k] = (rand() %  (upper - lower + 1)) + lower;                 
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
    // não conseguiu alocar memória
    assert(bucket);    
    for (int i = 0; i < size; i++)
        bucket[array[i]]++;    
    return bucket;
}

void limpaArray(void *array) {free(array); array = NULL;}

float **criaResultados(int size)
{
    float **resultados = (float**)calloc(size, sizeof(float*));
    assert(resultados);
    for (int i = 0; i < size; i++)
    {
        resultados[i] = (float*)calloc(N_METRICAS, sizeof(float));
        assert(resultados[i]);
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
                        if (array[j]>0) { y = j; j = LIM_NOTAS; }
                    
                    return (x+y)/(float)2;
                }
                return i;
            } else
            {// numero impar de elementos
                if (posicaoMediana == 0)
                    for (int j = i+1; j < LIM_NOTAS+1; j++){ if (array[j]>0) {return j;} }                
                
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
        soma += array[i]*i;

    return soma/(float)size;
}

float obterDesvioPadrao(int size, float avg, int *array)
{
    float soma = 0;
    for (int i = 0; i < LIM_NOTAS+1; i++)
        soma += ((float)i - avg)*((float)i - avg)*array[i];
    
    return sqrt(soma/size);
}

void obterResultadosCidades(int R, int C, int A, int ***M, int **bucketsCidades, float **resultadosCidades)
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            bucketsCidades[j+C*i] = criaBucket(A, M[i][j]);
            calculaMetricas((j + C*i), A, resultadosCidades, bucketsCidades);
        }   
    }
}

void calculaMetricas(int index, int n_elem, float **resultados, int **buckets)
{
    /*
        index -> indice sobre o qual os valores devem ser armazenados 
        n_elem -> numero de elementos para as metricas que dependem dessa info 
    */
    resultados[index][0] = obterMin(buckets[index]);
    resultados[index][1] = obterMax(buckets[index]);
    resultados[index][2] = obterMediana(n_elem, buckets[index]);
    resultados[index][3] = obterMedia(n_elem, buckets[index]);
    resultados[index][4] = obterDesvioPadrao(n_elem, resultados[index][3], buckets[index]);
}

void obterResultadosRegioes(int R, int C, int A, float **resultadosRegioes, int ** bucketsCidades, int **bucketsRegioes){
    for (int i = 0; i < R; i++)
    {
        bucketsRegioes[i] = (int*)calloc(LIM_NOTAS+1, sizeof(int));
        assert(bucketsRegioes[i]);
        // composicao dos buckets
        for (int j = 0; j < C; j++)
        {
            for (int k = 0; k < LIM_NOTAS+1; k++)
                bucketsRegioes[i][k] += bucketsCidades[j + C*i][k];
        }       
        calculaMetricas(i, C*A, resultadosRegioes, bucketsRegioes);
    }
}

void imprimeResultados(int R, int C, int A, float **resultadosCidades, float **resultadosRegioes, float **resultadosGerais) 
{
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
}