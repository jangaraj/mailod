/*
 * mailod: optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include "common_function.h"

/* start logging to system log */
void start_log(config *conf);

/* stop logging to system log */
void stop_log(void);

/* write message to systme log */
void logging(int ll, const char *fmt, ... );
