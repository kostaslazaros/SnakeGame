snakegame: main.o
	g++ -o snakegame main.o -s

main.o: main.c
	gcc -Wall -O2 -c main.c -o main.o

clean:
	rm -f main.o snakegame
