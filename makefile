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


SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
LIB_DIR = lib
EXE_DIR = exe
APP_NAME = Dynamics
MAC_DIR = mac_build
WX_CONFIG = /home/ezra/Documents/builds/wxWidgets-3.1.0/64bit-build/wx-config

WX_FLAGS = $(subst -I,-isystem, $(shell $(WX_CONFIG) --cxxflags))

#Finds all src files in all directories of any depth in src/
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
SRCS = $(call rwildcard,$(SRC_DIR)/,*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

LIBS = `$(WX_CONFIG) --gl-libs --libs`
LIBS += $(call rwildcard, $(LIB_DIR)/,*.a)
LIBS += -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lstdc++


CC = g++ -std=c++11 -static-libgcc -static-libstdc++ -I$(SRC_DIR) -isystem $(INCLUDE_DIR) $(OS_SPECIFIC) $(WX_FLAGS)

DIRS = $(EXE_DIR) $(OBJ_DIR) $(dir $(OBJS))

WARNINGS = -Wall -pedantic  \
    -pedantic-errors -Wextra -Wcast-align \
    -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion \
    -Wdisabled-optimization \
    -Wfloat-equal  -Wformat  -Wformat=2 \
    -Wformat-nonliteral -Wformat-security  \
    -Wformat-y2k \
    -Wimport  -Winit-self  -Winline \
    -Winvalid-pch   \
    -Wlong-long -Wmissing-braces \
    -Wmissing-field-initializers -Wmissing-format-attribute   \
    -Wmissing-include-dirs -Wmissing-noreturn \
    -Wpacked -Wparentheses  -Wpointer-arith \
    -Wredundant-decls -Wreturn-type \
    -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector \
    -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch  -Wswitch-default \
    -Wswitch-enum -Wtrigraphs  -Wuninitialized \
    -Wunknown-pragmas  -Wunreachable-code \
    -Wunused-function  -Wunused-label -Wunused \
    -Wunused-value  -Wunused-variable  -Wvariadic-macros \
    -Wvolatile-register-var  -Wwrite-strings \
		-Wno-unused-parameter \
		-Wno-reorder -Wno-sign-conversion

DEVELOPMENT = -ggdb -O0
PRODUCTION = -Waggregate-return -Weffc++ -O3 -Wunused-parameter -Wpadded -Werror

all: $(EXE_DIR)/$(APP_NAME) package

$(EXE_DIR)/$(APP_NAME): $(OBJS) .dirstamp
	@echo Linking...
	$(CC) $(WARNINGS) $(DEVELOPMENT) $(OBJS) -L$(LIB_DIR) $(LIBS)  -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp .dirstamp
	@echo Compiling $<
	@$(CC) $(WARNINGS) $(DEVELOPMENT) -c -o $@ $<

package: $(EXE_DIR)/$(APP_NAME) $(MAC_DIR)/Info.plist .dirstamp
ifeq ($(OS),MAC)
		@SetFile -t APPL $(EXE_DIR)/$(APP_NAME)
		@cp $(MAC_DIR)/Info.plist $(EXE_DIR)/$(APP_NAME).app/Contents/Info.plist
		@printf "APPL????" > $(EXE_DIR)/$(APP_NAME).app/Contents/PkgInfo
		@cp $(EXE_DIR)/$(APP_NAME) $(EXE_DIR)/$(APP_NAME).app/Contents/MacOS/$(APP_NAME)
		@cp resources/* $(EXE_DIR)/$(APP_NAME).app/Contents/Resources/
endif
ifeq ($(OS),LINUX)

endif

ifeq ($(OS),WINDOWS)
endif

depend:
	rm .depend
	$(foreach SRC,$(SRCS),$(CC) $(WARNINGS) $(DEVELOPMENT) \
	$(INCLUDES) $(OS_SPECIFIC) -MM $(SRC) -MT  $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC)) >> .depend;)

-include .depend

.dirstamp:
	mkdir -p $(DIRS)
	touch $@

clean:
	-@rm -r $(OBJ_DIR)
	-@rm -r $(EXE_DIR)
	-@rm .dirstamp

.PHONY: build-wx clean-wx all clean depend
