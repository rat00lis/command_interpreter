// input_processing.c

#include "headers.h"

char **tokenize_input(char *line, int *num_tokens)
{
    const char *separadores = " \t\n";
    char *token;
    char *line_copy;
    char **argv;
    int i;

    // Alocar memoria para line_copy
    int size_read = sizeof(char) * strlen(line);
    line_copy = malloc(size_read);
    if (line_copy == NULL)
    {
        perror("malloc: error al reservar memoria");
        return NULL;
    }
    strcpy(line_copy, line); // Copiar la string

    // Contar tokens
    *num_tokens = 0;
    token = strtok(line, separadores);
    while (token != NULL)
    {
        (*num_tokens)++;
        token = strtok(NULL, separadores);
    }
    (*num_tokens)++;

    // Alocar memoria para argv
    argv = malloc(sizeof(char *) * (*num_tokens));
    if (!argv)
    {
        perror("malloc: error al reservar memoria para argv");
        free(line_copy);
        return NULL;
    }

    // Guardar los tokens en argv
    token = strtok(line_copy, separadores);
    for (i = 0; token != NULL; i++)
    {
        argv[i] = malloc(sizeof(char) * (strlen(token) + 1)); // +1 para el '\0'
        if (!argv[i])
        {
            perror("malloc: error al reservar memoria para argv[i]");
            for (int j = 0; j < i; j++)
            { // Liberar memoria previamente asignada
                free(argv[j]);
            }
            free(argv);
            free(line_copy);
            return NULL;
        }
        strcpy(argv[i], token);
        token = strtok(NULL, separadores);
    }
    argv[i] = NULL;
    free(line_copy); // Liberamos line_copy aquí
    return argv;
}

void execmd(char **argv)
{
    pid_t pid, wpid;
    int status;

    if (!argv[0])
    {
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
        {
            perror("Error:");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("Error al hacer fork");
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}
