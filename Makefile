CC := gcc
INCLUDE := -Iinclude

SRC_DIR := src
SRC := $(shell find $(SRC_DIR) -name "*.c")

OBJ_DIR := obj
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

LIB_NAME := libCerealShell.a
M_DIR_NAME := CerealShell

PREFIX ?= /usr/local


all: $(OBJ)
	ar rcs $(LIB_NAME) $^


obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(INCLUDE) -Wall -o $@ $^


.PHONY: clear
clear: $(LIB_NAME)
	rm -f -r $(OBJ_DIR)
	rm -f $(LIB_NAME)

.PHONY: install
install: $(LIB_NAME)
	install -d $(PREFIX)/include/$(M_DIR_NAME)
	install -m 644 include/*.h $(PREFIX)/include/$(M_DIR_NAME)/
	install -m 644 $(LIB_NAME) $(PREFIX)/lib/


.PHONY: uninstall
uninstall:
	rm -r $(PREFIX)/include/$(M_DIR_NAME)
	rm $(PREFIX)/lib/$(LIB_NAME)
