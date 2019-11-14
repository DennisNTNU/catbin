
all : 
	mkdir -p build
	gcc src/main.c -o build/hexdumb -Wall

install :
	mkdir -p ~/.local/bin
	cp build/hexdumb ~/.local/bin/hexdumb
