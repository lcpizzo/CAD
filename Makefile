all:
	gcc studentspar.c -o studentspar -lm -fopenmp
	./studentspar < 2.in > 2par.out