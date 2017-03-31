#ifndef SFISH_H
#define SFISH_H
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_SIZE 4096
void builtin_help();
void builtin_exit();
void builtin_cd(char *argv[], char** oldpwd);
void builtin_pwd();
int get_cmds(char* cmd, char** cmdlist);
char* get_prompt();
int get_redirect_out_index(char** cmds, int numOfcmds);
int get_redirect_in_index(char** cmds, int numOfcmds);
#endif
