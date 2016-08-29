all: ahdecode.o ahencode.o
	g++ ahencode.o -o ahencode
	g++ ahdecode.o -o ahdecode
o_ahdecode: ahdecode.cpp
	g++ -c ahdecode.cpp
o_ahencode: ahencode.cpp
	g++ -c ahencode.cpp
clean:
	rm -f *.o ahencode ahdecode

