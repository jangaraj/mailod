#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "email.h"
#include "const.h"

email *readmail(void){
	char buffer[BUFFER_SIZE], *reading_text;	
	int read_size, nblock;
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
	while (0<(read_size = read(0, buffer, BUFFER_SIZE))) {
		if(read_size<0) {
		fprintf(stderr, "Error reading input file\n");
			exit (1);
		} 
		if((reading_text = (char *) realloc(reading_text, ((nblock*BUFFER_SIZE)+read_size+1)*sizeof(char)))==NULL) {
			fprintf(stderr,"Error, realloc by reading text\n");
			exit(1);
		}
		strncat(reading_text, buffer,read_size);
		nblock++;							//counter na citaci cyklus - pocet alokovanych blokov
	}
	read_email->body = reading_text;

	return read_email;
}
