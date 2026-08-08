# PCSX GTE Test Framework Makefile

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
INCLUDES = -Iinclude

SRCDIR = src
BINDIR = bin
TARGET = $(BINDIR)/gte-pcsx-runner

SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/pcsx_gte_stub.cpp \
          $(SRCDIR)/pcsx_gte_cpu_state.cpp \
          $(SRCDIR)/gte.c

OBJECTS = $(SOURCES:.cpp=.o,.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.c
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(BINDIR)

run: $(TARGET)
	./$(TARGET) $(args)

.PHONY: all clean run
