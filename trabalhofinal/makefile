# the compiler to use
CC = clang++

# compiler flags:
CFLAGS = -std=c++11 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -lm -lSOIL -lGLEW

# add files
SHADER = common/shader.cpp
TEXTURE = common/texture.cpp
CONTROLS = common/controls.cpp

#files to link:
LFLAGS = -I/usr/local/include/GLFW -lglfw3 -lassimp

# the name to use for both the target source file, and the output file:
TARGET = trabalho
  
all: execute
  
compiler:
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LFLAGS) $(SHADER) $(TEXTURE) $(CONTROLS)

execute: compiler
	./$(TARGET)