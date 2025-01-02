.PHONY: all test prerequisites clean

TARGET=purp3d

all:
	# compile
	@g++ src/main.cpp -o $(TARGET) -lGL -lGLEW -lglfw -Iglm -Ilib/stb
	@$(MAKE) test

test:
	# running $(TARGET)
	@$(abspath $(TARGET))

prerequisites:
	@sudo apt install g++ libglfw3-dev libglew-dev libglm-dev libglm-dev libgl1-mesa-glx libglew-dev libglfw3-dev

clean:
	rm -rf $(TARGET)