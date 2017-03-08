.PHONY: all depend clean install
FILELIB = /tmp/graphic/usr/lib/
FILEGRAPH = /tmp/graphic/usr/include/
all: tgz editeur warcraft test libgraphic.a
depend: 
	makedepend -Y *.c

graphic.o: graphic.c
	$(CC) $(CFLAGS) -I $(FILEGRAPH) -c $< -o $@

test.o: 
	$(CC) -c -I $(FILEGRAPH) test.c -o test.o
	

tgz:
	tar xzvf graphic.tgz -C /tmp/	


%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@



#Editions de lien

warcraft: warcraft.o video.o perso.o

editeur: editeur.o video.o perso.o

test: test.o
	$(CC) -lpthread -lrt -lX11 -L $(FILELIB) -lgraphic test.o -o $@

libgraphic.a: graphic.o
	ar -r libgraphic.a graphic.o

clean:
	rm -f *.o editeur warcraft
	rm libgraphic.a

install:
	install -m 755 -D editeur /tmp/root/usr/bin/editeur
	install -m 755 -D warcraft /tmp/root/usr/bin/warcraft

# DO NOT DELETE

editeur.o: perso.h video.h editeur.h
graphic.o: graphic.h
perso.o: perso.h video.h
video.o: video.h
warcraft.o: video.h perso.h warcraft.h
