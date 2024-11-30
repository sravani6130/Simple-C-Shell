#include "header.h"
pid_t foreground_pid = -1;

void signal_handler(int signum) {
    switch (signum) {
        case SIGINT:  // Ctrl-C
            if (foreground_pid > 0) {
                kill(foreground_pid, SIGINT);
                printf("\nSent SIGINT to process %d\n", foreground_pid);
            } else {
                printf("\nNo foreground process to interrupt\n");
            }
            break;

        case SIGTSTP:  // Ctrl-Z
            if (foreground_pid > 0) {
                kill(foreground_pid, SIGTSTP);
                printf("\nSent SIGTSTP to process %d\n", foreground_pid);
            } else {
                printf("\nNo foreground process to stop\n");
            }
            break;

        case SIGQUIT:  // Ctrl-D
            printf("\nExiting shell...\n");
            exit(0);
            break;

        default:
            printf("\nReceived signal %d\n", signum);
            break;
    }
}
void ping_command(char *input)
{
    char *token;
    char *args[3];
    int i = 0;

    token = strtok(input, " ");
    while (token != NULL && i < 3)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    if (i != 3)
    {
        printf("Invalid input format. Usage: ping <pid> <signal_number>\n");
        return;
    }

    int pid = atoi(args[1]);
    int signal_number = atoi(args[2]);
    int signal_to_send = signal_number % 32;

    if (kill(pid, signal_to_send) == -1)
    {
        if (errno == ESRCH)
        {
            printf("No such process found\n");
        }
        else
        {
            perror("Error sending signal");
        }
    }
    else
    {
        printf("Sent signal %d to process with pid %d\n", signal_to_send, pid);
    }
}