.Phony: start
start: main.o stack_func.o
	clang++ main.o stack_func.o -o main
main.o: main.cpp main.h
	clang++ -c main.cpp
stack_func.o: stack_func.cpp main.h
	clang++ -c stack_func.cpp

