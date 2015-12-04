# the compiler to use
CC = clang++

# compiler flags:
CFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -lm -lSOIL -lGLEW

# add files
# INITSHADER = common/InitShader.cpp
# INITGLEW = common/iniciaGLEW.cpp 
# INITGLFW = common/iniciaGLFW.cpp
# INITOBJLOADER = common/objloader.cpp
SHADER = common/shader.cpp

#files to link:
LFLAGS = -I/usr/local/include/GLFW -lglfw3 

# the name to use for both the target source file, and the output file:
TARGET = tutorial04
  
all: execute
  
compiler:
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LFLAGS) $(SHADER)

execute: compiler
	./$(TARGET)