#include "const.h"

typedef struct {
	char *head;		//hlavicka emailu
	char *body;		//telo emailu
	char *hash;		//hash tela emailu
	char *filesystem;//filesystem where stored email file
	char *to;		//local user
	char *homedir;	//homedir of local user
	char *filepath;	//cesta k emailu spolu s nazvom suboru
	int hardlink;	//ulozit ako hardlinku
	int size;		//velkost celeho emailu
	int done;		//flag, ze email je spravne spracovany
	long id;		//id in database
	long inode;		//inode na fs
} email;

/* reading email from input */
email *readmail(void);

/* write email to users maildir */
int write_email(email *new_email);

/* create link or call write_email on error of creating link */
int link_email(email *new_email, email *master_email);

/* generate uniq name of email file for users */
char *make_filepath(email *email);

/* make from filepath only dirpath */
char *make_only_dir(char *filepath);
