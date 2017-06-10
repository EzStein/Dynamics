rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
LIB_DIR = lib
EXE_DIR = exe
WX_DIR = wxWidgets/std-build

#Finds all src files in all directories of any depth in src/
SRCS = $(call rwildcard,$(SRC_DIR)/,*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
CC = g++
WARNINGS = -Wall -Weffc++ -pedantic  \
    -pedantic-errors -Wextra -Waggregate-return -Wcast-align \
    -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion \
    -Wdisabled-optimization \
    -Wfloat-equal  -Wformat  -Wformat=2 \
    -Wformat-nonliteral -Wformat-security  \
    -Wformat-y2k \
    -Wimplicit  -Wimport  -Winit-self  -Winline \
    -Winvalid-pch   \
    -Wlong-long -Wmissing-braces \
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
    -Wvolatile-register-var  -Wwrite-strings

DEVELOPMENT = -g -O0
PRODUCTION = -static -O3



LIBS = -L $(LIB_DIR) `$(WX_DIR)/wx-config --libs`
INCLUDES = -isystem $(INCLUDE_DIR) `$(WX_DIR)/wx-config --cxxflags | sed 's/-I/-isystem /g'`


STUPID_MAC_STUFF = -mmacosx-version-min=10.5
DEPS = $(call rwildcard,$(SRC_DIR),*.h)

all: exe/out

exe/out: $(OBJS)
	-@mkdir -p $(dir $@)
	@$(CC) $(WARNINGS) $(DEVELOPMENT) $(OBJS) $(INCLUDES) $(STUPID_MAC_STUFF) $(LIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	-@mkdir -p $(dir $@)
	@$(CC) $(WARNINGS) $(DEVELOPMENT) $(INCLUDES) $(STUPID_MAC_STUFF) -c -o $@ $<

clean:
	-@rm -r $(OBJ_DIR)
	-@rm -r $(EXE_DIR)
