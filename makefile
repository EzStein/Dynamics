# OS agnostic Makefile
# Set flags based on the operating system.
ifeq ($(OS),Windows_NT)
    OS = WIN
    OS_SPECIFIC = -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        OS_SPECIFIC += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            OS_SPECIFIC += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            OS_SPECIFIC += -D IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
	OS = LINUX
        OS_SPECIFIC = -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
	OS = MAC
	OS_SPECIFIC = -m32 -stdlib=libc++ -mmacosx-version-min=10.7 -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
	OS_SPECIFIC += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        OS_SPECIFIC += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        OS_SPECIFIC += -D ARM
    endif
endif

# All source code including headers must be contained in this directory.
SRC_DIR = src

# Each .cpp file is compiled into a corresponding .o file in this directory.
OBJ_DIR = obj

# Directory in which to look for external include files.
INCLUDE_DIR = include

# Directory in which to look for external library files.
LIB_DIR = lib

# Directory where executables and bundles are stored. 
BUILD_DIR = build

# This is a subdirectory of both OBJ_DIR and SRC_DIR where main obj and src
# files are kept.
MAIN_DIR = main 

APP_NAME = DynSolver

# A routine which recursively finds all files in a root directory.
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# A list of all .cpp files with path starting at $(SRC_DIR)
SRCS = $(call rwildcard,$(SRC_DIR)/,*.cpp)

# A list of all OBJ files.
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# A list of all OBJ files which do not contain a main method. That is,
# they are not in the obj/main folder.
NON_MAIN_OBJS = $(filter-out $(OBJ_DIR)/$(MAIN_DIR)/%, $(OBJS))

WX_CONFIG = /home/ezra/Documents/builds/wxWidgets-3.1.0/64bit-build/wx-config

WX_FLAGS = $(subst -I,-isystem, $(shell $(WX_CONFIG) --cxxflags))

LIBRARY_FLAGS = -L$(LIB_DIR) \
`$(WX_CONFIG) --gl-libs --libs` \
$(call rwildcard, $(LIB_DIR)/,*) \
-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lstdc++

INCLUDE_FLAGS = -I$(SRC_DIR) -isystem $(INCLUDE_DIR)

COMPILER_FLAGS = -std=c++11 $(OS_SPECIFIC) $(WX_FLAGS) -ggdb -O0

CC = g++

WARNINGS = -Wall -Weffc++ -pedantic  \
    -pedantic-errors -Wextra -Waggregate-return -Wcast-align \
    -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion \
    -Wdisabled-optimization \
    -Werror -Wfloat-equal  -Wformat  -Wformat=2 \
    -Wformat-nonliteral -Wformat-security  \
    -Wformat-y2k \
    -Wimplicit  -Wimport  -Winit-self  -Winline \
    -Winvalid-pch   \
    -Wunsafe-loop-optimizations  -Wlong-long -Wmissing-braces \
    -Wmissing-field-initializers -Wmissing-format-attribute   \
    -Wmissing-include-dirs -Wmissing-noreturn \
    -Wpacked  -Wpadded -Wparentheses  -Wpointer-arith \
    -Wredundant-decls -Wreturn-type \
    -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector \
    -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch  -Wswitch-default \
    -Wswitch-enum -Wtrigraphs  -Wuninitialized \
    -Wunknown-pragmas  -Wunreachable-code -Wunused \
    -Wunused-function  -Wunused-label  -Wunused-parameter \
    -Wunused-value  -Wunused-variable  -Wvariadic-macros \
    -Wvolatile-register-var  -Wwrite-stringss

all: app test

app: $(BUILD_DIR)/$(APP_NAME)

test: $(BUILD_DIR)/$(TEST_NAME)

$(BUILD_DIR)/$(APP_NAME): $(NON_MAIN_OBJS) $(APP_MAIN_OBJ)
	@mkdir -p $(@D)
	@echo Linking App...
	@$(CC) $^ $(LIBRARY_FLAGS) -o $@

$(BUILD_DIR)/$(TEST_NAME): $(NON_MAIN_OBJS) $(TEST_MAIN_OBJ)
	@mkdir -p $(@D)
	@echo Linking Test...
	@$(CC) $^ $(LIBRARY_FLAGS) -o $@

# If a certain object file is requested, it is compiled from the corresponding
# source file.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo Compiling $^
	@$(CC) $(WARNINGS) $(COMPILER_FLAGS) $(INCLUDE_FLAGS) -c -o $@ $^

# Generates a dependency file that associates each object file to the headers
# which it depends on. This should be called manually whenever a .cpp file
# changes its header dependency.
depend:
	rm .depend
	$(foreach SRC,$(SRCS),$(CC) $(INCLUDE_FLAGS) -MM $(SRC) -MT \
	$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC)) >> .depend;)

-include .depend

clean:
	-@rm -r $(OBJ_DIR)
	-@rm -r $(BUILD_DIR)
	-@rm -r .depend

.PHONY: all test app clean depend
