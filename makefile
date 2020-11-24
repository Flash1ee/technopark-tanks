all:
	g++ -c server.cpp
	g++ -o server-out server.o -lsfml-network -lsfml-system

	g++ -c client.cpp
	g++ -o client-out client.o -lsfml-network -lsfml-system
