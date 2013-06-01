CC=gcc
CFLAGS= -Wall -fPIC -DPIC -g -O2
LDFLAGS= -shared 

all:playweibo.so

%.o : %.c
	$(CC) $(CFLAGS) -c $<

playweibo.so: playweibo.o parson.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f playweibo.so playweibo.o parson.o
