#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
//#include <fstab.h>

#include "email.h"
#include "const.h"

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

email *readmail(void)
{
	char buffer[BUFFER_SIZE], *reading_email_all, *reading_email_head, *reading_email_body, *reading_email_to, *position, *position2;	
	int read_size, nblock, length_position;
	email *read_email;

	nblock=0;

	if((read_email=(email *) malloc(sizeof(email)))==NULL) {
		fprintf(stderr, "Error, malloc of reading email\n");
		exit (1);
	}
	if((reading_email_all=(char *) malloc(sizeof(char)))==NULL) {
		fprintf(stderr,"Error, malloc of char of reading email\n");
		exit (1);
	}
	*(reading_email_all)='\0';
	if((reading_email_head=(char *) malloc(sizeof(char)))==NULL) {
		fprintf(stderr,"Error, malloc of char of reading email\n");
		exit (1);
	}
	*(reading_email_head)='\0';
	while (0<(read_size = read(0, buffer, BUFFER_SIZE))) {
		if(read_size<0) {
		fprintf(stderr, "Error reading input file\n");
			exit (1);
		} 
		if((reading_email_all = (char *) realloc(reading_email_all, ((nblock*BUFFER_SIZE)+read_size+1)*sizeof(char)))==NULL) {
			fprintf(stderr,"Error, realloc by reading text\n");
			exit(1);
		}
		strncat(reading_email_all, buffer, read_size);
		nblock++;							//counter na citaci cyklus - pocet alokovanych blokov
	}
	read_email->size = nblock*BUFFER_SIZE+read_size;
	//divide head from all
	position = strstr(reading_email_all, DIVIDER_HEAD_BODY);
	//position = position+2*sizeof(char);
//	position++;
	if(position==NULL) {
		fprintf(stderr,"Error, dividing email to head and body\n");
		exit (1);
	}
	length_position = position - reading_email_all;
	if((reading_email_head=(char *) realloc(reading_email_head, length_position*sizeof(char)))==NULL) {
		fprintf(stderr,"Error, realloc reading_email_head\n");
		exit (1);
	}
	strncat(reading_email_head, reading_email_all, length_position);
	if((reading_email_body=(char *) malloc((strlen(position)+1)*sizeof(char)))==NULL) {
		fprintf(stderr, "Error, malloc reading_email_body\n");
		exit (1);
	}
	*reading_email_body = '\0';
	strcat(reading_email_body, position+2);
	//determinate to: local user
	position = strstr(reading_email_all, PARSE_TO);
	if(position==NULL) {
		fprintf(stderr,"Error, not found in mail header parse string %s\n",PARSE_TO);
		exit (1);
	}
	//ak je adresat v <mailaddress> najdem zobacik ina prvu " "
	//TODO vytiahnut local usera aj ked nie je v zobacikoch
	if((position=strstr(position,"<"))==NULL) {
		fprintf(stderr,"Error parse local user - only <local@user.tld> is allowed now\n");
		exit (1);
	}
	position2 = strstr(position,"@");
	if(position2==NULL) {
		fprintf(stderr,"Error, in email header - parse string @\n");
		exit (1);
	}
	length_position = position2 - position;
	if((reading_email_to=(char *) malloc(length_position*sizeof(char)))==NULL) {
		fprintf(stderr,"Error, malloc reading_email_to\n");
		exit (1);
	}
	position++;
	*reading_email_to = '\0';
	strncat(reading_email_to, position, length_position-1);
	free((void *) reading_email_all);
	read_email->head = reading_email_head;
	read_email->body = reading_email_body;
	read_email->to = reading_email_to;
	read_email->homedir = (getpwnam(reading_email_to)->pw_dir);

	return read_email;
}

int write_email(email *new_email) 
{
	//	struct fstab *info_file;
//	struct mntentchn *info_file;
	int fw, i;
	if((new_email->filepath = make_filepath(new_email)) == NULL) {
		fprintf(stderr,"Error, generate uniq name of email file\n");
		return 1;
	}
	//unique filename template: time.pid.hostname
	//t - 10 chars
	//getpid - 5 chars
	//hostname MAXHOSTNAMELEN chars
	//otvorenie suboru
	//TODO zamykanie suboru - musi byt? link()
	if((fw = open(new_email->filepath,O_WRONLY|O_CREAT|O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP))==-1) {
		fprintf(stderr,"Error opening email file in user's homedir\n");
		return 1;
	}
	//zapis jednotlivych casti emailu do suboru
	if((i = strlen(new_email->head)) != write(fw, new_email->head,i)) {
		fprintf(stderr,"Error, writing email head\n");
		return 1;
	}
	if((i = strlen(DIVIDER_HEAD_BODY)) != write(fw, DIVIDER_HEAD_BODY,i)) {
		fprintf(stderr,"Error, writing email divider\n");
		return 1;
	}
	if((i = strlen(new_email->body)) != write(fw, new_email->body,i)) {
			fprintf(stderr,"Error, writing email body\n");
	}
	//zavretie suboru
	if(close(fw)!=0) {
		fprintf(stderr,"Error, close email file in user's homedir\n");
		return 1;
	}
	//TODO vytiahnut aj filesysytem na ktorom je ulozeny aby som ho mohol insertnut do db
	//nefungen
	//info_file = getfsfile(filepath);
//	printf("fs_spec %s\n,fs_file %s\n",info_file->fs_spec,info_file->fs_file);



	return 0;
}

int link_email(email *new_email, email *master_email)
{
	int rval, lval;
	rval = access(master_email->filepath, F_OK);
	if (rval == 0) {
		if((new_email->filepath = make_filepath(new_email)) == NULL) {
			fprintf(stderr,"Error, generate uniq name of email file\n");
			return 1;
		}
		if((lval = link(master_email->filepath, new_email->filepath)) != 0) {
			printf("Error pri vytvarani linku\n");
			new_email->done = 2;
			switch (errno) {
				case EACCES:
					fprintf(stderr,"Error, denied search permision of path\n");
					break;
				case EMLINK:
					printf("prekrocil som max pocet hardliniek na subor\n");
					//TODO delet ident email and select new ident
					//vymazat zaznam ident z db
					new_email->done = EMLINK;
					break;
				case ENAMETOOLONG:
					fprintf(stderr,"Error, name of email file is too long\n");
					break;
				case EPERM:
					fprintf(stderr,"Error, path %s is a directory and does not have privileges of using link()\n",new_email->filepath);
					break;
				case EROFS:
					fprintf(stderr,"Error, read-only file system\n");
					break;
				case EXDEV:
					fprintf(stderr,"Error, path1 %s and path2 %s are on different file systems\n",master_email->filepath,new_email->filepath);
					break;
				default:
					fprintf(stderr,"Error, link is not created\n");
					break;
			}

		}
		else printf("Uspesne vytvoreny link\n");
	} //rval == 0 - file exist
	else {
		//TODO skus cur adresar prehladat
		printf("Nenasiel som mail v new\n");
		// TODO ak nenajdes nastav flag na 2 ze nelinkovany a return 1 , nech sa uz v hlavnom programe write normalne
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
		return NULL;
	}
	do {							//generovanie nazvu suboru, ktory neexistuje
		t = time((time_t*)0);
		//TODO safehostname - bez badchars
		gethostname(name,MAXHOSTNAMELEN);
		name[MAXHOSTNAMELEN-1] = '\0';
		sprintf(filepath,"%s%s%d.%d.",email->homedir,INBOX,(int) t,getpid());
		if(strlen(filepath)+strlen(name)>17+MAXHOSTNAMELEN) {
			//realloc filepath na vacsiu velkost
			if((filepath = (char *) realloc(filepath, strlen(filepath)+strlen(name)))==NULL) {
				fprintf(stderr,"Error realloc filepath\n");
				return NULL;
			}
		}
		strcat(filepath,name);
//printf("Meno suboru bude: %s\n",filepath);
	} while(stat(filepath,&filestat)!=-1);  //generujem meno suboru pokial este neexistuje

	return filepath;
}
