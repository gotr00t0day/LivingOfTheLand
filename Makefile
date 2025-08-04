# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g

# Directories
SRC_DIR = .
CORE_DIR = core
TOOLS_DIR = tools
MODULES_DIR = modules

# Source and object files
SRCS = $(SRC_DIR)/main.cpp \
       $(CORE_DIR)/init.cpp \
       $(TOOLS_DIR)/suids.cpp \
       $(MODULES_DIR)/executils.cpp \
       $(TOOLS_DIR)/web.cpp \
       $(MODULES_DIR)/parsers.cpp \
       $(MODULES_DIR)/base64.cpp \
       $(TOOLS_DIR)/portscanner.cpp \
       $(TOOLS_DIR)/clearlogs.cpp \
       $(MODULES_DIR)/scandir.cpp

OBJS = $(SRCS:.cpp=.o)

# Output binary
TARGET = shadowharvester

# Default build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rebuild if headers change
$(CORE_DIR)/init.o: $(CORE_DIR)/init.h
$(TOOLS_DIR)/suids.o: $(TOOLS_DIR)/suids.h
$(MODULES_DIR)/executils.o: $(MODULES_DIR)/executils.h
$(TOOLS_DIR)/web.o: $(TOOLS_DIR)/web.h
$(MODULES_DIR)/parsers.o: $(MODULES_DIR)/parsers.h
$(MODULES_DIR)/base64.o: $(MODULES_DIR)/base64.h
$(TOOLS_DIR)/portscanner.o: $(TOOLS_DIR)/portscanner.h
$(TOOLS_DIR)/clearlogs.o: $(TOOLS_DIR)/clearlogs.h
$(MODULES_DIR)/scandir.o: $(MODULES_DIR)/scandir.h

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Optional: force rebuild
rebuild: clean all

.PHONY: all clean rebuild
