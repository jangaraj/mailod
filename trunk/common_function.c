#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "const.h"
#include "common_function.h"

int readconf( char conffile[], config *conf ) {
	FILE *fp;
	char buf[512];
	int  line = 0;
	fp = fopen( conffile, "r" );
	if( fp == NULL ) {
		fprintf(stderr,"Error, fopen config file: %s\n", conffile );
		return 0;
	}
	while( (fgets(buf, 512, fp)) != NULL) {
		char *parm;
		char *val;

		line++;
		strip(buf);
		if(buf[0]=='#' || buf[0]=='\0' ) continue;
		parm = strtok( buf, " " );
		val = strtok( NULL, "" );
		if((parm == NULL) || (val == NULL)) {
			fprintf(stderr,"Error, invalid config line number: %d\n", line);
			if(parm != NULL) {
				fprintf( stderr,"Error,  %s does not have a value!\n", parm );
			}
			fclose(fp);
			return 1;
		}
		strip(parm);
		strip(val);
		if((strcasecmp(DB_DRIVER, parm))==0) {
			strncpy( conf->db_driver, val, 200 );
		}
		if( (strcasecmp(DB_HOST, parm)) == 0 ) {
			strncpy( conf->db_host, val, 1024 );
		}
		if((strcasecmp(DB_USERNAME, parm))==0) {
			strncpy( conf->db_username, val, 200 );
		}
		if( (strcasecmp(DB_PASSWORD, parm)) == 0 ) {
			strncpy( conf->db_password, val, 1024 );
		}
		if((strcasecmp(DB_NAME, parm))==0) {
			strncpy( conf->db_name, val, 200 );
		}
		if( (strcasecmp(TIME_WINDOW, parm)) == 0 ) {
			conf->time_window = atoi(val);
		}
		if( (strcasecmp(LOG_LEVEL, parm)) == 0 ) {
			conf->log_level = atoi(val);
		}
	}
	fclose(fp);
	/*
	if( !conf->db_driver ) {
		fprintf(stderr,"Error, missing LDAPSERVER <param> in %s\n", conffile );
		return 1;
	}
	if( !conf->is_ldapPort ) {
		fprintf(stderr,"- missing LDAPPORT <param> in %s\n", conffile );
		return 1;
	}
	if( !conf->is_searchBase ) {
		fprintf(stderr,"- missing SEARCHBASE <param> in %s\n", conffile );
		return 1;
	}*/
	return 0;
}

void strip(char *buf) {
	head( buf );
	tail( buf );
}

void head(char *buf) {
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

void tail(char *buf) {
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
