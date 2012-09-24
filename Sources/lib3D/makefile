CC=g++
CFLAGS=-c -Wall -O3 -I../libCommon -I../tinyxml -I../zlib -I../libPng -DGLES2 -DLINUX -DPANDORA
LDFLAGS=-g
SOURCES=EngWindow.cpp Geometry.cpp GLES20Context.cpp Indexbuffer.cpp Shaderlib.cpp Vertexbuffer.cpp Camera.cpp Texture.cpp Material.cpp Mesh.cpp Model.cpp Node.cpp ConvertDAE.cpp libEngine.cpp Logic.cpp SceneNode.cpp Input.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=lib3D.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) 
	ar rcs $(LIBRARY) $(OBJECTS) 
	mv $(LIBRARY) ../libs/

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(LIBRARY)
	rm -rf ../libs/$(LIBRARY)