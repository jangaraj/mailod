Wall: compile
ex: execute

compile:
	gcc -g -Wall -c hash_function.c
	gcc -g -Wall -c email.c
	gcc -g -Wall -o xmailod mailod.c email.o hash_function.o
execute:
	./xmailod

