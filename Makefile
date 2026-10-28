CC := gcc
INCLUDE := -Iinclude

SRC_DIR := src
SRC := $(shell find $(SRC_DIR) -name "*.c")

OBJ_DIR := obj
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

LIB_NAME := libCerealShell.a


$(LIB_NAME): $(OBJ)
	ar rcs $@ $^


obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(INCLUDE) -Wall -o $@ $^


clear:
	rm  $(OBJ)
