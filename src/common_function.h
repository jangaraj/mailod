/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#ifndef COMMON_H_EXIST
#define COMMON_H_EXIST

typedef struct {
	char db_driver[1024];
	char db_host[1024];
	char db_username[1024];
	char db_password[1024];
	char db_name[1024];
	int time_window;
	int log_level;
	int debug;
	int port;
	int backlog;
} config;

/* read configuration from file to conf struct */
int readconf(char conffile[], config *conf );

/* strip string */
void strip(char *buf);

/* head string */
void head(char *buf);

/* tail string */
void tail(char *buf); 

#endif /* COMMON_H_EXIST */
