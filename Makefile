CC = gcc
CFLAGS = -Wall -g

build: tema3

tema3: main.o c1.o c2.o
	$(CC) -o tema3 $^ 

main.o: main.c
	$(CC) $(CFLAGS) -c $^

c1.o: c1.c
	$(CC) $(CFLAGS) -c $^

c2.o: c2.c
	$(CC) $(CFLAGS) -c $^

run: build
	./tema3

clean:
	rm -f *.o tema3