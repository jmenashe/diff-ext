PROJ := liblayout
BUILD ?= .
UPDATE_VERSION ?= ../bin/update_version

CXX ?= g++.exe
CC ?= gcc.exe
AR ?= ar.exe
WINDRES ?= windres.exe
DLLWRAP ?= dllwrap.exe

LIB ?= ../lib
INCLUDE ?= ../include

CXXFLAGS ?= -g -I$(INCLUDE) -D_WIN32_IE=0x0500
CFLAGS ?= -I$(INCLUDE) -D_UNICODE -DUNICODE -D_WIN32_IE=0x0500 -Wall -ansi -pedantic -g 
#ARFLAGS += rc
LIBS :=  
#LDFLAGS += 
STATICLIB = $(LIB)/$(PROJ).a

SRC-CXX = $(wildcard *.cpp)
SRC-C = $(wildcard *.c)
SRC-RC = $(wildcard *.rc)

RES = $(SRC-RC:.rc=.o)
OBJ := $(SRC-CXX:.cpp=.o) $(SRC-C:.c=.o)

INCS :=
EXE  := $(BUILD)/$(PROJ).exe

.PHONY: all all-before all-after clean clean-custom

.SUFFIXES: .rc

all: .depend all-before $(STATICLIB) $(LANG) all-after

.depend: Makefile $(SRC-C) $(SRC-RC) $(SRC-CXX)
	$(CXX) -M $(CXXFLAGS) $(SRC-C) $(SRC-RC) $(SRC-CXX) > .depend
	
include .depend

all-after: $(SRC-RC)
#	$(UPDATE_VERSION)  --input $< --output $<

clean: clean-custom
	${RM}  $(OBJ) $(EXE) ${RES} ${EXPLIB} $(STATICLIB)

$(STATICLIB): $(OBJ) $(RES)
	$(AR) $(ARFLAGS) $@  $(OBJ) $(RES) 
#	strip $@

.cpp.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS)

.rc.o:
	$(WINDRES)  -D_WIN32_IE=0x0500 $< -J rc -o $@ -O coff -DMING
