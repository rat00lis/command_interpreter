// command_execution.c

#include "headers.h"

void execute_command(const char *cmd)
{
    char *const delim = " ";
    char *token = strtok(strdup(cmd), delim);
    char *args[20]; // Asumimos un máximo de 20 argumentos por comando
    int i = 0;

    while (token)
    {
        args[i] = token;
        token = strtok(NULL, delim);
        i++;
    }
    args[i] = NULL;

    execvp(args[0], args);
    perror("execvp");
    exit(1);
}

void execute_pipeline(char *commands[], int n)
{
    int fd[n - 1][2];
    int status, pid;

    for (int i = 0; i < n - 1; i++)
        pipe(fd[i]);

    for (int i = 0; i < n; i++)
    {
        pid = fork();

        if (pid == 0)
        {
            if (i > 0)
            {
                dup2(fd[i - 1][READ_END], STDIN_FILENO);
                close(fd[i - 1][READ_END]);
                close(fd[i - 1][WRITE_END]);
            }

            if (i < n - 1)
            {
                dup2(fd[i][WRITE_END], STDOUT_FILENO);
                close(fd[i][WRITE_END]);
                close(fd[i][READ_END]);
            }

            execute_command(commands[i]);
        }
        else
        {
            if (i > 0)
            {
                close(fd[i - 1][READ_END]);
                close(fd[i - 1][WRITE_END]);
            }
        }
    }

    for (int i = 0; i < n; i++)
        wait(&status);
}
