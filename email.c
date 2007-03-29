#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "email.h"
#include "const.h"

email *readmail(void){
	char buffer[BUFFER_SIZE], *reading_email_all, *reading_email_head, *reading_email_body, *reading_email_to, *position, *position2;	
	int read_size, nblock, length_position;
	email *read_email;

	nblock=0;

	if((read_email=(email *) malloc(sizeof(email)))==NULL) {
		fprintf(stderr, "Error, malloc of reading email\n");
		exit (1);
	}
	if((reading_email_all=(char *) malloc(sizeof(char)))==NULL) {
		fprintf(stderr,"Error, malloc of char of reading email\n");
		exit (1);
	}
	*(reading_email_all)='\0';
	if((reading_email_head=(char *) malloc(sizeof(char)))==NULL) {
		fprintf(stderr,"Error, malloc of char of reading email\n");
		exit (1);
	}
	*(reading_email_head)='\0';
	while (0<(read_size = read(0, buffer, BUFFER_SIZE))) {
		if(read_size<0) {
		fprintf(stderr, "Error reading input file\n");
			exit (1);
		} 
		if((reading_email_all = (char *) realloc(reading_email_all, ((nblock*BUFFER_SIZE)+read_size+1)*sizeof(char)))==NULL) {
			fprintf(stderr,"Error, realloc by reading text\n");
			exit(1);
		}
		strncat(reading_email_all, buffer, read_size);
		nblock++;							//counter na citaci cyklus - pocet alokovanych blokov
	}
	//divide head from all
	position = strstr(reading_email_all, DIVIDER_HEAD_BODY);
	if(position==NULL) {
		fprintf(stderr,"Error, dividing email to head and body\n");
		exit (1);
	}
	length_position = position - reading_email_all;
	if((reading_email_head=(char *) realloc(reading_email_head, length_position*sizeof(char)))==NULL) {
		fprintf(stderr,"Error, realloc reading_email_head\n");
		exit (1);
	}
	strncat(reading_email_head, reading_email_all, length_position);
	if((reading_email_body=(char *) malloc((strlen(position)+1)*sizeof(char)))==NULL) {
		fprintf(stderr, "Error, malloc reading_email_body\n");
		exit (1);
	}
	*reading_email_body = '\0';
	strcat(reading_email_body, position+2);
	//determinate to: local user
	position = strstr(reading_email_all, PARSE_TO);
	if(position==NULL) {
		fprintf(stderr,"Error, not found in mail header parse string %s\n",PARSE_TO);
		exit (1);
	}
	//TODO odeknut "To: "
	position2 = strstr(position+4, "\n");
	if(position2==NULL) {
		fprintf(stderr,"Error, in email header - parse string %s\n",PARSE_TO);
		exit (1);
	}
	length_position = position2 - position;
	if((reading_email_to=(char *) malloc(length_position*sizeof(char)))==NULL) {
		fprintf(stderr,"Error, malloc reading_email_to\n");
		exit (1);
	}
	//printf("Druhe To parse je %s\n",position2);
	printf("lenght To: %d\n",length_position);
	strncat(reading_email_to, position+4, length_position);

	//printf("To je tu: %s\n",position);

	free((void *) reading_email_all);
	read_email->head = reading_email_head;
	read_email->body = reading_email_body;
	read_email->to = reading_email_to;

	return read_email;
}
