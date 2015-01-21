CC = gcc

CFLAGS = -Wall -fPIC -g -O2
CFLAGS += `pkg-config --cflags jansson`
CFLAGS += `pkg-config --cflags avro-c`

LDFLAGS = -shared 
LDFLAGS += `pkg-config --libs jansson`
LDFLAGS += `pkg-config --libs avro-c`


all:libkriface.so

%.o : %.c
	$(CC) $(CFLAGS) -c $<

libkriface.so: weixin-trad-flow.o netbank-trad-flow.o pos-atm-trad-flow.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f libkriface.so *.o
