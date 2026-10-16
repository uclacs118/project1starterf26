CXX=g++
CXXFLAGS=-s

all: clean build

default: build

build: server.cpp client.cpp
	${CXX} -o server server.cpp ${CXXFLAGS}
	${CXX} -o client client.cpp ${CXXFLAGS}

clean:
	rm -rf server client *.bin *.out *.dSYM

zip: clean
	rm -f project.zip
	mkdir -p project
	cp server.c client.c consts.h Makefile project
	zip project1.zip project/*
