PROJ := libutil.a

ROOT ?= ../..

LIB ?= .

include $(ROOT)/make/static-library.gmk

CXXFLAGS += -I../../util/include -I../include -D_WIN32_WINNT=0x0500
CFLAGS += -I../../util/include -I../include -D_WIN32_WINNT=0x0500
LDFLAGS += -mwindows
