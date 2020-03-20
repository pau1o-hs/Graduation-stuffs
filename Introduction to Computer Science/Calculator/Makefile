all:
	gcc -c -Wall stack.c
	gcc -c -Wall Calculator.c -lm
	gcc -o Calculator.exe stack.c Calculator.o -lm

run:
	./Calculator.exe
