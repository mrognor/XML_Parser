CXXFLAGS = 

all: debug

debug: CXXFLAGS += -g 
debug: bin/main

release: CXXFLAGS += -D NDEBUG -O3
release: bin/main

bin/main: bin/XML_Parser.o bin/XML_Functions.o main.cpp
	mkdir -p bin
	g++ $(CXXFLAGS) bin/XML_Parser.o bin/XML_Functions.o main.cpp -o bin/main

bin/XML_Parser.o: XML_Parser.h XML_Parser.cpp
	mkdir -p bin
	g++ -c $(CXXFLAGS) XML_Parser.cpp -o bin/XML_Parser.o

bin/XML_Functions.o: XML_Functions.h XML_Functions.cpp
	mkdir -p bin
	g++ -c $(CXXFLAGS) XML_Functions.cpp -o bin/XML_Functions.o

clean:
	rm -rf bin