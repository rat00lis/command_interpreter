// main.c

#include "headers.h"

int main(int argc, char *argv[])
{
    // Verificar si se debe ejecutar el demonio o el shell
    if (argc > 1 && strcmp(argv[1], "daemon") == 0)
    {
        log_system_info(argc, argv);
    }
    else
    {
        char *prompt = "(shell_bakan) $ ";
        char *line = NULL;
        size_t len = 0;
        ssize_t nchars_read;
        int num_tokens;
        (void)argc;

        while (1)
        {
            printf("%s", prompt);
            nchars_read = getline(&line, &len, stdin);

            if (nchars_read == -1)
            {
                printf("Saliendo de la shell...\n");
                free(line);
                return (-1);
            }

            // Remove trailing newline character from line
            if (line[nchars_read - 1] == '\n')
            {
                line[nchars_read - 1] = '\0';
            }

            if (strstr(line, "|"))
            {
                // Pipes
                char *commands[20]; // Asumimos un máximo de 20 comandos en la cadena
                char *token = strtok(line, "|");
                int i = 0;

                while (token)
                {
                    commands[i] = token;
                    token = strtok(NULL, "|");
                    i++;
                }

                execute_pipeline(commands, i);
            }
            else
            {
                char **cmd_argv = tokenize_input(line, &num_tokens);
                if (cmd_argv)
                {
                    execmd(cmd_argv);
                    for (int i = 0; cmd_argv[i] != NULL; i++)
                    {
                        free(cmd_argv[i]);
                    }
                    free(cmd_argv);
                }
            }
        }
        free(line); // Clean up 'line' memory when exiting loop
        return 0;
    }
}