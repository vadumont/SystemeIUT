.PHONY: all depend clean
all: send recv

depend:
	makedepend -y *.c

%.o: %.c
	$(CC) -c $< -o $@

%: %.o
	$(CC) $^ -o $@

clean:
	rm -f *.o send recv
