etapa3: y.tab.o lex.yy.o hash.o ast.o main.o 
	gcc lex.yy.o hash.o ast.o main.o y.tab.o -o etapa3 -Wall

main.o: main.c
	gcc -c main.c -Wall

hash.o: hash.c
	gcc -c hash.c -Wall

y.tab.o: y.tab.c
	gcc -c y.tab.c -Wall

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c -Wall

y.tab.c: parser.y lex.yy.c
	yacc -d parser.y

lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l

ast.o: ast.c
	gcc -c ast.c -Wall

clean:
	rm *.o etapa3 lex.yy.c lex.yy.h y.tab.c y.tab.h
