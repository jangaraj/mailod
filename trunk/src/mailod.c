/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mcheck.h>			//testing of malloc
#include <errno.h>
#include "const.h"
#include "email.h"
#include "hash_function.h"
#include "common_function.h"
#include "logging.h"
#include "database_function.h"

int main(int argc, char* argv[]) {
	mtrace();

	email *new_email, *ident_email;
	config *conf_struct;
	
	if((conf_struct=(config *) malloc(sizeof(config)))==NULL) {
		fprintf(stderr, "Error, malloc config structure - exited status 1\n");
		exit (1);
	}
	if(readconf(CONFIG_FILE, conf_struct)!=0) {
		fprintf(stderr,"Error, reading config file - exited status 1\n");
		exit (1);
	}
	start_log(conf_struct);
/*	printf("Setting - db_driver: %s\n",conf_struct->db_driver);
	printf("Setting - db_username: %s\n",conf_struct->db_username);
	printf("Setting - db_password: %s\n",conf_struct->db_password);
	printf("Setting - db_name: %s\n",conf_struct->db_name);
	printf("Setting - db_host: %s\n",conf_struct->db_host);
	printf("Setting - time_window: %d\n",conf_struct->time_window);
	printf("Setting - log_level: %d\n",conf_struct->log_level);
	printf("Setting - debug: %d\n",conf_struct->debug);
	printf("Setting - port: %d\n",conf_struct->port);
	printf("Setting - backlog: %d\n",conf_struct->backlog);
	*/


	new_email = readmail();
	new_email->hash = hash_text(new_email->body);
	if((ident_email = select_by_hash(conf_struct, new_email->hash))==NULL) {
		printf("nemam ident email. Idem ho zapisat to filesystema a do DB.\n");
		if(write_email(new_email)!=0) {
			fprintf(stderr,"Error standard-writing email.\n");
		}
		else 
		{	
		  	new_email->done = 0;
		   	printf("uspesny zapis email suboru\n");
			if((insert_email(conf_struct, new_email))!=0) {
				fprintf(stderr,"Error, inserting email to database\n");	
				new_email->done = 2;					//email nebol zapisany do db
			}
			else {
				printf("uspesny zapis zaznamu do db\n");
			}
		}
	}
	else {
		//linkovanie emailu
		printf("Linkujem podla ident emailu\n");
		do {
			if(link_email(new_email, ident_email)!=0) {
				printf("linkovanie zlyhalo, ak kvoli max hardlinks bude nasledovat dalsie kolo ukladania\n");
				fprintf(stderr,"Error, linking email\n");
				if(new_email->done == EMLINK) { 
					printf("Idem zmazat zaznam, ktory uz ma full hardlinks\n");
					if((delete_email(conf_struct, ident_email))!=0) {
						fprintf(stderr,"Error, deleting full hardlinks record from database\n");
					}
				}
				free((void *) ident_email);
				if((ident_email = select_by_hash(conf_struct, new_email->hash))==NULL) {
					//nenasiel som dalsi vhodny email koncim slucku nastavim new_email na obycajny write
					printf("Nenasiel som dalsi vhodny email z db\n");
					new_email->done = 1;
					break;
				}
			}
		} while (new_email->done == EMLINK);  //ak chyba max hardliniek dalsie kolo s inym identom
		free((void *) ident_email);
	}
	//zavercne testovanie ci je done==0 - email ulozeny alebo nalinkovany
	if(new_email->done!=0) {
		if(write_email(new_email)!=0) {
			fprintf(stderr,"Error fail-writing email.\n");
			exit (1);				//pokus zapisu suboru zlyhal
		}
		else 
		{	
		   	printf("uspesny zapis email suboru2\n");
			if((insert_email(conf_struct, new_email))!=0) {
				fprintf(stderr,"Error, inserting email to database\n");	
			}
			else {
				printf("uspesny zapis zaznamu do db2\n");
			}
		}
	}
	
	
//	printf(">>>>>>>>>>>>>>>>>>>>>>>>Toto je mail->head z email struktury:\n%s\n", new_email->head);
//	printf("%s", new_email->body);
//	printf(">>>>>>>>>>>>>>>>>>>>>>>>Toto je mail->hash z email struktury:\n%s\n", new_email->hash);
//	printf(">>>>>>>>>>>>>>>>>>>>>>>>Toto je mail->to z email struktury:\n%s\n", new_email->to);
//	printf(">>>>>>>>>>>>>>>>>>>>>>>>Toto je mail->homedir z email struktury:\n%s\n", new_email->homedir);
//	printf(">>>>>>>>>>>>>>>>>>>>>>>>Toto je mail->size z email struktury:\n%d\n",new_email->size);


	//TODO uvolnit alokovanu pamat
	free((void *) conf_struct);
	free((void *) new_email->to);
	free((void *) new_email->head);
	free((void *) new_email->hash);
	free((void *) new_email->body);
	free((void *) new_email->filepath);
//	free((void *) new_email->homedir);
	free((void *) new_email);

	muntrace();
	return 0;
}
