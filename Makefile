CC := gcc
CXX := g++

COMPILE_FLAGS_C := -Ic/include -Wall
COMPILE_FLAGS_CPP := -Icpp/include -Wall

SRC_C_DIR := c/src
SRC_C := $(shell find $(SRC_C_DIR) -name "*.c")

SRC_CPP_DIR := cpp/src
SRC_CPP := $(shell find $(SRC_CPP_DIR) -name "*.cpp")

OBJ_DIR := obj
OBJ_C := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_C_DIR))
OBJ_CPP := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

LIB_OBJ := libCerealShell.a
LIB_NAME := CerealShell

PREFIX ?= /usr/local

all: $(OBJ_C) $(OBJ_CPP)
	ar rcs $(LIB_OBJ) $^


$(OBJ_DIR)/%.o: $(SRC_C_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(COMPILE_FLAGS_C) -o $@ $^


$(OBJ_DIR)/%.o: $(SRC_CPP_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(COMPILE_FLAGS_CPP) -o $@ $^


.PHONY: clear
clear: $(LIB_OBJ)
	rm -f -r $(OBJ_DIR)
	rm -f $(LIB_NAME)

.PHONY: install
install: $(LIB_OBJ)
	install -d $(PREFIX)/include/$(LIB_NAME)
	install -m 644 include/*.h $(PREFIX)/include/$(LIB_NAME)/
	install -m 644 $(LIB_OBJ) $(PREFIX)/lib/


.PHONY: uninstall
uninstall:
	rm -r $(PREFIX)/include/$(LIB_NAME)
	rm $(PREFIX)/lib/$(LIB_OBJ)


test_cpp:
	$(CXX) test.cpp $(SRC_CPP) $(COMPILE_FLAGS_CPP)  -o test_cpp
