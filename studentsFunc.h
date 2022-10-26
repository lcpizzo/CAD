#ifndef _STUDENTS_FUNC_H_
#define _STUDENTS_FUNC_H_

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
void imprimeResultados(int R, int C, int A, float **resultadosCidades, float **resultadosRegioes, float *resultadosGerais);

int* criaBucketPar(int size, int *array); 
void obterResultadosCidadesPar(int R, int C, int A, int ***M, int **bucketsCidades, float **resultadosCidades);

#endif