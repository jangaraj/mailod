/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#define PROG "Mailod-Garaj"			//name of program
#define VERS "0.0.21"				//version of program
#define BUFFER_SIZE 1024			//size of reading( writing email buffer
#define DIVIDER_HEAD_BODY "\n\n"	//divider email head and email body
#define CONFIG_FILE "/home/vlk/ggmailod/src/setting_true"	//name of config file
#define DB_DRIVER "database_driver" 
#define DB_HOST	 "database_host"
#define DB_USERNAME "database_username"
#define DB_PASSWORD "database_password"
#define DB_NAME	 "database_name"
#define TIME_WINDOW "time_window"
#define LOG_LEVEL "log_level"
#define LOG_STDERR "debug"
#define CON_PORT "port"				//MYPORT in setting
#define CON_BACK "backlog"			//BACKLOG in setting
#define DEBUG	 3
#define WARN	 2
#define INFO	 1
#define RUN	 0
//#define LENGTH_FILESYSTEM 254		//lenght of string filesystem
#define MYPORT 6666					//the port users will be connecting to
#define BACKLOG 10	 				//how many pending connections queue will hold
#define PARSE_TO "To: "				//parse string to determinate local user
#define PARSE_MAILDIR "X-Mailod-dir"//special email header - email saved in maildir/<value> of this <para> - inserted by MDA(procmail, maildrop)
#define MAILDIR "/.maildir"			//name of maildir - depend on setting postfix
#define CUR "cur"					//folder of readed emails
#define NEW "/new"					//folder if new emails
#define INBOX "/.maildir/new/"		//for stat in write_email
