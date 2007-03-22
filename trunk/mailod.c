#include <stdio.h>
#include <stdlib.h>			//exit
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>		//O_CREAT
#include <string.h>			//strcat
#include "const.h"			//BUFFER_SIZE
#include "email.h"			//email struct

email *readmail(void );

int main(int argc, char* argv[]) {
	email *p_new_email;

	p_new_email = readmail();
	printf("Toto je nacitany mail z email struktury:\n %s\n", p_new_email->body);

	return 0;
}

email *readmail(void ){
	char buffer[BUFFER_SIZE], *reading_text;
	int read_size, desc_out, nblock;	//write_size
	email *read_email;

	nblock=0;

	if((read_email=(email *) malloc(sizeof(email)))==NULL) {
		fprintf(stderr, "Error, malloc of reading email\n");
		exit (1);
	}
	if((reading_text=(char *) malloc(sizeof(char)))==NULL) {
		fprintf(stderr,"Error, malloc of char of reading email\n");
		exit (1);
	}
	*(reading_text+0)='\0';
	//desc_out = open("output" , O_CREAT | O_APPEND | O_WRONLY | O_TRUNC , S_IRUSR | S_IWUSR); //otvorim si subor na zap
	while (0<(read_size = read(0, buffer, BUFFER_SIZE))) {
		/*write_size = write(desc_out, buffer, read_size);
		if(write_size != read_size) {
			fprintf(stderr, "Error writing output file\n");
			exit (1);
		}
		if(read_size<0) {
		fprintf(stderr, "Error reading input file\n");
			exit (1);
		} */
		//TODO realokacia reading_text
		reading_text = (char *) realloc(reading_text, sizeof(reading_text)+((nblock*BUFFER_SIZE)+read_size+10)*sizeof(char));
		printf("buffer: %s\n", buffer);
		printf("reading_text: %s\n", reading_text);
		strcat(reading_text, buffer);
		nblock++;							//counter na citaci cyklus - pocet alokovanych blokov
	}
	close(desc_out);	
	read_email->body = reading_text;
	printf("Toto je nacitany mail z email struktury:\n %s\n", read_email->body);

	return read_email;
}
