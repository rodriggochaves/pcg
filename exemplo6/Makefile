INCDIR = -I ../include/
LDFLAGS = $(INCDIR) -lGL -lGLEW -lglfw -lm


.cpp: 
	g++ $(LDFLAGS) -o$@   $^ ../common/InitShader.o ../common/iniciaGLFW.o ../common/iniciaGLEW.o
