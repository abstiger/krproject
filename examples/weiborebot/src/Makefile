CC=gcc
CFLAGS= -Wall -fPIC -DPIC -g -O2
LDFLAGS= -shared 

all:weibo.so

%.o : %.c
	$(CC) $(CFLAGS) -c $<

weibo.so: weibo.o parson.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f weibo.so weibo.o parson.o
