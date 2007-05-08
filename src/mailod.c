/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <dbi/dbi.h>
#include "const.h"
#include "email.h"
#include "hash_function.h"
#include "common_function.h"
#include "logging.h"
#include "database_function.h"

int main(void)
{
	int sockfd, new_fd;  			//listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr;		//my address information
	struct sockaddr_in their_addr; 	//connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	email *new_email, *ident_email;
	config *conf_struct;
	dbi_conn conn;

	//drop root rights and set UID_DAEMON_USER
	if((setuid(UID_DAEMON_USER))==-1) {
		fprintf(stderr,"Error, %s\n",strerror(errno));
		exit (1);
	}
	//reading config file
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
	close(1);								//closing stdout for daemoize
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	my_addr.sin_family = AF_INET;		 	//host byte order
	my_addr.sin_port = htons(conf_struct->port);	 	//short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; 	//automatically fill with my IP
	memset(&(my_addr.sin_zero), '\0', 8); 	//zero the rest of the struct
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, conf_struct->backlog) == -1) {
		perror("listen");
		exit(1);
	}
	sa.sa_handler = sigchld_handler; 		//reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	while(1) {  							//main accept() loop
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
			perror("accept");
			continue;
		}
		logging(DEBUG,"got connection from %s\n",inet_ntoa(their_addr.sin_addr));
		if (!fork()) { 						//this is the child process
			close(sockfd); 					//child doesn't need the listener
			logging(DEBUG,"USPESNE FORKNUTY");
			if((new_email = readmail(new_fd)) == NULL) {
				logging(DEBUG, "Error, reading and processing input email\n");
				exit (1);
			}
			close(new_fd);
			if(((conn = connect_db(conf_struct)) != NULL) && (new_email->hash = hash_text(new_email->body))) {
				//connect to database and hashing body text - OK
				if((ident_email = select_by_hash(conn, new_email->hash, conf_struct->time_window))==NULL) {
					logging(DEBUG, "nemam ident email. Idem ho zapisat to filesystema a do DB.\n");
					if(write_email(new_email)!=0) {
						logging(DEBUG, "Error standard-writing email.\n");
					}
					else 
					{	
					  	new_email->done = 0;
						logging(DEBUG, "uspesny zapis email suboru\n");
						if((insert_email(conn, new_email))!=0) {
							logging(DEBUG, "Error standard-writing email.\n");
						}
						else {
							logging(DEBUG, "uspesny zapis zaznamu do db\n");
						}
					}
				}
				else {
					logging(DEBUG, "Linkujem podla ident emailu\n");
					do {
						if(link_email(new_email, ident_email)!=0) {
							logging(DEBUG, "Error, linking email\n");
							if(new_email->done == EMLINK) { 
								logging(DEBUG, "Idem zmazat zaznam, ktory uz ma full hardlinks\n");
								if((delete_email(conn, ident_email))!=0) {
									logging(DEBUG, "Error, deleting full hardlinks record from database\n");
								}
							}	
							free((void *) ident_email);
							if((ident_email = select_by_hash(conn, new_email->hash, conf_struct->time_window))==NULL) {
								//not found ident email in database, break loop
								logging(DEBUG, "Nenasiel som dalsi vhodny email z db\n");
								break;
							}
						}
					} while (new_email->done == EMLINK);  //if fail on max number hardlinks - next loop

					free((void *) ident_email);
				}	
			}
			if(new_email->done!=0) {				//end testing - done = 0 = email is good processed
				if(write_email(new_email)!=0) {
					logging(DEBUG, "Error fail-writing email\n");
					exit (1);						//critical fail input email
				}
				else 
				{	
					logging(DEBUG, "uspesny zapis email suboru2\n");
					if((insert_email(conn, new_email))!=0) {
						logging(DEBUG, "Error, inserting email to database2\n");
					}
					else {
						logging(DEBUG, "uspesny zapis zaznamu do db2\n");
					}
				}	
			}

			if(conf_struct == NULL) logging(DEBUG,"NUL conf_stuct\n");
			if(new_email->to == NULL) logging(DEBUG,"NUL new_email->to\n");
			if(new_email->head == NULL) logging(DEBUG,"NUL new_email->head\n");
			if(new_email->hash == NULL) logging(DEBUG,"NUL new_email->hash\n");
			if(new_email->body == NULL) logging(DEBUG,"NUL new_email->body\n");
			if(new_email == NULL) logging(DEBUG,"NUL new_email\n");
			if(new_email->filepath == NULL) logging(DEBUG,"NUL new_email->filepath\n");
			free((void *) conf_struct);
			free((void *) new_email->to);
			free((void *) new_email->head);
			free((void *) new_email->hash);
			free((void *) new_email->body);
			free((void *) new_email->filepath);
			free((void *) new_email);
			stop_log();
			exit (0);
		}
	close(new_fd);  								// parent doesn't need this
	}

	return 0;
}
