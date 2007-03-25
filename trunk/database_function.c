#include <stdio.h>
#include </home/vlk/bin/include/dbi/dbi.h>
#include "email.h"
#include "common_function.h"
#include "database_function.h"

email *select_by_hash(config *conf, char hash_value[]) {
	dbi_conn conn;
    dbi_result result;
	email *ident_email;
	char *sql;

	ident_email=NULL;
	dbi_initialize(NULL);
    conn = dbi_conn_new(conf->db_driver);
    dbi_conn_set_option(conn, "host", conf->db_host);
    dbi_conn_set_option(conn, "username", conf->db_username);
	dbi_conn_set_option(conn, "password", conf->db_password);
	dbi_conn_set_option(conn, "dbname", conf->db_name);
	//dbi_conn_set_option(conn, "encoding", "UTF-8");
    if (dbi_conn_connect(conn) < 0) {
    	fprintf(stderr,"Could not connect. Please check the option settings\n");
	return NULL;
    }
    else {
		//TODO select podla hashu, time_window, a hardlinks(zavisi od FS)
		//sprintf(sql,"SELECT * FROM `mailod` WHERE `body_hash`='%s'",hash_value);
		//printf("SQL: %s\n",sql);
	   	result = dbi_conn_query(conn,sql);
    	if (result) {
			if((ident_email=(email *) malloc(sizeof(email)))==NULL) {
				fprintf(stderr, "Error, mallock ident_email\n");
				return NULL;
			}
			while (dbi_result_next_row(result)) {
	  			ident_email->filesystem = dbi_result_get_string(result, "filesystem");
	 	  		ident_email->hardlinks = dbi_result_get_uint(result, "number_hardlinks");
			}
			dbi_result_free(result);
		}
		//else {
		//	printf("query zial nic nevratil, mas to pokazene");	
		//}
		dbi_conn_close(conn);
	}
	dbi_shutdown();
    return ident_email;
}
