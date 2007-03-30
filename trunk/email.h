#include "const.h"

typedef struct {
	char *head;		//hlavicka emailu
	char *body;		//telo emailu
	char *hash;		//hash tela emailu
	char *filesystem;//filesystem where stored email file
	char *to;		//local user
	char *homedir;	//homedir of local user
	int hardlink;	//ulozit ako hardlinku
} email;

email *readmail(void);

