CC=g++
CFLAGS=-c -Wall -O3 -I./ -DGLES2 -DLINUX -DPANDORA
LDFLAGS=-g
SOURCES=tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libTinyXml.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) 
	ar rcs $(LIBRARY) $(OBJECTS) 
	mv $(LIBRARY) ../libs/

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(LIBRARY)
	rm -rf ../libs/$(LIBRARY)