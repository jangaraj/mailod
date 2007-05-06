/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "const.h"
#include "common_function.h"
#include "logging.h"

int readconf( char conffile[], config *conf ) 
{
	FILE *fp;
	char buf[512];
	int  line = 0;
	fp = fopen( conffile, "r" );
	if( fp == NULL ) {
		logging(DEBUG,"Error by fopen config file %s\n", conffile );
		return 1;
	}
	while( (fgets(buf, 512, fp)) != NULL) {
		char *parm;
		char *val;

		line++;
		strip(buf);
		if(buf[0]=='#' || buf[0]=='\0' ) continue;
		parm = strtok( buf, "\t" );
		val = strtok( NULL, "" );
		if((parm == NULL) || (val == NULL)) {
			logging(DEBUG,"Error, invalid config line number: %d\n", line);
			if(parm != NULL) {
				logging(DEBUG,"Error, %s does not have a value!\n", parm);
			}
			fclose(fp);
			return 1;
		}
		strip(parm);
		strip(val);
		if((strcasecmp(DB_DRIVER, parm))==0) {
			strcpy( conf->db_driver, val );
		}
		if( (strcasecmp(DB_HOST, parm)) == 0 ) {
			strcpy( conf->db_host, val );
		}
		if((strcasecmp(DB_USERNAME, parm))==0) {
			strcpy( conf->db_username, val );
		}
		if( (strcasecmp(DB_PASSWORD, parm)) == 0 ) {
			strcpy( conf->db_password, val );
		}
		if((strcasecmp(DB_NAME, parm))==0) {
			strcpy( conf->db_name, val );
		}
		if( (strcasecmp(TIME_WINDOW, parm)) == 0 ) {
			conf->time_window = atoi(val);
			if(conf->time_window<0) {
				logging(DEBUG,"Error, <param> %s at line %d has invalid value %d\n", TIME_WINDOW, line, conf->time_window);
				return 1;
			}
		}
		if( (strcasecmp(LOG_LEVEL, parm)) == 0 ) {
			conf->log_level = atoi(val);
			if((conf->log_level<0) || (conf->log_level>6)) {
				logging(DEBUG,"Error, <param> %s at line %d has invalid value %d\n", LOG_LEVEL, line, conf->log_level);
				return 1;
			}

		}
		if( (strcasecmp(LOG_STDERR, parm)) == 0 ) {
			conf->debug = atoi(val);
			if((conf->debug<0) || (conf->debug>1)) {
				logging(DEBUG,"Error, <param> %s at line %d has invalid value %d\n", LOG_STDERR, line, conf->debug);

				return 1;
			}
		}
		if( (strcasecmp(CON_PORT, parm)) == 0 ) {
			conf->port = atoi(val);
			if((conf->port<0) || (conf->port>65535)) {
				logging(DEBUG,"Error, <param> %s at line %d has invalid value %d\n", CON_PORT, line, conf->debug);
				return 1;
			}
		}
		if( (strcasecmp(CON_BACK, parm)) == 0 ) {
			conf->backlog = atoi(val);
			if(conf->backlog<0) {
				logging(DEBUG,"Error, <param> %s at line %d has invalid value %d\n", CON_BACK, line, conf->debug);
				return 1;
			}
		}

	}
	fclose(fp);
	//TODO ak daco potrebne nensatavene default hodnotu z const.h
	/*
	if( !conf->db_driver ) {
		fprintf(stderr,"Error, missing db_driver <param> in %s\n", conffile );
		return 1;
	}
	}*/
	return 0;
}

void strip(char *buf) 
{
	head( buf );
	tail( buf );
}

void head(char *buf) 
{
	int l;

	if( buf == NULL ) return;
	l = strlen( buf );
	while((buf[0] <= 0x20 ) && (l > 0)) {
		int x;
		for( x = 0; x < l; x++ )
			buf[x] = buf[x+1];
		buf[l-1] = '\0';
		l = strlen(buf);
	}
}

void tail(char *buf) 
{
	int l;
	int i;

	if(buf == NULL) return;
	l = strlen( buf ) - 1;
	for( i = l; i >= 0; i-- ) {
		if( buf[i] <= 0x20 )
			buf[i] = '\0';
		else
			break;
	}
}

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

