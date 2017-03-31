#include "sfish.h"

void builtin_help(){
	printf("help [-dms] [pattern ...]\n");
	printf("exit [n]\n");
	printf("cd [-L|[-P [-e]] [-@]] [dir]\n");
	printf("pwd [-LP]\n");
	printf("alarm [n]\n");

}


void builtin_exit(){
	exit(3);
}


void builtin_cd(char *argv[], char** oldpwd) {
	char newpwd[MAX_SIZE];
	if(argv[1]==NULL){

		if (*oldpwd != NULL)
			free(*oldpwd);
		getcwd(newpwd, sizeof(newpwd));
		*oldpwd = (char*)malloc(strlen(newpwd));
		strcpy(*oldpwd, newpwd);
		chdir(getenv("HOME"));
		//printf("%s\n", *oldpwd);
	}
	else if(strcmp(argv[1],"-")==0){
		if(*oldpwd==NULL){
			fprintf(stderr, "sfish: cd: OLDPWD not set\n");
		}
		else{
			char* origin = (char*)malloc(strlen(*oldpwd));
			strcpy(origin, *oldpwd);
			free(*oldpwd);
			getcwd(newpwd, sizeof(newpwd));
			*oldpwd = (char*)malloc(strlen(newpwd));
			strcpy(*oldpwd, newpwd);
			chdir(origin);
			free(origin);
		}

	}
	else {

		if (*oldpwd != NULL)
			free(*oldpwd);
		getcwd(newpwd, sizeof(newpwd));
		*oldpwd = (char*)malloc(strlen(newpwd));
		strcpy(*oldpwd, newpwd);
		if((chdir(argv[1])) < 0) {
			fprintf(stderr, "sfish: cd: %s: No such file or directory\n", argv[1]);
		}
	}
}

void builtin_pwd(){
	char curpwd[MAX_SIZE];
	getcwd(curpwd, sizeof(curpwd));
	printf("%s\n",curpwd);

}




