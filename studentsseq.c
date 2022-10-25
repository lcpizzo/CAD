#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "studentsFunc.h"

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

    clock_t start = clock();
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

    free(bucketGeral[0]);

    for (int i = 0; i < R*C; i++) {free(bucketsCidades[i]); };
    for (int i = 0; i < R; i++) {free(bucketsRegioes[i]); };
    

    free(bucketsCidades);
    free(bucketsRegioes);
    free(bucketGeral);
    clock_t end = clock();

    imprimeResultados(R, C, A, resultadosCidades, resultadosRegioes, resultadosGerais);

    limpaResultados(R*C, (void**)resultadosCidades);
    limpaResultados(R, (void**)resultadosRegioes);
    limpaResultados(1, (void**)resultadosGerais);
    limpaMatriz(R, C, A, (void***)M);

    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo em Segundos: %lf\n", seconds);

    return 0;
}