typedef struct {
	char *head;		//hlavicka emailu
	char *body;		//telo emailu
	char hash;		//hash tela emailu
	char filesystem;//filesystem, kde sa ma ulozit email
	int hardlink;	//ulozit ako hardlinku
} email;

email *readmail(void);

