#include <stdio.h>
#include <stdlib.h>			//exit
#include <unistd.h>
#include "const.h"			//BUFFER_SIZE
#include "email.h"			//email struct
#include "hash_function.h"	//hash function
#include "common_function.h"	//read conf file
#include "logging.h"		//logging function

int main(int argc, char* argv[]) {
	email *p_new_email;
	config *conf_struct;
	
	if((conf_struct=(config *) malloc(sizeof(config)))==NULL) {
		fprintf(stderr, "Error, malloc config structure\n");
		exit (1);
	}
	if(readconf(CONFIG_FILE, conf_struct)!=0) {
		fprintf(stderr,"Error, parse config file\n");
		exit (1);
	}
	start_log(conf_struct);
	printf("Setting - db_driver: %s\n",conf_struct->db_driver);
	printf("Setting - db_username: %s\n",conf_struct->db_username);
	printf("Setting - db_password: %s\n",conf_struct->db_password);
	printf("Setting - db_name: %s\n",conf_struct->db_name);
	printf("Setting - db_host: %s\n",conf_struct->db_host);
	printf("Setting - time_window: %d\n",conf_struct->time_window);
	printf("Setting - log_level: %d\n",conf_struct->log_level);
	printf("Setting - debug: %d\n",conf_struct->debug);


	p_new_email = readmail();
	p_new_email->hash = hash_text(p_new_email->body);
	
	/*printf(">>>>>>>>>>>>>>>>>>>>>>>>Toto je nacitany mail->head z email struktury:\n%s\n", p_new_email->head);
	printf(">>>>>>>>>>>>>>>>>>>>>>>>Toto je nacitany mail->body z email struktury:\n%s\n", p_new_email->body);
	printf(">>>>>>>>>>>>>>>>>>>>>>>>Toto je nacitany mail->hash z email struktury:\n%s\n", p_new_email->hash);
*/


	//TODO uvolnit alokovanu pamat
	free((void *) p_new_email);
	return 0;
}
