CC=gcc
CFLAGS= -Wall -fPIC -DPIC -g -O2
LDFLAGS= -shared 

all:antifraud.so

%.o : %.c
	$(CC) $(CFLAGS) -c $<

antifraud.so: antifraud.o parson.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f antifraud.so antifraud.o parson.o
