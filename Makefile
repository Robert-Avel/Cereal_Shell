CXX := gcc
TEST_EXE := test
TEST_FILE := test.c
INCLUDE := -Iinclude
SRC := $(shell find src -name "*.c")


$(TEST_EXE):
	$(CXX) $(INCLUDE) $(TEST_FILE) $(SRC) -o $(TEST_EXE) -g -Wall
