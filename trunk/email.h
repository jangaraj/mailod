#include "const.h"

typedef struct {
	char *head;		//hlavicka emailu
	char *body;		//telo emailu
	char *hash;		//hash tela emailu
	char *filesystem;
	char *to;		//local user
	int hardlink;	//ulozit ako hardlinku
	int hardlinks;
} email;

email *readmail(void);

