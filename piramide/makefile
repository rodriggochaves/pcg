# the compiler to use
CC = clang++

# compiler flags:
CFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

# add files

INITSHADER = ../common/InitShader.cpp
INITGLEW = ../common/iniciaGLEW.cpp
INITGLFW = ../common/iniciaGLFW.cpp

#files to link:
LFLAGS = -I/usr/local/include/GLFW -lglfw3

# the name to use for both the target source file, and the output file:
TARGET = piramide

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LFLAGS) $(INITSHADER) $(INITGLFW) $(INITGLEW)