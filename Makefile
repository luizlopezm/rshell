CXX = g++
BIN = bin
OUT_PATH = ../bin/rshell
SRC_PATH = src
OUT_PATH2 = ../bin/ls
FILES = main.cc
FILES2 = ls.cc


all:
	mkdir -p $(BIN)
	cd $(SRC_PATH) ; $(CXX) $(FILES) -o $(OUT_PATH)
	cd $(SRC_PATH) ; $(CXX) $(FILES2) -o $(OUT_PATH2)

rshell:
	mkdir -p $(BIN)
	cd $(SRC_PATH) ; $(CXX)  $(FILES) -o $(OUT_PATH)

clean:
	rm -rf $(BIN)
ls:
	mkdir -p $(BIN)
	cd $(SRC_PATH) ; $(CXX) $(FILES2) -o $(OUT_PATH2)
