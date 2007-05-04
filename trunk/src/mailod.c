/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dbi/dbi.h>
#include "const.h"
#include "email.h"
#include "hash_function.h"
#include "common_function.h"
#include "logging.h"
#include "database_function.h"

int main(int argc, char* argv[]) 
{

	email *new_email, *ident_email;
	config *conf_struct;
	dbi_conn conn;
	
	if((conf_struct=(config *) malloc(sizeof(config)))==NULL) {
		fprintf(stderr, "Error, malloc config structure - exited status 1\n");
		exit (1);
	}
	if(readconf(CONFIG_FILE, conf_struct)!=0) {
		fprintf(stderr,"Error, reading config file - exited status 1\n");
		exit (1);
	}
	start_log(conf_struct);
	logging(DEBUG, "Nacital som uspesne konfiguraciu a zapol logovanie\n");
	logging(DEBUG, "USER ID je %d\n",getuid());
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
	if((new_email = readmail()) == NULL) {
	//	fprintf(stderr,"Error, reading and processing input email\n");
		logging(DEBUG, "Error, reading and processing input email\n");
		exit (1);
	}
	if(((conn = connect_db(conf_struct)) != NULL) && (new_email->hash = hash_text(new_email->body))) {
		//connect to database and hashing body text - OK
		if((ident_email = select_by_hash(conn, new_email->hash, conf_struct->time_window))==NULL) {
	//		printf("nemam ident email. Idem ho zapisat to filesystema a do DB.\n");
			logging(DEBUG, "nemam ident email. Idem ho zapisat to filesystema a do DB.\n");
			if(write_email(new_email)!=0) {
	//			fprintf(stderr,"Error standard-writing email.\n");
				logging(DEBUG, "Error standard-writing email.\n");
			}
			else 
			{	
			  	new_email->done = 0;
	//		   	printf("uspesny zapis email suboru\n");
				logging(DEBUG, "uspesny zapis email suboru\n");
				if((insert_email(conn, new_email))!=0) {
	//				fprintf(stderr,"Error, inserting email to database\n");	
					logging(DEBUG, "Error standard-writing email.\n");
				}
				else {
	//				printf("uspesny zapis zaznamu do db\n");
					logging(DEBUG, "uspesny zapis zaznamu do db\n");
				}
			}
		}
		else {
	//		printf("Linkujem podla ident emailu\n");
			logging(DEBUG, "Linkujem podla ident emailu\n");
			do {
				if(link_email(new_email, ident_email)!=0) {
	//				printf("linkovanie zlyhalo, ak kvoli max hardlinks bude nasledovat dalsie kolo ukladania\n");
	//				fprintf(stderr,"Error, linking email\n");
					logging(DEBUG, "Error, linking email\n");
					if(new_email->done == EMLINK) { 
	//					printf("Idem zmazat zaznam, ktory uz ma full hardlinks\n");
						logging(DEBUG, "Idem zmazat zaznam, ktory uz ma full hardlinks\n");
						if((delete_email(conn, ident_email))!=0) {
	//						fprintf(stderr,"Error, deleting full hardlinks record from database\n");
							logging(DEBUG, "Error, deleting full hardlinks record from database\n");
						}
					}
					free((void *) ident_email);
					if((ident_email = select_by_hash(conn, new_email->hash, conf_struct->time_window))==NULL) {
						//not found ident email in database, break loop
	//					printf("Nenasiel som dalsi vhodny email z db\n");
						logging(DEBUG, "Nenasiel som dalsi vhodny email z db\n");
						break;
					}
				}
			} while (new_email->done == EMLINK);  //if fail on max number hardlinks - next loop
			free((void *) ident_email);
		}
	}
	//end testing - done = 0 = email is good processed
	if(new_email->done!=0) {
		if(write_email(new_email)!=0) {
	//		fprintf(stderr,"Error fail-writing email.\n");
			logging(DEBUG, "Error fail-writing email\n");
			exit (1);				//critical fail input email
		}
		else 
		{	
	//	   	printf("uspesny zapis email suboru2\n");
			logging(DEBUG, "uspesny zapis email suboru2\n");
			if((insert_email(conf_struct, new_email))!=0) {
	//			fprintf(stderr,"Error, inserting email to database2\n");	
				logging(DEBUG, "Error, inserting email to database2\n");
			}
			else {
	//			printf("uspesny zapis zaznamu do db2\n");
				logging(DEBUG, "uspesny zapis zaznamu do db2\n");
			}
		}
	}
	
//	printf("mail->head:\n%s\n", new_email->head);
//	printf("mail->body:\n%s\n", new_email->body);
//	printf("mail->hash:\n%s\n", new_email->hash);
//	printf("mail->to:\n%s\n", new_email->to);
//	printf("mail->homedir:\n%s\n", new_email->homedir);
//	printf("mail->size:\n%d\n",new_email->size);

	//TODO uvolnit alokovanu pamat
	free((void *) conf_struct);
	free((void *) new_email->to);
	free((void *) new_email->head);
	free((void *) new_email->hash);
	free((void *) new_email->body);
	free((void *) new_email->filepath);
//	free((void *) new_email->homedir); 		//num_chuck erorr!!!
	free((void *) new_email);
	stop_log();

	return 0;
}
