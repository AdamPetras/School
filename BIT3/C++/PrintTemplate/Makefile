all: main
main: main.o
	g++ -std=c++14 -Wall -Wextra -Werror -g -o app main.o
clean:
	-rm main.o app
main.o: main.cpp PrintTemplate.h
	g++ -std=c++14 -Wall -Wextra -Werror -c -g main.cpp