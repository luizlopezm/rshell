CXX = g++
BIN = bin
OUT_PATH = ../bin/rshell
SRC_PATH = src
FILES = main.cc
CXXFLAGS = -Wall -Werror -ansi -pedantic

all:
	mkdir -p $(BIN)
	cd $(SRC_PATH) ; $(CXX) $(CXXFLAGS) $(FILES) -o $(OUT_PATH)

rshell:
	mkdir -p $(BIN)
	cd $(SRC_PATH) ; $(CXX) $(CXXFLAGS) $(FILES) -o $(OUT_PATH)

clean:
	rm -rf $(BIN)

