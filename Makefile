all:
	gcc -o studentspar studentspar.c studentsFunc.c -lm
	./studentspar < 1.in > 1par.out