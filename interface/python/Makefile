all: install

install:
	sudo python setup.py install --record install.log
	
uninstall:
	cat install.log | sudo xargs rm -rf 

clean:
	rm -rf build
	rm -f *.pyc
	rm -f install.log

