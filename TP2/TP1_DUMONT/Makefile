export CC = gcc
export CFLAGS = -Wall
export LDFLAGS =
export FILEDOC = /tmp/root/usr/share/man/man6
export FILESRC = /tmp/root/usr/bin
.PHONY: clean install depend all tgz

all:
	make -C src depend
	make -C src $@
	make -C doc $@
depend:
	make -C src $@

clean:
	
	make -C src $@
	make -C doc $@
	rm src.tgz

tgz:
	tgz src.tgz src/

install:
	make -C src $@
	make -C doc $@

