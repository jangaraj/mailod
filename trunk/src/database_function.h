/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */
#include <dbi/dbi.h>

/* init dbi driver and connect to database */
dbi_conn connect_db(config *conf);

/* close connect to database and shutdown dbi driver */
int disconnect_db(dbi_conn conn);

/* select from database by hash and time */
email *select_by_hash(dbi_conn conn, char hash_value[],int time_window);

/* insert record about new saved email */
int insert_email(dbi_conn conn, email *new_email);

/* delete record from database, email file have max hardlinks */
int delete_email(dbi_conn conn, email *ident_email);
