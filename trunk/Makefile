Wall: compile
ex: execute

compile:
	gcc -lm -ldl -ldbi -c database_function.c -L/home/vlk/bin/lib/
	gcc -g -Wall -c logging.c
	gcc -g -Wall -c common_function.c
	gcc -g -Wall -c hash_function.c
	gcc -g -Wall -c email.c
	gcc -g -Wall -o xmailod mailod.c email.o hash_function.o common_function.o logging.o database_function.o
execute:
	./xmailod

