rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
LIB_DIR = lib
EXE_DIR = exe
WX_DIR = wxWidgets
WX_BUILD_DIR = wxWidgets/std-build
APP_NAME = Dynamics
MAC_DIR = mac_build


#Finds all src files in all directories of any depth in src/
SRCS = $(call rwildcard,$(SRC_DIR)/,*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
CC = g++ -m32 -stdlib=libc++ -std=c++11 -I$(SRC_DIR)

DIRS = $(EXE_DIR)/$(APP_NAME).app $(EXE_DIR)/$(APP_NAME).app/Contents \
	$(EXE_DIR)/$(APP_NAME).app/Contents/MacOS $(EXE_DIR)/$(APP_NAME).app/Contents/Resources \
	$(EXE_DIR) $(OBJ_DIR) $(dir $(OBJS))

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
    -Wpacked -Wparentheses  -Wpointer-arith \
    -Wredundant-decls -Wreturn-type \
    -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector \
    -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch  -Wswitch-default \
    -Wswitch-enum -Wtrigraphs  -Wuninitialized \
    -Wunknown-pragmas  -Wunreachable-code \
    -Wunused-function  -Wunused-label -Wunused \
    -Wunused-value  -Wunused-variable  -Wvariadic-macros \
    -Wvolatile-register-var  -Wwrite-strings \
		-Wno-c++11-extensions -Wno-unused-parameter \
		-Wno-reorder

DEVELOPMENT = -g -O0
PRODUCTION = -static -O3 -Wunused-parameter -Wpadded -Werror



LIBS = -L $(LIB_DIR) `$(WX_BUILD_DIR)/wx-config --libs`
INCLUDES = -isystem $(INCLUDE_DIR) `$(WX_BUILD_DIR)/wx-config --cxxflags | sed 's/-I/-isystem /g'`


STUPID_MAC_STUFF = -mmacosx-version-min=10.7
DEPS = $(call rwildcard,$(SRC_DIR),*.h)

all: $(EXE_DIR)/$(APP_NAME) $(EXE_DIR)/$(APP_NAME).app

$(EXE_DIR)/$(APP_NAME): $(OBJS) .dirstamp
	@echo Linking...
	@$(CC) $(WARNINGS) $(DEVELOPMENT) $(OBJS) $(INCLUDES) $(STUPID_MAC_STUFF) $(LIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp .dirstamp
	@echo Compiling $<
	@$(CC) $(WARNINGS) $(DEVELOPMENT) $(INCLUDES) $(STUPID_MAC_STUFF) -c -o $@ $<

$(EXE_DIR)/$(APP_NAME).app: $(EXE_DIR)/$(APP_NAME) $(MAC_DIR)/Info.plist .dirstamp
	@SetFile -t APPL $(EXE_DIR)/$(APP_NAME)
	@cp $(MAC_DIR)/Info.plist $(EXE_DIR)/$(APP_NAME).app/Contents/Info.plist
	@printf "APPL????" > $(EXE_DIR)/$(APP_NAME).app/Contents/PkgInfo
	@cp $(EXE_DIR)/$(APP_NAME) $(EXE_DIR)/$(APP_NAME).app/Contents/MacOS/$(APP_NAME)
	@cp resources/* $(EXE_DIR)/$(APP_NAME).app/Contents/Resources/



clean-wx:
	-@cd $(WX_BUILD_DIR); \
	make clean; \
	make distclean
	-@rm -r $(WX_BUILD_DIR)

build-wx:
	-mkdir $(WX_BUILD_DIR)

	export CC="gcc -stdlib=libc++ -m32"; \
	export CXX="g++ -stdlib=libc++ -std=c++11 -m32"; \
	cd $(WX_BUILD_DIR); \
	../configure --disable-shared --enable-unicode --without-subdirs --with-macosx-version-min=10.7; \
	rm utils/wxrc/Makefile; \
	make

depend: .depend

.depend:
	$(foreach SRC,$(SRCS),$(CC) $(WARNINGS) $(DEVELOPMENT) \
	$(INCLUDES) $(STUPID_MAC_STUFF) -MM $(SRC) -MT  $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC)) >> .depend;)

-include .depend

.dirstamp:
	mkdir -p $(DIRS)
	touch $@

clean:
	-@rm -r $(OBJ_DIR)
	-@rm -r $(EXE_DIR)
	-@rm .dirstamp

.PHONY: build-wx clean-wx all clean depend
