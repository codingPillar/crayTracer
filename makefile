OUT_DIR:=build/make
SRC_DIR:=src

SRC_FILES:=$(wildcard $(SRC_DIR)/*.c)
SRC_OBJ:=$(SRC_FILES:$(SRC_DIR)/%=$(OUT_DIR)/%.o)

CC:=gcc
CFLAGS:=-Wall -Wextra -Werror
CINCLUDE:=-I./include
CLINK:=-lm
define COMPILE =
$(CC) $(CFLAGS) $(CINCLUDE) -o $@ $^
endef

$(OUT_DIR)/%.o: $(SRC_DIR)/%
	$(COMPILE) -c

# ACTUAL BUILD TARGET
OUTNAME:=tracer
$(OUTNAME): $(SRC_OBJ)
	$(COMPILE) $(CLINK) 

cmake:
	cmake -S . -B build
	cd build && make

clean:
	rm -rf $(OUT_DIR)/*
	rm -f $(OUTNAME)