

CC=g++
CFLAGS=-c -Wall -O3 -I./ -I../libCommon -I../lib3D -DGLES2 -DLINUX -DPANDORA
LDFLAGS=-L../usr/lib -L../libs -lX11 -lGLESv2 -lEGL -lts -l3D -lTinyXml -lpng -lzlib -lCommon
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=launcher

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@  $(LDFLAGS)
	mv $(EXECUTABLE) ../../Bin/
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)
	rm -rf ../../Bin/$(EXECUTABLE)
