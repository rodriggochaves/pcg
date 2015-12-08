# the compiler to use
CC = clang++

# compiler flags:
# CFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

# add files

INITSHADER = common/InitShader.cpp
INITGLEW = common/iniciaGLEW.cpp 
INITGLFW = common/iniciaGLFW.cpp 
INITOBJLOADER = common/objloader.cpp common/shader.cpp common/texture.cpp common/controls.cpp

#files to link:
LFLAGS = -std=c++11 -lXinerama -lX11 -lglfw3 -lGL -lm -lXrandr -lXcursor -lrt -pthread -lXi -lXxf86vm -lpthread -lSOIL -lGLEW -ldl -lGLU -lassimp

# the name to use for both the target source file, and the output file:
TARGET = t3-novo

all: $(TARGET)
  
#$(TARGET): $(TARGET).cpp
#	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LFLAGS) $(INITSHADER) $(INITGLFW) $(INITGLEW)

$(TARGET): $(TARGET).cpp
	$(CC) -o $(TARGET) $(TARGET).cpp $(LFLAGS) $(INITSHADER) $(INITGLFW) $(INITGLEW) $(INITOBJLOADER) $(CFLAGS)
