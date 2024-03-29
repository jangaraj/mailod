/*
 * mailod: optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include <stdio.h>
#include <dbi/dbi.h>
#include "email.h"
#include "common_function.h"
#include "database_function.h"
#include "logging.h"

dbi_conn connect_db(config *conf)
{
	dbi_conn conn;

	dbi_initialize(NULL);
    conn = dbi_conn_new(conf->db_driver);
    dbi_conn_set_option(conn, "host", conf->db_host);
    dbi_conn_set_option(conn, "username", conf->db_username);
	dbi_conn_set_option(conn, "password", conf->db_password);
	dbi_conn_set_option(conn, "dbname", conf->db_name);
	dbi_conn_set_option(conn, "encoding", "UTF-8");
    if (dbi_conn_connect(conn) < 0) {
		logging(DEBUG,"Could not connect. Please check the option settings\n");
		return NULL;
	}

	return conn;
}

int disconnect_db(dbi_conn conn)
{
	dbi_conn_close(conn);
	dbi_shutdown();

	return 0;
}

email *select_by_hash(dbi_conn conn, char hash_value[], int time_window)
{
    dbi_result result;
	email *ident_email;
	char sql[1024];

	ident_email=NULL;
	//selekujem vzdy najstarsi mozny
	sprintf(sql,"SELECT * FROM `mailod` WHERE `body_hash`='%s' AND `timestamp`>(NOW()+INTERVAL -%d MINUTE) ORDER BY `email_id` LIMIT 1;",hash_value, time_window);
	logging(DEBUG,"selectSQL: %s\n",sql);
   	result = dbi_conn_query(conn,sql);
   	if (result) {
		if((dbi_result_get_numrows(result))<1) return NULL;		//0 selected rows
		if((ident_email=(email *) malloc(sizeof(email)))==NULL) {
			logging(DEBUG,"Error, mallock ident_email\n");
			return NULL;
		}
		while (dbi_result_next_row(result)) {
  			ident_email->filepath = (char *) dbi_result_get_string_copy(result, "filepath");
			ident_email->id = dbi_result_get_long(result,"email_id");
			ident_email->inode = dbi_result_get_long(result,"inode");
		}
		dbi_result_free(result);
	}
	
    return ident_email;
}

int insert_email(dbi_conn conn, email *new_email)
{
    dbi_result result;
	char sql[1024];
	sprintf(sql,"INSERT INTO mailod (body_hash, filepath, inode) VALUES (\"%s\",\"%s\",%ld);", new_email->hash, new_email->filepath, new_email->inode);
   	result = dbi_conn_query(conn,sql);
	if(dbi_result_get_numrows_affected(result)!=1) return 1;   // 0 affected rows - not inserted
	dbi_result_free(result);

	return 0;
}

int delete_email(dbi_conn conn, email *ident_email)
{
    dbi_result result;
	char sql[1024];
	sprintf(sql,"DELETE FROM mailod where `email_id`=\"%ld\";", ident_email->id);
   	result = dbi_conn_query(conn,sql);
	if(dbi_result_get_numrows_affected(result)!=1) return 1;   // 0 affected rows - not inserted
	dbi_result_free(result);

	return 0;
}
