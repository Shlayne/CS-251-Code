GCC = g++ -c -g -std=c++17
OBJ = main.o
NAME = main.exe

$(NAME): $(OBJ)
	g++ -o $(NAME) $(OBJ)

main.o: main.cpp
	$(GCC) main.cpp

.phony: run
run: $(NAME)
	valgrind --tool=memcheck --leak-check=full ./$(NAME)

.phony: clean
clean:
	rm -f $(NAME) $(OBJ)
