compiler: parser.c scanner.c token.h tree.h tree.c symtab.h symtab.c semantic.h semantic.c translate.h translate.c
	gcc parser.c scanner.c tree.c symtab.c semantic.c translate.c  -lfl -o compiler

parser.c: parser.y
	bison -v --defines=token.h -o parser.c parser.y

scanner.c: scanner.l
	flex -o scanner.c scanner.l

clean:
	rm scanner.c parser.c token.h compiler