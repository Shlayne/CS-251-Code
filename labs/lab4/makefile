CPP_FLAGS := -std=c++17 -O3
LIB_FLAGS := -lgmpxx -lgmp
SRC_DIR := src
BIN_DIR := bin
INT_DIR := bin-int
NAME := main.exe

# This find command is not 100% perfect. It's goal is to reduce the number of directories to make for the intermediate binaries.
# The only case I've found in which it doesn't remove a directory it should is if a parent directory has a .cpp file and another
# directory that has another .cpp file: this doesn't remove the parent directory, even though the child directory would recursively
# add the parent.
DIRS := $(BIN_DIR) $(INT_DIR) $(patsubst $(SRC_DIR)/%,$(INT_DIR)/%,$(shell find $(SRC_DIR) -mindepth 2 -type f -name "*.cpp" | sed -r "s|/[^/]+$$||" | sort -u))
INT_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(INT_DIR)/%.o,$(shell find $(SRC_DIR) -type f -name "*.cpp"))
OUT_FILE := $(BIN_DIR)/$(NAME)

all: $(OUT_FILE)

$(OUT_FILE): $(INT_FILES)
	g++ $(LIB_FLAGS) -o $@ $^

$(INT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(DIRS)
	g++ $(CPP_FLAGS) -c -o $@ $^

$(DIRS):
	mkdir -p $(DIRS)

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
