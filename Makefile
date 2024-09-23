PHONY: run

build:
	@gcc -Wall -Wextra -g main.c -o main

run:
	@make build
	@./main
