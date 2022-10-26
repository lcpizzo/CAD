all:
	gcc -o studentspar studentspar.c studentsFunc.c -lm -fopenmp -g
	./studentspar < 1.in > 1par.out