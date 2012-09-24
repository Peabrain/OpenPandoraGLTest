CC=gcc
CFLAGS=-c -Wall -O3 -I../zlib
LDFLAGS=-g
SOURCES=png.c pngerror.c pngget.c pngmem.c pngpread.c pngread.c pngrio.c pngrtran.c pngrutil.c pngset.c pngtest.c pngtrans.c pngwio.c pngwrite.c pngwtran.c pngwutil.c
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=libpng.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) 
	ar rcs $(LIBRARY) $(OBJECTS)
	mv $(LIBRARY) ../libs/
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(LIBRARY)
	rm -rf ../libs/$(LIBRARY)