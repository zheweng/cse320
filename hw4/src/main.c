#include "sfish.h"
#include "debug.h"
static char* lastdir = NULL;

/*
 * As in previous hws the main function must be in its own file!
 */

// int main(int argc, char const *argv[], char* envp[]){
    /* DO NOT MODIFY THIS. If you do you will get a ZERO. */
 //   rl_catch_signals = 0;
    /* This is disable readline's default signal handlers, since you are going to install your own.*/
//    char *cmd;
//        if (strcmp(cmd, "exit")==0)
//           break;
//        printf("%s\n",cmd);
        /* All your debug print statements should use the macros found in debu.h */
        /* Use the `make debug` target in the makefile to run with these enabled. */
//        info("Length of command entered: %ld\n", strlen(cmd));
        /* You WILL lose points if your shell prints out garbage values. */
 //   }

    /* Don't forget to free allocated memory, and close file descriptors. */
//    free(cmd);

//    return EXIT_SUCCESS;
//}
int main(int argc, char const *argv[], char* envp[]){

    /* DO NOT MODIFY THIS. If you do you will get a ZERO. */
    rl_catch_signals = 0;
    /* This is disable readline's default signal handlers, since you are going to install your own.*/
    char *cmd;

    char* prompt = get_prompt();

    char* cmds[10];

    while((cmd = readline(prompt)) != NULL) {
    	get_cmds(cmd,cmds);


        if (strcmp(cmds[0], "exit")==0) {
        	builtin_exit();
        }

        else if (strcmp(cmds[0],"help")==0){
        	if(cmds[1]==NULL){
        		builtin_help();
        	}
        	else{
        		fprintf(stderr, "sfish: help: no help topics match ` %s:\n", cmds[1]);
        	}
        }
        else if (strcmp(cmds[0],"cd")==0){
        	//printf("%s",lastdir);
        	builtin_cd(cmds, &lastdir);
        }
        else if(strcmp(cmds[0],"pwd")==0){
        	pid_t cpid;
        	if((cpid=fork())==0){
        		builtin_pwd();
        	}
            int status;
            pid_t ppid;
            ppid=wait(&status);
            if(ppid>0){
                if (!WIFEXITED(status))
                    fprintf(stderr, "Child Error!\n");
            }
       	}
        else{

            char* ret;
            ret= strchr(cmds[0],'/');
            if(ret!=NULL){
                pid_t cpid;
                if((cpid=fork())==0){
                    struct stat cbuf;
                    if(stat(cmds[0],&cbuf)==0){
                        execv(cmds[0],cmds);
                    }
                    else{
                        fprintf(stderr, "sfish: Unknown command %s\n", cmds[0]);
                    }
                }
                int status;
                pid_t ppid;
                ppid=wait(&status);
                if(ppid>0){
                    if (!WIFEXITED(status))
                        fprintf(stderr, "Child Error!\n");
                }

            }
            struct stat buf;
            char* dup= strdup(getenv("PATH"));
            char* plist=dup;
            char* p=NULL;
            int found = 0;
            char* exe_path = NULL;
            p=strtok(plist,":");
            while(p!=NULL && !found){
                char* currPath = (char*)malloc(strlen(p) + strlen(cmds[0]) + 2);
                memcpy(currPath, p, strlen(p));
                currPath[strlen(p)] = '/';
                char* cmd_start = currPath + strlen(p) + 1;
                memcpy(cmd_start, cmds[0], strlen(cmds[0]));
                currPath[strlen(p) + strlen(cmds[0]) + 1] = 0;
                if(stat(currPath ,&buf) == 0){
                    found = 1;
                    exe_path = currPath;
                }
                p=strtok(NULL,":");
            }

            if (found) {
                pid_t cpid;
                if((cpid=fork())==0){
                    execv(exe_path,cmds);
                }
                int status;
                pid_t ppid;
                ppid=wait(&status);
                if(ppid>0){
                    if (!WIFEXITED(status))
                        fprintf(stderr, "Child Error!\n");
                }
            }
        }

        free(prompt);
        prompt = get_prompt();
    }

    /* Don't forget to free allocated memory, and close file descriptors. */
    free(cmd);

    return EXIT_SUCCESS;
}

void get_cmds(char* cmd, char** cmdlist) {
	char* line = strdup(cmd);

	line[strlen(line)] = ' ';

	while(*line && *line == ' ')
		line++;

	int num = 0;
	char* cursor;
	while ((cursor = strchr(line, ' ')) != NULL) {
		cmdlist[num] = line;
		*cursor = 0;
		line = cursor + 1;
		num++;
		while (*line && *line == ' ')
			line++;
	}
	cmdlist[num] = 0;

}


char* get_prompt() {
	char buf[MAX_SIZE];
    char curdir[MAX_SIZE];
    getcwd(curdir,sizeof(curdir));

    strcpy(buf, "<zheweng> : ");
    strcat(buf,"<");
    strcat(buf,curdir);

    strcat(buf,"> ");
    strcat(buf,"$ ");

    char* prompt = (char*)malloc(strlen(buf) + 1);
    strcpy(prompt, buf);
    prompt[strlen(buf)] = 0;
    return prompt;
}
