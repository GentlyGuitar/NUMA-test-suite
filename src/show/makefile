PROJECT=/home/tzhou/explore/numa/
CC=g++
CFLAGS=-std=c++0x -c -lnuma
LDFLAGS=-lnuma
SHARE_DIR=$(PROJECT)src/share/
THIS_DIR=$(PROJECT)src/show/
BUILD_DIR=$(PROJECT)build/

EXECUTABLE=show
CPP_FILES=$(wildcard $(THIS_DIR)/*.cpp) # grab all the cpp files in ./
SOURCES=$(CPP_FILES) $(SHARE_DIR)optParser.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)$@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
