
all : 
	mkdir -p build
	gcc src/main.c -o build/catbin -Wall

install :
	mkdir -p ~/.local/bin
	cp build/catbin ~/.local/bin/catbin
