#include "studentsFunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    int R, C, A, seed;

    scanf("%d", &R);
    scanf("%d", &C);
    scanf("%d", &A);
    scanf("%d", &seed);

    //printf("Rodando\n");
    srand(seed);

    float **resultadosCidades = criaResultados(R * C);
    assert(resultadosCidades);
    float **resultadosRegioes = criaResultados(R);
    assert(resultadosRegioes);
    float *resultadosGerais = (float*)calloc(N_METRICAS, sizeof(float));
    assert(resultadosGerais);

    int ***M = criaMatriz(R, C, A);
    //imprimeMatriz(R, C, A, M);

    clock_t start = clock();
    int **bucketsCidades = (int **)calloc(R * C, sizeof(int *));
    assert(bucketsCidades);
    
    #pragma omp parallel for
    for (int i = 0; i < R; i++)
    {
        #pragma for
        for (int j = 0; j < C; j++)
        {
            bucketsCidades[j + (C * i)] = (int *)calloc(LIM_NOTAS + 1, sizeof(int));
            assert(bucketsCidades[j + (C * i)]);
        }
    }

    int **bucketsRegioes = (int **)calloc(R, sizeof(int *));
    assert(bucketsRegioes);

    #pragma omp parallel for
    for (int i = 0; i < R; i++)
    {
        bucketsRegioes[i] = (int *)calloc(LIM_NOTAS + 1, sizeof(int));
        assert(bucketsRegioes[i]);
    }

    int *bucketGeral = (int *)calloc(LIM_NOTAS + 1, sizeof(int));

    int n_threads = 8;
    omp_set_num_threads(n_threads);
    #pragma omp parallel for
    for (int i = 0; i < R; i++)
    {
        #pragma for
        for (int j = 0; j < C; j++)
        {
            int *bucketTemp = (int *)calloc(LIM_NOTAS + 1, sizeof(int));
            
            #pragma for critical
            for (int k = 0; k < A; k++){
                bucketTemp[M[i][j][k]]++;
            }
            //#pragma omp for reduction(+: bucketsCidades[0][:LIM_NOTAS + 1])
            for (int k = 0; k < LIM_NOTAS + 1; k++)
            {
                bucketsCidades[(i * C) + j][k] += bucketTemp[k];
            }

            resultadosCidades[j + (i * C)][0] = obterMin(bucketsCidades[j + (i * C)]);
            resultadosCidades[j + (i * C)][1] = obterMax(bucketsCidades[j + (i * C)]);
            resultadosCidades[j + (i * C)][2] = obterMediana(A, bucketsCidades[j + (i * C)]);
            resultadosCidades[j + (i * C)][3] = obterMedia(A, bucketsCidades[j + (i * C)]);
            resultadosCidades[j + (i * C)][4] = obterDesvioPadrao(A, resultadosCidades[j + (i * C)][3], bucketsCidades[j + (i * C)]);

            free(bucketTemp);
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < R; i++)
    {
        // reduzir os buckets cidade pros buckets regiao
        #pragma for
        for (int j = 0; j < C; j++)
        {
            //#pragma omp for reduction (+:bucketsRegioes[i][:LIM_NOTAS + 1])
            for (int k = 0; k < LIM_NOTAS + 1; k++)
            {
                bucketsRegioes[i][k] += bucketsCidades[(i * C) + j][k];
            }
        }   
        
        // calcular os resultados da regiao
        resultadosRegioes[i][0] = obterMin(bucketsRegioes[i]);
        resultadosRegioes[i][1] = obterMax(bucketsRegioes[i]);
        resultadosRegioes[i][2] = obterMediana(A * C, bucketsRegioes[i]);
        resultadosRegioes[i][3] = obterMedia(A * C, bucketsRegioes[i]);
        resultadosRegioes[i][4] = obterDesvioPadrao(A * C, resultadosRegioes[i][3], bucketsRegioes[i]);
    }
    
    //#pragma omp parallel
    //{
        #pragma omp for reduction (+: bucketGeral[:LIM_NOTAS + 1])
        for (int i = 0; i < R; i++)
        {
        // reduzir os buckets regiao pro bucket geral
            for (int j = 0; j < LIM_NOTAS + 1; j++)
            {
                bucketGeral[j] += bucketsRegioes[i][j];
            }
        }
    // }

    // calcular os resultados da regiao      
    #pragma omp parallel
    { 
        resultadosGerais[0] = obterMin(bucketGeral);
        resultadosGerais[1] = obterMax(bucketGeral);
        resultadosGerais[2] = obterMediana(R*C*A, bucketGeral);
        resultadosGerais[3] = obterMedia(R*C*A, bucketGeral);
        resultadosGerais[4] = obterDesvioPadrao(A * R * C, resultadosGerais[3], bucketGeral);
    }
    
    clock_t end = clock();

    for (int i = 0; i < R * C; i++) free(bucketsCidades[i]);
    for (int i = 0; i < R; i++) free(bucketsRegioes[i]);

    free(bucketsCidades);
    free(bucketsRegioes);
    free(bucketGeral);

    imprimeResultados(R, C, A, resultadosCidades, resultadosRegioes, resultadosGerais);

    limpaResultados(R*C, (void**)resultadosCidades);
    limpaResultados(R, (void**)resultadosRegioes);
    //limpaResultados(N_METRICAS, (void*)resultadosGerais);
    free(resultadosGerais);
    limpaMatriz(R, C, A, (void***)M);
    
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo em Segundos: %lf\n", seconds);
    
    return 0;
}