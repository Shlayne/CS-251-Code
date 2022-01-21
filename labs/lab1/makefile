CPP_FLAGS := -std=c++17
LIB_FLAGS := -lgmpxx -lgmp
SRC_DIR := src
BIN_DIR := bin
INT_DIR := bin-int
NAME := main.exe

INT_DIRS := $(INT_DIR) $(patsubst $(SRC_DIR)/%,$(INT_DIR)/%,$(shell find $(SRC_DIR) -mindepth 1 -type d))
INT_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(INT_DIR)/%.o,$(shell find $(SRC_DIR) -name "*.cpp"))
OUT_FILE := $(BIN_DIR)/$(NAME)

all: $(OUT_FILE)

$(OUT_FILE): $(INT_FILES)
	g++ $(LIB_FLAGS) -o $@ $^

$(INT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(INT_DIRS)
	g++ $(CPP_FLAGS) -c -o $@ $^

$(INT_DIRS):
	mkdir -p $(BIN_DIR) $(INT_DIRS)

.phony: debug
debug: CPP_FLAGS += -g
debug: all

.phony: run
run: $(OUT_FILE)
	./$(OUT_FILE)

.phony: drun
drun: debug
	valgrind --tool=memcheck --leak-check=full ./$(OUT_FILE)

.phony: clean
clean:
	rm -rf $(INT_DIR)

.phony: clobber
clobber: clean
	rm -rf $(BIN_DIR)
