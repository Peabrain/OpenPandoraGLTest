CC=g++
CFLAGS=-c -Wall -O3 -I./ -DGLES2 -DLINUX -DPANDORA
LDFLAGS=-g
SOURCES=mathHelper.cpp defines.cpp Container.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libCommon.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) 
	ar rcs $(LIBRARY) $(OBJECTS) 
	mv $(LIBRARY) ../libs/

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(LIBRARY)
	rm -rf ../libs/$(LIBRARY)