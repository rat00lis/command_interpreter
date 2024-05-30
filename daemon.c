// daemon.c

#include "headers.h"

void log_system_info(int argc, char *argv[])
{
    int t = 5;  // Valor predeterminado de t (segundos)
    int p = 60; // Valor predeterminado de p (segundos)


    // Verificar argumentos de línea de comando
    if (argc > 2)
    {
        t = atoi(argv[2]);
    }
    if (argc > 3)
    {
        p = atoi(argv[3]);
    }

    // Configurar el demonio
    pid_t pid, sid;
    pid = fork();

    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    sid = setsid();

    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    openlog("diablito_bakan", LOG_PID, LOG_DAEMON);

    // Ciclo principal del demonio
    int i;
    for (i = 0; i < p / t; i++)
    {
        FILE *stat = fopen("/proc/stat", "r");
        int processes = 0, procs_running = 0, procs_blocked = 0;
        char line[256];

        if (stat)
        {
            while (fgets(line, sizeof(line), stat))
            {
                if (sscanf(line, "processes %d", &processes) == 1)
                {
                    syslog(LOG_INFO, "Processes: %d", processes);
                }
                else if (sscanf(line, "procs_running %d", &procs_running) == 1)
                {
                    syslog(LOG_INFO, "Procs Running: %d", procs_running);
                }
                else if (sscanf(line, "procs_blocked %d", &procs_blocked) == 1)
                {
                    syslog(LOG_INFO, "Procs Blocked: %d", procs_blocked);
                }
            }
            fclose(stat);
        }
        else
        {
            syslog(LOG_ERR, "Failed to open /proc/stat");
        }

        sleep(t);
    }
    syslog(LOG_INFO, "Daemon finalizado");
    closelog();
    exit(EXIT_SUCCESS);
}
