typedef struct {
	char db_driver[1024];
	char db_host[1024];
	char db_username[1024];
	char db_password[1024];
	char db_name[1024];
	int time_window;
	int log_level;
} config;

int readconf(char conffile[], config *conf );
void strip(char *buf);
void head(char *buf);
void tail(char *buf); 

