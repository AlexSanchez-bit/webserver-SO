.PHONY: build
build:
	gcc main.c -o server.o

.PHONY: dev
run:
	gcc main.c -o server.out && ./server.out 1234 /home
