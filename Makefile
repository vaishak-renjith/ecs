all: main

main: main.cpp Engine.cpp
	g++ -Isrc/Include -Lsrc/lib -o main $^ -lmingw32 -lSDL2main -lSDL2

clean:
	rm -f main
