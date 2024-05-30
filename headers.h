// headers.h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <sys/stat.h>
#include <fcntl.h>

#define READ_END    0    
#define WRITE_END   1 

void execute_command(const char* cmd);
void execute_pipeline(char* commands[], int n);
char **tokenize_input(char *line, int *num_tokens);
void execmd(char **argv);
void log_system_info(int argc, char *argv[]);
