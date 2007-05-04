/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include "const.h"
#include "common_function.h"
#include "logging.h"

static char msg_buffer[4096];
static int  log_level;

void start_log(config *conf) {
	stop_log( );
	log_level = conf->log_level;
	if(conf->debug==0) {
		openlog(PROG, LOG_CONS | LOG_PID, LOG_USER );
	}
	else {
		openlog(PROG, LOG_CONS | LOG_PERROR | LOG_PID, LOG_USER );
	}
}

void stop_log(void) {
	closelog( );
}

void logging(int ll, const char *fmt, ... ) {
	char buffer[1024];
	va_list ap;
	va_start( ap, fmt );
	vsnprintf( buffer, sizeof( buffer ), fmt, ap );
	syslog( LOG_INFO, buffer );

	if( ll == DEBUG && log_level >= DEBUG )	{
		syslog( LOG_INFO, buffer );
	}
	else
		if( ll == WARN && log_level >= WARN ) {
			syslog( LOG_INFO, buffer );
		}
		else
			if( ll == INFO && log_level >= INFO ) {
				syslog( LOG_INFO, buffer );
			}
			else
				if( ll == RUN && log_level >= RUN ) {
					syslog( LOG_INFO, buffer );
				}
}

void start_msg(void) {
	msg_buffer[0] = '\0';
}



void log_msg(int ll) {
	logging( ll, msg_buffer );
}



void build_msg(const char *fmt, ... ) {
	char buffer[1024];
	va_list ap;
	va_start( ap, fmt );

	vsnprintf( buffer, sizeof( buffer ), fmt, ap );
	if( strlen( msg_buffer ) >= sizeof( msg_buffer ) ) {
		return;
	}
	else
		if( ( strlen( buffer ) + strlen( msg_buffer ) )  < sizeof( msg_buffer ) ) {
			strcat( msg_buffer, buffer );
		}
		else {
			strncat( msg_buffer, buffer, sizeof( msg_buffer ) - strlen( msg_buffer ) - 1 );
		}
}
