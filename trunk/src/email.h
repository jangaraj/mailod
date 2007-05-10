/*
 * mailod: optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include "const.h"

typedef struct {
	char *head;			//head of email
	char *body;			//body of email
	char *hash;			//hash of body
	char *to;			//local user
	char *homedir;		//homedir of local user
	char *filepath;		//full path to email file
	int to_uid;			//uid of local user
	int done;			//flag, ze email je spravne spracovany
	long id;			//id in database
	long inode;			//inode na fs
} email;

/* reading email from input */
email *readmail(int input);

/* write email to users maildir */
int write_email(email *new_email);

/* create link or call write_email on error of creating link */
int link_email(email *new_email, email *master_email);

/* generate uniq name of email file for users */
char *make_filepath(email *email);

/* make from filepath only dirpath */
char *make_only_dir(char *filepath);
