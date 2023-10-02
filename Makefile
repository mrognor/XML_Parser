CXXFLAGS = 

all: debug

debug: CXXFLAGS += -g 
debug: bin/main bin/auto_tests

release: CXXFLAGS += -D NDEBUG -O3
release: bin/main

bin/main: bin/libXML_Parser.a main.cpp
	mkdir -p bin
	g++ $(CXXFLAGS) main.cpp -I. -L./bin -lXML_Parser -o bin/main

bin/auto_tests: bin/libXML_Parser.a auto_tests.cpp
	mkdir -p bin
	g++ $(CXXFLAGS) auto_tests.cpp -I. -L./bin -lXML_Parser -o bin/auto_tests

bin/libXML_Parser.a: bin/XML_Parser.o bin/XML_Functions.o 
	ar rc bin/libXML_Parser.a bin/XML_Parser.o bin/XML_Functions.o
	ranlib bin/libXML_Parser.a

bin/XML_Parser.o: XML_Parser.h XML_Functions.h XML_Parser.cpp
	mkdir -p bin
	g++ -c $(CXXFLAGS) XML_Parser.cpp -o bin/XML_Parser.o

bin/XML_Functions.o: XML_Functions.h XML_Parser.h XML_Functions.cpp
	mkdir -p bin
	g++ -c $(CXXFLAGS) XML_Functions.cpp -o bin/XML_Functions.o

clean:
	rm -rf bin