# Makefile for Theorafile
# Written by Ethan "flibitijibibo" Lee

# System information
UNAME = $(shell uname)

# Compiler
ifeq ($(UNAME), Darwin)
	CC += -arch i386 -arch x86_64 -mmacosx-version-min=10.6
	TARGET = dylib
else
	TARGET = so
endif

# Dependencies
DEPS = -logg -lvorbis -ltheoradec

# Targets
all: clean
	$(CC) -O2 -fpic -fPIC -shared -o libtheorafile.$(TARGET) theorafile.c $(DEPS) -Wall -pedantic

clean:
	rm -f libtheorafile.$(TARGET)

test:
	$(CC) -g -o theorafile-test sdl2test/sdl2test.c theorafile.c -I. `sdl2-config --cflags --libs` $(DEPS) -Wall
