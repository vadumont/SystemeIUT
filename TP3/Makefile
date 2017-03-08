CC = gcc
CFLAGS = -Wall
LDFLAGS =
FILELIB = /tmp/graphic/usr/lib/
FILEGRAPH = /tmp/graphic/usr/include/


all:
	gcc -c -I $(FILEGRAPH) test.c -o test.o
	gcc -lpthread -lrt -lX11 -L $(FILELIB) -lgraphic test.o -o test

tgz:
	
	tar xzvf graphic.tgz -C /tmp/ 

clean:
	rm -f *.o test

