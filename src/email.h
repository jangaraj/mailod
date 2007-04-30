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
} email;

email *readmail(void);
int write_email(email *new_email);
int link_email(email *new_email);
