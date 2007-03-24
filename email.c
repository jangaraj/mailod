#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "email.h"
#include "const.h"

email *readmail(void){
	char buffer[BUFFER_SIZE], *reading_email_all, *reading_email_head, *reading_email_body, *position;	
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
/*	if((reading_email_body=(char *) malloc(sizeof(char)))==NULL) {
		fprintf(stderr,"Error, malloc of char of reading email\n");
		exit (1);
	}
	*(reading_email_body)='\0';*/
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
	position = strstr(reading_email_all, DIVIDER_HEAD_BODY);
	if(position==NULL) {
		fprintf(stderr,"Error, dividing email to head and body\n");
		exit (1);
	}
	length_position = position - reading_email_all;
	//printf("Pozicia dividera-dlzka headu: %d\n", length_position);
	if((reading_email_head=(char *) realloc(reading_email_head, length_position*sizeof(char)))==NULL) {
		fprintf(stderr,"Error, realloc reading_email_head\n");
		exit (1);
	}
	strncat(reading_email_head, reading_email_all, length_position);
	//printf("strlen-position %d\n", strlen(position));
	//printf("brak 55\n");
	//printf("mallockujem na body: %d\n", (strlen(position)+1));
	/*if((reading_email_body=(char *) malloc((strlen(position)+1)*sizeof(char)))==NULL) {
		fprintf(stderr, "Error, malloc reading_email_body\n");
		exit (1);
	}*/
	//printf("brak 59\n");
	reading_email_body = strcpy(reading_email_all, (position-(2*sizeof(char))));
	//printf("dlzka headu: %d\ndlzka body: %d\n", strlen(reading_email_head), strlen(reading_email_body));
	//TODO spravne uvolnit reading_email_all - pozor daco je v body
	free((void *) reading_email_all);
	read_email->head = reading_email_head;
	read_email->body = reading_email_body;

	return read_email;
}
