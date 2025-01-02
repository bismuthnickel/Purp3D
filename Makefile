.PHONY: all prerequisites clean

TARGET=purp3d

all:
	g++ src/main.cpp -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system

prerequisites:
	sudo apt install g++
	sudo apt install libsfml-dev

clean:
	rm -rf $(TARGET)