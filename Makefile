all: repl
repl: repl.o klang.o klang.h
	gcc -std=c99 -Wall repl.o klang.o -lreadline -o repl
klang.o: klang.c
	gcc -c -std=c99 -Wall klang.c -o klang.o
repl.o: repl.c
	gcc -c -std=c99 -Wall repl.c -o repl.o
clean:
	rm repl.o klang.o
