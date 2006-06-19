export BIN := $(shell pwd)/bin
export BUILD := $(shell pwd)/build
export LIB := $(shell pwd)/lib
export INCLUDE := $(shell pwd)/include
export CXX  := g++.exe
export CC   := gcc.exe
export AR := ar.exe
export WINDRES := windres.exe
export DLLWRAP = dllwrap.exe
export CXXFLAGS := -ansi -pedantic -Wall -W -I$(INCLUDE)
export CFLAGS := -ansi -pedantic -Wall -W -I$(INCLUDE)
export LDFLAGS := -L$(LIB)

all:
	mkdir -p $(LIB)
	$(MAKE) -C src
        
clean:
	$(MAKE) -C src clean
