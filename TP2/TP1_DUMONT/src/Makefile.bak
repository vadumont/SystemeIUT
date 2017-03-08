.PHONY: all depend clean install
all: editeur warcraft
depend: 
	makedepend -Y *.c

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

#Editions de lien

warcraft: warcraft.o video.o perso.o

editeur: editeur.o video.o perso.o

clean:
	rm -f *.o editeur warcraft

install:
	install -m 755 -D editeur /tmp/root/usr/bin/editeur
	install -m 755 -D warcraft /tmp/root/usr/bin/warcraft

# DO NOT DELETE

editeur.o: perso.h video.h editeur.h
perso.o: perso.h video.h
video.o: video.h
warcraft.o: video.h perso.h warcraft.h
