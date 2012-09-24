CC=gcc
CFLAGS=-c -Wall -O3 -I./ -DGLES2 -DLINUX -DPANDORA
LDFLAGS=-g
SOURCES=adler32.c compress.c crc32.c deflate.c gzclose.c gzlib.c gzread.c gzwrite.c infback.c inffast.c inflate.c inftrees.c trees.c zutil.c
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=libzlib.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) 
	ar rcs $(LIBRARY) $(OBJECTS)
	mv $(LIBRARY) ../libs/
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(LIBRARY)
	rm -rf ../libs/$(LIBRARY)