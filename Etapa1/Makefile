etapa1: lex.yy.o hash.o main.o 
	gcc lex.yy.o hash.o main.o -o etapa1

main.o: main.c
	gcc -c main.c

hash.o: hash.c
	gcc -c hash.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l

clean:
	rm *.o etapa1 lex.yy.c lex.yy.h
