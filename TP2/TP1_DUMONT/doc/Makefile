.PHONY: all clean install
all:
	pod2man --utf8 -r 0.1 -c "Game" -s 6 warcraft.pod > warcraft.6.gz
	pod2man --utf8 -r 0.1 -c "Graphic" -s 6 graphic.pod > graphic.6.gz

clean:
	rm warcraft.6.gz
	rm graphic.6.gz

install:
	install -m 755 -D warcraft.pod /tmp/root/usr/share/man/man6
	install -m 755 -D graphic.pod 
