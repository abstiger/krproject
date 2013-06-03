CC=gcc
CFLAGS= -Wall -fPIC -DPIC -g -O2
LDFLAGS= -shared 
PYFLAGS=`pkg-config --cflags --libs python`

all:build

%.o:%.c
	$(CC) $(CFLAGS) $(PYFLAGS) -c $<

playweibo_krdb.so: playweibo_krdb.o parson.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

playweibo_rule.so: playweibo_rule.o 
	$(CC) $(CFLAGS) $(PYFLAGS) $(LDFLAGS) $^ -o $@

build:playweibo_krdb.so playweibo_rule.so
	@echo "finished building!"

install:build
	@mv -f playweibo_krdb.so $(KRHOME)/lib
	@mv -f playweibo_rule.so $(KRHOME)/lib
	@echo "finished installing!"

uninstall:
	@rm -f $(KRHOME)/lib/playweibo_krdb.so
	@rm -f $(KRHOME)/lib/playweibo_rule.so
	@echo "finished uninstalling!"

clean:
	@rm -f playweibo_*.so playweibo_*.o parson.o 
	@echo "finished cleaning!"
