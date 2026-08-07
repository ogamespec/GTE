CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -Iinclude
SRCDIR = src
BUILDDIR = build
TARGET = gte-runner

SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/json_parser.cpp \
          $(SRCDIR)/gte_model.cpp \
          $(SRCDIR)/dummy_api.cpp \
          $(SRCDIR)/test_framework.cpp

OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
