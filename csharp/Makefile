# Makefile for Theorafile#
# Written by Ethan "flibitijibibo" Lee

build: clean
	mkdir bin
	cp Theorafile-CS.dll.config bin
	dmcs /unsafe -debug -out:bin/Theorafile-CS.dll -target:library Theorafile.cs

clean:
	rm -rf bin
