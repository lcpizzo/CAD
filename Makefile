num=1
ver=par
all:
	gcc -o studentspar studentspar.c studentsFunc.c -lm -fopenmp -g
	./studentspar < 1.in > 1par.out

#Uso: make run ver=seq|par num=1|2|3
run:
	gcc -o students$(ver) students$(ver).c studentsFunc.c -lm -fopenmp -g
	./students$(ver) < $(num).in > $(num)$(ver).out

echo:
	gcc -o students$(ver) students$(ver).c studentsFunc.c -lm -fopenmp -g
	./students$(ver) < $(num).in