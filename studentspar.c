#include "studentsFunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 6

int main()
{
    int R, C, A, seed;

    scanf("%d", &R);
    scanf("%d", &C);
    scanf("%d", &A);
    scanf("%d", &seed);

    printf("Rodando\n");
    srand(seed);

    float **resultadosCidades = criaResultados(R * C);
    assert(resultadosCidades);
    float **resultadosRegioes = criaResultados(R);
    assert(resultadosRegioes);
    float *resultadosGerais = (float*)calloc(N_METRICAS, sizeof(float));
    assert(resultadosGerais);

    int ***M = criaMatriz(R, C, A);
    //imprimeMatriz(R, C, A, M);

    int **bucketsCidades = (int **)calloc(R * C, sizeof(int *));
    assert(bucketsCidades);
    
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++){
            bucketsCidades[j + (C * i)] = (int *)calloc(LIM_NOTAS + 1, sizeof(int));
            assert(bucketsCidades[j + (C * i)]);
        }
    }

    int **bucketsRegioes = (int **)calloc(R, sizeof(int *));
    assert(bucketsRegioes);

    for (int i = 0; i < R; i++){
        bucketsRegioes[i] = (int *)calloc(LIM_NOTAS + 1, sizeof(int));
        assert(bucketsRegioes[i]);
    }

    int *bucketGeral = (int *)calloc(LIM_NOTAS + 1, sizeof(int));

    clock_t start = clock();
    int n_threads = NUM_THREADS;
    omp_set_num_threads(n_threads);
    // #pragma omp parallel for
    // clock_t t1 = clock();

    // #pragma omp parallel for
    for (int i = 0; i < R; i++)
    {
        // #pragma omp for
        for (int j = 0; j < C; j++)
        {
            #pragma omp parallel
            {
                int *bucketTemp = (int *)calloc(LIM_NOTAS + 1, sizeof(int));
                // clock_t tTemp1 = clock();
                int k =0;
                #pragma omp for simd
                for (k = 0; k < A; k++){
                    // printf("%d\n", omp_get_thread_num());
                    bucketTemp[M[i][j][k]]++;
                }
                // clock_t tTemp2 = clock();
                // printf("thread:%d\ntempo contagem cidade: %f\n", omp_get_thread_num(), (float)(tTemp2-tTemp1) / CLOCKS_PER_SEC);

                // #pragma omp for reduction(+: bucketsCidades[i*C+j][:LIM_NOTAS + 1])
                for (int k = 0; k < LIM_NOTAS + 1; k++)
                {
                    bucketsCidades[(i * C) + j][k] += bucketTemp[k];
                }
                free(bucketTemp);
            }
            
            resultadosCidades[j + (i * C)][0] = obterMin(bucketsCidades[j + (i * C)]);
            resultadosCidades[j + (i * C)][1] = obterMax(bucketsCidades[j + (i * C)]);
            resultadosCidades[j + (i * C)][2] = obterMediana(A, bucketsCidades[j + (i * C)]);
            resultadosCidades[j + (i * C)][3] = obterMedia(A, bucketsCidades[j + (i * C)]);
            resultadosCidades[j + (i * C)][4] = obterDesvioPadrao(A, resultadosCidades[j + (i * C)][3], bucketsCidades[j + (i * C)]);
        }
    }
    // clock_t t2 = clock();

    // printf("tempo contagem das notas: %f\n", (float)(t2-t1) / CLOCKS_PER_SEC);
    for (int i = 0; i < R; i++)
    {
        // reduzir os buckets cidade pros buckets regiao
        for (int j = 0; j < C; j++)
        {
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
    
    for (int i = 0; i < R; i++)
    {
        // reduzir os buckets regiao pro bucket geral
        for (int j = 0; j < LIM_NOTAS + 1; j++)
        {
            bucketGeral[j] += bucketsRegioes[i][j];
        }
    }

    // calcular os resultados da regiao      
    resultadosGerais[0] = obterMin(bucketGeral);
    resultadosGerais[1] = obterMax(bucketGeral);
    resultadosGerais[2] = obterMediana(R*C*A, bucketGeral);
    resultadosGerais[3] = obterMedia(R*C*A, bucketGeral);
    resultadosGerais[4] = obterDesvioPadrao(A * R * C, resultadosGerais[3], bucketGeral);
    
    clock_t end = clock();

    for (int i = 0; i < R * C; i++) free(bucketsCidades[i]);
    for (int i = 0; i < R; i++) free(bucketsRegioes[i]);

    free(bucketsCidades);
    free(bucketsRegioes);
    free(bucketGeral);

    imprimeResultados(R, C, A, resultadosCidades, resultadosRegioes, resultadosGerais);

    limpaResultados(R*C, (void**)resultadosCidades);
    limpaResultados(R, (void**)resultadosRegioes);

    free(resultadosGerais);
    limpaMatriz(R, C, A, (void***)M);
    
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo em Segundos: %lf\n", seconds);
    
    return 0;
}