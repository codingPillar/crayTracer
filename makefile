OUT_DIR:=build/make
SRC_DIR:=src

SRC_FILES:=$(wildcard $(SRC_DIR)/*.c)
SRC_OBJ:=$(SRC_FILES:$(SRC_DIR)/%=$(OUT_DIR)/%.o)

CC:=gcc
CFLAGS:=-Wall -Wextra #-Werror
CINCLUDE:=-I./include
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

# WEB VIEW BUILD
WEB_OUTNAME:=web_tracer
WEB_DOCS_OUTPUT_DIR:=docs
WEB_VIEW_SRC_DIR:=webView
WEB_VIEW_SRC_FILES:=$(wildcard $(SDL_VIEW_SRC_DIR)/*.c)
WEB_VIEW_SRC_OBJ:=$(WEB_VIEW_SRC_FILES:$(SDL_VIEW_SRC_DIR)/%=$(OUT_DIR)/%.wasm.o)
WEB_VIEW_SRC_OBJ+=$(SRC_FILES:$(SRC_DIR)/%=$(OUT_DIR)/%.wasm.o)
$(OUT_DIR)/%.wasm.o:$(SRC_DIR)/%
	clang $(CINCLUDE) $(CFLAGS) --target=wasm32 --no-standard-libraries -DWEBASSEMBLY -c -o $@ $<
$(OUT_DIR)/%.wasm.o:$(WEB_VIEW_SRC_DIR)/%
	clang $(CINCLUDE) $(CFLAGS) --target=wasm32 --no-standard-libraries -DWEBASSEMBLY -c -o $@ $<
$(WEB_OUTNAME):$(WEB_VIEW_SRC_OBJ)
	clang --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -Wl,-allow-undefined-file $(WEB_VIEW_SRC_DIR)/wasm.syms -Wl,--import-memory -o $(WEB_DOCS_OUTPUT_DIR)/$(WEB_OUTNAME).wasm $^

cmake:
	cmake -S . -B build
	cd build && make

clean:
	rm -rf $(OUT_DIR)/*
	rm -f $(OUTNAME)