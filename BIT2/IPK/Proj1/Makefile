all: client server

run: 
	./ipk-server -p 2000
	./ipk-client -h 127.0.0.1 -p 2000 -f adam

server: server.o
	g++ -g -o ipk-server server.o
client: client.o
	g++ -g -o ipk-client client.o

clean:
	-rm server.o client.o ipk-client ipk-server
server.o: server.cpp 
	g++ -c -g server.cpp
client.o: client.cpp
	g++ -c -g client.cpp
