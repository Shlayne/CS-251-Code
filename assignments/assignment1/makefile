GCC = g++ -c -std=c++17
LIB = -lgmpxx -lgmp
OBJ = main.o
NAME = main.exe

all: $(NAME)

debug: GCC += -g
debug: $(NAME)

$(NAME): $(OBJ)
	g++ -o $(NAME) $(LIB) $(OBJ)

main.o: main.cpp
	$(GCC) main.cpp

.phony: run
run: all
	./$(NAME)

.phony: drun
drun: debug
	valgrind --tool=memcheck --leak-check=full ./$(NAME)

.phony: clean
clean:
	rm -f $(NAME) $(OBJ)
