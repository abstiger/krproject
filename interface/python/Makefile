all: install

install:
	sudo python setup.py install --record install.log
	
uninstall:
	sudo cat install.log | sudo xargs rm -rf 

clean:
	sudo rm -rf build
	rm -f *.pyc
	rm -f install.log

