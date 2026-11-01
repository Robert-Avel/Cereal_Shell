CC := gcc
CXX := g++

COMPILE_FLAGS_C := -Ic/include -Wall
COMPILE_FLAGS_CPP := -Icpp/include -Wall

SRC_C_DIR := c/src
SRC_C := $(shell find $(SRC_C_DIR) -name "*.c")

SRC_CPP_DIR := cpp/src
SRC_CPP := $(shell find $(SRC_CPP_DIR) -name "*.cpp")

OBJ_DIR := obj
OBJ_C := $(patsubst $(SRC_C_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_C))
OBJ_CPP := $(patsubst $(SRC_CPP_DIR)/%.cpp, $(OBJ_DIR)/%+.o, $(SRC_CPP))


P_HEADER_C := Wrapper.h CerealShell.h Function.h ArgToken2.h ArgList.h
P_HEADER_CPP := Wrapper.hpp CerealShell.hpp Function.hpp PStatus.hpp ArgToken2.hpp

P_HEADER_CPP_PATH := $(patsubst %.hpp, cpp/include/%.hpp, $(P_HEADER_CPP))
P_HEADER_C_PATH := $(patsubst %.h, c/include/%.h, $(P_HEADER_C))

LIB_OBJ := libCerealShell.a
LIB_NAME := CerealShell

PREFIX ?= /usr/local

all: $(OBJ_C) $(OBJ_CPP)
	ar rcs $(LIB_OBJ) $^


$(OBJ_DIR)/%.o: $(SRC_C_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(COMPILE_FLAGS_C) -o $@ $^


$(OBJ_DIR)/%+.o: $(SRC_CPP_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(COMPILE_FLAGS_CPP) -o $@ $^


.PHONY: clear
clear: $(LIB_OBJ)
	rm -f -r $(OBJ_DIR)
	rm -f $(LIB_OBJ)


.PHONY: install
install: $(LIB_OBJ)
	install -d $(PREFIX)/include/$(LIB_NAME)
	install -m 644 $(P_HEADER_CPP_PATH) $(P_HEADER_C_PATH) $(PREFIX)/include/$(LIB_NAME)/
	install -m 644 $(LIB_OBJ) $(PREFIX)/lib/


.PHONY: uninstall
uninstall:
	rm -r $(PREFIX)/include/$(LIB_NAME)
	rm $(PREFIX)/lib/$(LIB_OBJ)


test_c:
	$(CC) test.c $(SRC_C) $(COMPILE_FLAGS_C)  -o test_c -g


test_cpp:
	$(CXX) test.cpp $(SRC_CPP) $(COMPILE_FLAGS_CPP)  -o test_cpp -g
