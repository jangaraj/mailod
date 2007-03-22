#include <stdio.h>
#include <stdlib.h>			//exit
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>		//O_CREAT
#include "const.h"			//BUFFER_SIZE
#include "email.h"			//email struct

email *readmail(void );

int main(int argc, char* argv[]) {
	email *p_new_email;

	p_new_email = readmail();

	return 0;
}

email *readmail(void ){
	char buffer[BUFFER_SIZE];
	int read_size, write_size, desc_out;
	email *read_email;

	desc_out = open("output" , O_CREAT | O_APPEND | O_WRONLY | O_TRUNC , S_IRUSR | S_IWUSR); //otvorim si subor na zapis

	while (0<(read_size = read(0, buffer, BUFFER_SIZE))) {
		//TODO alokovat pole + nasledne realokovat a zvacsit dane pole
		write_size = write(desc_out, buffer, read_size);
		if(write_size != read_size) {
			fprintf(stderr, "Error writing output file\n");
			exit (1);
		}
		if(read_size<0) {
		fprintf(stderr, "Error reading input file\n");
			exit (1);
		} 
	}
	close(desc_out);	
	return read_email;
}
