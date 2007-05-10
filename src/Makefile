mailod: mailod.c const.h database_function.o email.o hash_function.o common_function.o logging.o
	gcc -g -Wall -lm -ldl -ldbi -lacl mailod.c email.o hash_function.o common_function.o logging.o database_function.o -o ../bin/$@ 
database_function.o: database_function.c database_function.h email.h common_function.h
	gcc -g -Wall -c database_function.c
logging.o: logging.c logging.h const.h common_function.h
	gcc -g -Wall -c logging.c
common_function.o: common_function.c common_function.h 
	gcc -g -Wall -c common_function.c
hash_function.o: hash_function.c hash_function.h
	gcc -g -Wall -c hash_function.c
email.o: email.c email.h const.h
	gcc -g -Wall -c email.c

install:
	echo "Installing mailod to /usr/sbin"
	cp ../bin/mailod /usr/sbin/mailod
	if ( -f /etc/postfix/mailod.conf ) then
		echo "Config file is installed from previous installation"
	else
		echo "Instaling new mailod.conf to /etc/postfix/mailod.conf"
		cp ../etc/postfix/mailod.conf /etc/postfix/mailod.conf
	endif
clean:
	echo "Cleaning object files and binaries from system"
	rm *.o
	rm ../bin/mailod
