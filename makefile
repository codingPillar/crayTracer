OUT_DIR:=build/make
SRC_DIR:=src

SRC_FILES:=$(wildcard $(SRC_DIR)/*.c)
SRC_OBJ:=$(SRC_FILES:$(SRC_DIR)/%=$(OUT_DIR)/%.o)

CC:=gcc
CFLAGS:=-Wall -Wextra -Werror
CINCLUDE:=-I./include
# ONLY LIB DEPENDENCIES
CLINK:=-lm
define COMPILE =
$(CC) $(CFLAGS) $(CINCLUDE) -o $@ $^
endef

$(OUT_DIR)/%.o: $(SRC_DIR)/%
	$(COMPILE) -c

# ACTUAL BUILD TARGET
OUTNAME:=tracer
SDL_VIEW_SRC_DIR:=sdlView
SDL_VIEW_SRC_FILES:=$(wildcard $(SDL_VIEW_SRC_DIR)/*.c)
SDL_VIEW_SRC_OBJ:=$(SDL_VIEW_SRC_FILES:$(SDL_VIEW_SRC_DIR)/%=$(OUT_DIR)/%.o)
$(OUT_DIR)/%.o: $(SDL_VIEW_SRC_DIR)/%
	$(COMPILE) -c $(shell pkg-config sdl2 --cflags)
$(OUTNAME): $(SRC_OBJ) $(SDL_VIEW_SRC_OBJ)
	$(COMPILE) $(CLINK) $(shell pkg-config sdl2 --libs)

cmake:
	cmake -S . -B build
	cd build && make

clean:
	rm -rf $(OUT_DIR)/*
	rm -f $(OUTNAME)