# INCDIR = -I ../include/
# LDFLAGS = $(INCDIR) -lGL -lGLEW -lglfw -lm -lSOIL


# .cpp:
# 	g++ $(LDFLAGS) -o$@   $^ ../common/InitShader.o ../common/iniciaGLFW.o ../common/iniciaGLEW.o ../common/objloader.o

# the compiler to use
CC = clang++

# compiler flags:
CFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -lm -lSOIL

# add files

INITSHADER = ../common/InitShader.cpp
INITGLEW = ../common/iniciaGLEW.cpp
INITGLFW = ../common/iniciaGLFW.cpp
INITOBJLOADER = ../common/objloader.cpp

#files to link:
LFLAGS = -I/usr/local/include/GLFW -lglfw3

# the name to use for both the target source file, and the output file:
TARGET = solar-system

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LFLAGS) $(INITSHADER) $(INITGLFW) $(INITGLEW) $(INITOBJLOADER)