# PCSX GTE Test Framework Makefile

CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -O2 -Wall
CFLAGS = -O2
INCLUDES = -Iinclude

SRCDIR = src
BINDIR = bin
TARGET = $(BINDIR)/gte-pcsx-runner

C_SOURCES = $(SRCDIR)/gte.c $(SRCDIR)/pcsx_globals.c
C_OBJECTS = $(C_SOURCES:.c=.o)

CPP_SOURCES = $(SRCDIR)/main.cpp \
              $(SRCDIR)/pcsx_gte_stub.cpp \
              $(SRCDIR)/pcsx_gte_cpu_state.cpp \
              $(SRCDIR)/json_parser.cpp \
              $(SRCDIR)/gte_model.cpp \
              $(SRCDIR)/test_framework.cpp \
              $(SRCDIR)/gte_stub.cpp

CPP_OBJECTS = $(CPP_SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(C_OBJECTS) $(CPP_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) -o $@ $^

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(BINDIR)

run: $(TARGET)
	./$(TARGET) $(args)

.PHONY: all clean run
