#include <stdio.h>
#include <stdlib.h>			//exit
#include <unistd.h>
#include "const.h"			//BUFFER_SIZE
#include "email.h"			//email struct

int main(int argc, char* argv[]) {
	email *p_new_email;

	p_new_email = readmail();
	printf("Toto je nacitany mail z email struktury:\n %s\n", p_new_email->body);


	//TODO uvolnit alokovanu pamat
	return 0;
}
