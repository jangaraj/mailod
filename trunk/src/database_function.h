/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

/* select from database by hash and time */
email *select_by_hash(config *conf, char hash_value[]);

/* insert record about new saved email */
int insert_email(config *conf, email *new_email);

/* delete record from database, email file have max hardlinks */
int delete_email(config *conf, email *ident_email);
