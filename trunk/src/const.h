/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#define PROG "mailod"							//name of program - for logging
#define VERS "0.1.3"							//version of program
#define BUFFER_SIZE 1024						//size of reading( writing email buffer
#define DIVIDER_HEAD_BODY "\n\n"				//divider email head and email body
#define CONFIG_FILE "/etc/postfix/mailod.conf"	//name of config file
#define DB_DRIVER "database_driver" 
#define DB_HOST	 "database_host"
#define DB_USERNAME "database_username"
#define DB_PASSWORD "database_password"
#define DB_NAME	 "database_name"
#define TIME_WINDOW "time_window"
#define LOG_LEVEL "log_level"
#define LOG_STDERR "debug"
#define CON_PORT "port"				
#define CON_BACK "backlog"			
#define DEBUG	 3
#define WARN	 2
#define INFO	 1
#define RUN	 0
#define PARSE_TO "\nTo: "						//parse string to determinate local user
#define MAILDIR "/.maildir"						//name of maildir - depend on setting postfix
#define CUR "cur"								//folder of readed emails
#define NEW "/new"								//folder if new emails
#define INBOX "/.maildir/new/"					//for stat in write_email
#define UID_DAEMON_USER	207						//uid for daemon user - depend on rights of maildir directory
#define XORIGINAL "X-Original-To: "				//hidden line in header
#define DELIVERED "Delivered-TO: "				//hidden line in header
#define TO	"To: "								//hidden line in header
