#include "studentsseq.h"
#include <stdio.h> 
#include <stdlib.h>
#include <omp.h>

#define LIM_NOTAS 100
#define N_METRICAS 5

int* criaBucketPar(int size, int *array) 
{
    int *bucket = (int*)calloc(LIM_NOTAS+1, sizeof(int));
    // não conseguiu alocar memória
    assert(bucket);    
    // divide size por numero de threads e cada thread 
    for (int i = 0; i < size; i++)
        bucket[array[i]]++;    
    return bucket;
}


int main(){
    int R, C, A, seed;

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

    // Lógica paralela aqui
    int n_threads = 8;
    omp_set_num_threads(n_threads);
    #pragma omp parallel
    {
        int **bucketTemp = (int**) calloc(n_threads, sizeof(int*));
        // separar a matriz de alunos em partes para ser trabalhada por diversas threads
        for (int i = 0; i < R; i++)
        {
            #pragma omp parallel for shared(j) \ num_threads(n_threads)
            for(int j = 0; j < C; j++){
                // trabalha em cima dos buckets temp
                bucketTemp[j] = criaBucket(A, M[i][j]);
            }

            // redução dos buckets pro bucket geral
            for(int k=0;k<C;k++){
                #pragma omp parallel for reduction (+: bucketsCidades[k])
                for(int w=0;w<C; w++){
                    bucketsCidades[k] += bucketTemp[w][k];
                }
                #pragma omp parallel for
                for(int j=0; j<C;j++){                                    
                    resultadosCidades[j + i*C][0] = obterMin(bucketsCidades[j + i*C]);
                    resultadosCidades[j + i*C][1] = obterMax(bucketsCidades[j + i*C]);
                    resultadosCidades[j + i*C][2] = obterMediana(A, bucketsCidades[j + i*C]);
                    resultadosCidades[j + i*C][3] = obterMedia(A, bucketsCidades[j + i*C]);
                    resultadosCidades[j + i*C][4] = obterDesvioPadrao(A, resultadosCidades[j + i*C][3], bucketsCidades[j + i*C]);
                }
            }
        }        
        //free no bucket temp
    }

    #pragma         

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