/*
 * mailod:  optimalization of the usage disk
 * Author: Jan Garaj	
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <acl/libacl.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include "email.h"
#include "const.h"
#include "logging.h"

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

email *readmail(int input)
{
	char buffer[BUFFER_SIZE], *reading_email_all, *reading_email_head, *reading_email_body, *reading_email_to, *position, *position2;	
	int read_size, nblock, length_position;
	email *read_email;

	nblock=0;

	if((read_email=(email *) malloc(sizeof(email)))==NULL) {
		logging(DEBUG,"Error, malloc of reading email\n");
		return NULL;;
	}
	if((reading_email_all=(char *) malloc(sizeof(char)))==NULL) {
		logging(DEBUG,"Error, malloc of char of reading email\n");
		return NULL;
	}
	*(reading_email_all)='\0';
	if((reading_email_head=(char *) malloc(sizeof(char)))==NULL) {
		logging(DEBUG,"Error, malloc of char of reading email\n");
		return NULL;
	}
	*(reading_email_head)='\0';
	while (0<(read_size = read(input, buffer, BUFFER_SIZE))) {
		if(read_size<0) {
			logging(DEBUG,"Error reading input file\n");
			return NULL;
		} 
		if((reading_email_all = (char *) realloc(reading_email_all, ((nblock*BUFFER_SIZE)+read_size+1)*sizeof(char)))==NULL) {
			logging(DEBUG,"Error, realloc by reading text\n");
			return NULL;
		}
		strncat(reading_email_all, buffer, read_size);
		nblock++;							//counter na citaci cyklus - pocet alokovanych blokov
	}
	//divide head from all
	position = strstr(reading_email_all, DIVIDER_HEAD_BODY);
	if(position==NULL) {
		logging(DEBUG,"Error, dividing email to head and body\n");
		return NULL;
	}
	length_position = position - reading_email_all;
	if((reading_email_head=(char *) realloc(reading_email_head, length_position*sizeof(char)))==NULL) {
		logging(DEBUG,"Error, realloc reading_email_head\n");
		return NULL;
	}
	strncat(reading_email_head, reading_email_all, length_position);
	if((reading_email_body=(char *) malloc((strlen(position)+1)*sizeof(char)))==NULL) {
		logging(DEBUG,"Error, realloc reading_email_body\n");
		return NULL;
	}
	*reading_email_body = '\0';
	strcat(reading_email_body, position+2);
	//determinate to: local user
	position = strstr(reading_email_all, PARSE_TO);
	if(position==NULL) {
		logging(DEBUG,"Error, not found in mail header parse string %s\n",PARSE_TO);
		return NULL;
	}
	if((position=strstr(position," "))==NULL) {
		logging(DEBUG,"Error parse local user - only \"To: local@user.tld is\" allowed now\n");
		return NULL;
	}
	position++;
	position2 = strstr(position,"@");
	if(position2==NULL) {
		logging(DEBUG,"Error, in email header - parse string @\n");
		return NULL;
	}
	length_position = position2 - position;
	if((reading_email_to=(char *) malloc(length_position*sizeof(char)))==NULL) {
		logging(DEBUG,"Error, malloc reading_email_to\n");
		return NULL;
	}
	*reading_email_to = '\0';
	strncat(reading_email_to, position, length_position);
	if((strstr(reading_email_to,"<"))!=NULL) {
		reading_email_to++;
	}
	free((void *) reading_email_all);
	read_email->head = reading_email_head;
	read_email->body = reading_email_body;
	read_email->to = reading_email_to;
	logging(DEBUG,"EMIL TO JE %s\n",reading_email_to);
	if(getpwnam(reading_email_to) == NULL) {
		logging(DEBUG,"Error, not determinated homedir for %s\n",reading_email_to);
		return NULL;
	}
	else {
		read_email->homedir = getpwnam(reading_email_to)->pw_dir;
		read_email->to_uid = getpwnam(reading_email_to)->pw_uid;
	}
	read_email->done = 1;

	return read_email;
}

int write_email(email *new_email) 
{
	int fw, i;
	struct stat filestat;
	mode_t umask_bak;
	acl_t acl;					/* ACL information */
	acl_entry_t entry_p;		/* ACL entry */	
	acl_permset_t permset;		/* Permissions */

	if((new_email->filepath = make_filepath(new_email)) == NULL) {
		logging(DEBUG,"Error, generate uniq name of email file\n");
		return 1;
	}
	//unique filename template: time.pid.hostname
	//t - 10 chars
	//getpid - 5 chars
	//hostname MAXHOSTNAMELEN chars
	//otvorenie suboru
	//TODO zamykanie suboru - musi byt? link()
	
	//backup umask setting
	umask_bak = umask(0);
	//set umask
	//umask(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IRUSR);
	//umask(S_IRUSR | S_IWUSR );
	if((fw = open(new_email->filepath,O_WRONLY|O_CREAT|O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))==-1) {
		logging(DEBUG,"Error opening email file in user's homedir\n");
		return 1;
	}
	//writing part of emails to file
	if((i = strlen(new_email->head)) != write(fw, new_email->head,i)) {
		logging(DEBUG,"Error, writing email head\n");
		return 1;
	}
	if((i = strlen(DIVIDER_HEAD_BODY)) != write(fw, DIVIDER_HEAD_BODY,i)) {
		logging(DEBUG,"Error, writing email divider\n");
		return 1;
	}
	if((i = strlen(new_email->body)) != write(fw, new_email->body,i)) {
			logging(DEBUG,"Error, writing email body\n");
			return 1;
	}
	//closing file
	if(close(fw)!=0) {
		logging(DEBUG,"Error, close email file in user's homedir\n");
		return 1;
	}
	//check file
	if(stat(new_email->filepath,&filestat)==-1) {
		logging(DEBUG,"Error checking for existing email file\n");
	}
	//restore umask
	umask(umask_bak);
	new_email->inode = filestat.st_ino;
	//setting acl
	acl = acl_get_file(new_email->filepath,ACL_TYPE_ACCESS);
	if(!acl) {
		logging(DEBUG,"In file %s not found acl's\n");
	}
	if(acl_create_entry(&acl,&entry_p) == -1) {
		logging(DEBUG,"Error, adding acl entry: %s\n",strerror(errno));
	}
	if(acl_get_permset(entry_p, &permset) == -1) {
		logging(DEBUG,"Error, getting permset of acl: %s\n",strerror(errno));
	}
	if(acl_add_perm(permset, ACL_READ) == -1) {
		logging(DEBUG,"Error, adding permset of acl: %s\n",strerror(errno));
	}
	if(acl_set_tag_type(entry_p, ACL_USER) == -1) {
		logging(DEBUG,"Error, setting type of acl: %s\n",strerror(errno));
	}
	if(acl_set_qualifier(entry_p, &new_email->to_uid) == -1) {
		logging(DEBUG,"Error, seting qualifier of acl: %s\n",strerror(errno));
	}
	if(acl_set_permset(entry_p, permset) == -1) {
		logging(DEBUG,"Error, seting permset of acl: %s\n",strerror(errno));	
	}
	if (!acl_valid(acl)) {
		logging(DEBUG,"Error, not valid acl\n");
	}
	if(acl_set_file(new_email->filepath,ACL_TYPE_ACCESS,acl) == -1) {
		logging(DEBUG,"Error, set new acl for file %s\n",new_email->filepath);
//		if(errno == EINVAL) new_email->done = 2;
	}
	return 0;
}

int link_email(email *new_email, email *master_email)
{
	int rval, lval;
	DIR *dp;
	struct dirent *dir;
	acl_t acl;					/* ACL information */
	acl_entry_t entry_p;		/* ACL entry */	
	acl_permset_t permset;		/* Permissions */

	if((new_email->filepath = make_filepath(new_email)) == NULL) {
			logging(DEBUG,"Error, generate uniq name of email file\n");
			return 1;
	}
	//checking existence of email file
	rval = access(master_email->filepath, F_OK);
	if (rval == 0) {
		//setting acl
		acl = acl_get_file(master_email->filepath,ACL_TYPE_ACCESS);
		if(!acl) {
			logging(DEBUG,"In file %s not found acl's\n");
		}
		if(acl_create_entry(&acl,&entry_p) == -1) {
			logging(DEBUG,"Error, adding acl entry: %s\n",strerror(errno));
		}
		if(acl_get_permset(entry_p, &permset) == -1) {
			logging(DEBUG,"Error, getting permset of acl: %s\n",strerror(errno));
		}
		if(acl_add_perm(permset, ACL_READ) == -1) {
			logging(DEBUG,"Error, adding permset of acl: %s\n",strerror(errno));
		}
		if(acl_set_tag_type(entry_p, ACL_USER) == -1) {
			logging(DEBUG,"Error, setting type of acl: %s\n",strerror(errno));
		}
		if(acl_set_qualifier(entry_p, &new_email->to_uid) == -1) {
			logging(DEBUG,"Error, seting qualifier of acl: %s\n",strerror(errno));
		}
		if(acl_set_permset(entry_p, permset) == -1) {
			logging(DEBUG,"Error, seting permset of acl: %s\n",strerror(errno));	
		}
		if (!acl_valid(acl)) {
			logging(DEBUG,"Error, not valid acl\n");
		}
		if(acl_set_file(master_email->filepath,ACL_TYPE_ACCESS,acl) == -1) {
			logging(DEBUG,"Error, set new acl for file %s\nerrno: %s\n",new_email->filepath,strerror(errno));
			if(errno == EINVAL) new_email->done = EINVAL;
		}
		if(new_email->done != EINVAL) {
			logging(DEBUG,"link(%s,%s);\n",master_email->filepath, new_email->filepath);
			if((lval = link(master_email->filepath, new_email->filepath)) != 0) {
				logging(DEBUG,"Error pri vytvarani linku\n");
				new_email->done = 2;
				switch (errno) {
					case EACCES:
						logging(DEBUG,"Error, denied search permision of path\n");
						break;
					case EMLINK:
						logging(DEBUG,"prekrocil som max pocet hardliniek na subor\n");
						new_email->done = EMLINK;
						break;
					case ENAMETOOLONG:
						logging(DEBUG,"Error, name of email file is too long\n");
						break;
					case EPERM:
						logging(DEBUG,"Error, path %s is a directory and does not have privileges of using link()\n",new_email->filepath);
						break;
					case EROFS:
						logging(DEBUG,"Error, read-only file system\n");
						break;
					case EXDEV:
						logging(DEBUG,"Error, path1 %s and path2 %s are on different file systems\n",master_email->filepath,new_email->filepath);
						break;
					default:
						logging(DEBUG,"Error, link is not created\n");
						break;
				}

			}
			else {
				new_email->done = 0;	
				logging(DEBUG,"Uspesne vytvoreny link\n");
			}
		}
		else {
			return 1;		//error cl
		}
	} //rval == 0 - file exist
	else {
		logging(DEBUG,"Nenasiel som mail v new pokusim sa ho najst v cur\n");
		if((master_email->filepath = make_only_dir(master_email->filepath)) == NULL) {
			logging(DEBUG,"Error, making only dir from filepath\n");
			return 1;
		}
		strcat(master_email->filepath,"cur");
		logging(DEBUG,"Idem otvarat dir %s\n",master_email->filepath);
 		if ((dp = opendir(master_email->filepath)) == NULL) {
		 		logging(DEBUG,"Error, cannot open directory\n");
			 return 1;
		}
		while ((dir = readdir(dp)) != NULL)	{
	       if (dir->d_ino == 0)
	             continue;
	       if(master_email->inode == dir->d_ino)
			 break;
	    }
		if((closedir(dp) != 0)) {
	 		logging(DEBUG,"Error, closedir(%s)\n",master_email->filepath);
		}
		// link with file from cur
		logging(DEBUG,"Nasiel som subor s rovnakym inodom v cur, idem skusit linkovat\n");
		master_email->filepath = (char *) realloc(master_email->filepath, (strlen(master_email->filepath)+strlen(dir->d_name)+1)*sizeof(char));
		strcat(master_email->filepath,"/");
		strcat(master_email->filepath,dir->d_name);
		//setting acl
		acl = acl_get_file(master_email->filepath,ACL_TYPE_ACCESS);
		if(!acl) {
			logging(DEBUG,"In file %s not found acl's\n");
		}
		if(acl_create_entry(&acl,&entry_p) == -1) {
			logging(DEBUG,"Error, adding acl entry: %s\n",strerror(errno));
		}
		if(acl_get_permset(entry_p, &permset) == -1) {
			logging(DEBUG,"Error, getting permset of acl: %s\n",strerror(errno));
		}
		if(acl_add_perm(permset, ACL_READ) == -1) {
			logging(DEBUG,"Error, adding permset of acl: %s\n",strerror(errno));
		}
		if(acl_set_tag_type(entry_p, ACL_USER) == -1) {
			logging(DEBUG,"Error, setting type of acl: %s\n",strerror(errno));
		}
		if(acl_set_qualifier(entry_p, &new_email->to_uid) == -1) {
			logging(DEBUG,"Error, seting qualifier of acl: %s\n",strerror(errno));
		}
		if(acl_set_permset(entry_p, permset) == -1) {
			logging(DEBUG,"Error, seting permset of acl: %s\n",strerror(errno));	
		}
		if (!acl_valid(acl)) {
			logging(DEBUG,"Error, not valid acl\n");
		}
		if(acl_set_file(master_email->filepath,ACL_TYPE_ACCESS,acl) == -1) {
			logging(DEBUG,"Error, set new acl for file %s\n",new_email->filepath);
			if(errno == EINVAL) new_email->done = EINVAL;
		}
		if(new_email->done != EINVAL) {
			logging(DEBUG,"link(%s,%s);\n",master_email->filepath, new_email->filepath);
			if((lval = link(master_email->filepath, new_email->filepath)) != 0) {
				logging(DEBUG,"Error pri vytvarani linku\n");
				new_email->done = 2;
				switch (errno) {
					case EACCES:
						logging(DEBUG,"Error, denied search permision of path\n");
						break;
					case EMLINK:
						logging(DEBUG,"prekrocil som max pocet hardliniek na subor\n");
						new_email->done = EMLINK;
						break;
					case ENAMETOOLONG:
						logging(DEBUG,"Error, name of email file is too long\n");
						break;
					case EPERM:
						logging(DEBUG,"Error, path %s is a directory and does not have privileges of using link()\n",new_email->filepath);
						break;
					case EROFS:
						logging(DEBUG,"Error, read-only file system\n");
						break;
					case EXDEV:
						logging(DEBUG,"Error, path1 %s and path2 %s are on different file systems\n",master_email->filepath,new_email->filepath);
						break;
					default:
						logging(DEBUG,"Error, link is not created2: %s\n",strerror(errno));
						break;
				}

			}
			else {
				new_email->done = 0;	
				logging(DEBUG,"Uspesne vytvoreny link v cur-e\n");
			}
		}
		else {
			return 1;				//error acl
		}
	}
	return 0;
}

char *make_filepath(email *email)
{
	char *filepath;
	static time_t t;
	static char name[MAXHOSTNAMELEN];
	struct stat filestat;

	if((filepath = (char *) malloc((strlen(email->homedir)+strlen(INBOX)+17+MAXHOSTNAMELEN)*sizeof(char)))==NULL) {
		fprintf(stderr,"Error malloc filename\n");
		logging(DEBUG,"Error malloc filename\n");
		return NULL;
	}
	do {								//generating name of nonexisting file
		t = time((time_t*)0);
		gethostname(name,MAXHOSTNAMELEN);
		name[MAXHOSTNAMELEN-1] = '\0';
		sprintf(filepath,"%s%s%d.%d.",email->homedir,INBOX,(int) t,getpid());
		if(strlen(filepath)+strlen(name)>17+MAXHOSTNAMELEN) {
			if((filepath = (char *) realloc(filepath, strlen(filepath)+strlen(name)))==NULL) {
				logging(DEBUG,"Error realloc filepath\n");
				return NULL;
			}
		}
		strcat(filepath,name);
	} while(stat(filepath,&filestat)!=-1); 

	return filepath;
}

char *make_only_dir(char *filepath)
{
	char *new;
	int i;

	if((new=(char *) malloc((strlen(filepath)+6)*sizeof(char))) == NULL) {
		logging(DEBUG,"Error, malloc new filepath\n");
		return NULL;
	}
	strcpy(new, rindex(filepath, '/'));
	i = strlen(filepath) - strlen(new) - 3; 	//without new
	strncpy(new,filepath,i);	
	*(new+i) = '\0';
	free((void *) filepath);

	return new;
}
