all: repl
repl: repl.o klack.o 
	gcc -std=c99 -Wall repl.o klack.o -lreadline -o repl
klack.o: klack.c klack.h
	gcc -c -std=c99 -Wall klack.c -o klack.o
repl.o: repl.c klack.h
	gcc -c -std=c99 -Wall repl.c -o repl.o
clean:
	rm repl.o klack.o
