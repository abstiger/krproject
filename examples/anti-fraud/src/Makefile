CC=gcc
CFLAGS= -Wall -g -O2 
LDFLAGS= -fPIC -DPIC -shared -O2

all:antifraud.so

%.o : %.c
	$(CC) $(CFLAGS) -c $<

antifraud.so: antifraud.o parson.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f antifraud.so antifraud.o parson.o
