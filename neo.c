#include"header.h"
volatile sig_atomic_t running = 1;

void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        running = 0;
        printf("\n");
    }
}

int get_most_recent_pid()
{
    DIR *dir;
    struct dirent *entry;
    int max_pid = -1;
    int pid;
    char *endptr;

    dir = opendir("/proc");
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        pid = strtol(entry->d_name, &endptr, 10);
        if (*endptr == '\0' && pid > max_pid)
        {
            max_pid = pid;
        }
    }

    closedir(dir);
    return max_pid;
}

void set_raw_mode(struct termios *orig_term_attr) {
    struct termios new_term_attr;

    tcgetattr(STDIN_FILENO, orig_term_attr);

    new_term_attr = *orig_term_attr;
    cfmakeraw(&new_term_attr);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_term_attr);
}

void reset_terminal_mode(struct termios *orig_term_attr) {
    tcsetattr(STDIN_FILENO, TCSANOW, orig_term_attr);
}

void run_neonate(char *args)
{
    char *tokens[3];
    char *token = strtok(args, " ");
    int count = 0;

    while (token != NULL && count < 3)
    {
        tokens[count++] = token;
        token = strtok(NULL, " ");
    }

    if (count != 3 || strcmp(tokens[1], "-n") != 0)
    {
        fprintf(stderr, "Usage: neonate -n [time_arg]\n");
        exit(EXIT_FAILURE);
    }

    int time_arg = atoi(tokens[2]);
    if (time_arg <= 0)
    {
        fprintf(stderr, "Invalid time argument. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    struct termios orig_term_attr;
    set_raw_mode(&orig_term_attr);

    signal(SIGINT, handle_signal);

    while (running)
    {
        int pid = get_most_recent_pid();
        printf("%d\n", pid);
        fflush(stdout);

        for (int i = 0; i < time_arg * 10; i++)
        {
            usleep(100000); 

            
            if (running) {
                char ch;
                int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
                fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

                if (read(STDIN_FILENO, &ch, 1) > 0 && ch == 'x') {
                    running = 0;
                    // break;
                    exit(0);
                }

                fcntl(STDIN_FILENO, F_SETFL, flags);
            }
        }
    }

    reset_terminal_mode(&orig_term_attr);
}