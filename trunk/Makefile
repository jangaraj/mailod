#Wall: compile
#ex: execute

#compile:
#	gcc -g -Wall -c database_function.c -L/home/vlk/bin/lib/
#	gcc -g -Wall -c logging.c
#	gcc -g -Wall -c common_function.c
#	gcc -g -Wall -c hash_function.c
#	gcc -g -Wall -c email.c
#	gcc -g -Wall -lm -ldl -ldbi -L/home/vlk/bin/lib/ -o xmailod mailod.c email.o hash_function.o common_function.o logging.o database_function.o 

#execute:
#	./xmailod
xmailod: mailod.c const.h database_function.o email.o hash_function.o common_function.o logging.o
	gcc -g -Wall -lm -ldl -ldbi -L/home/vlk/bin/lib/ -o $@ mailod.c email.o hash_function.o common_function.o logging.o database_function.o 
database_function.o: email.h common_function.h database_function.h
	gcc -g -Wall -c database_function.c -L/home/vlk/bin/lib/
logging.o: const.h common_function.o logging.h
	gcc -g -Wall -c logging.c
common_function.o: common_function.h 
	gcc -g -Wall -c common_function.c
hash_function.o: hash_function.h
	gcc -g -Wall -c hash_function.c
email.o: const.h email.h
	gcc -g -Wall -c email.c

