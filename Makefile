all:
	gcc -o studentspar studentspar.c studentsFunc.c -lm -fopenmp -g
	./studentspar