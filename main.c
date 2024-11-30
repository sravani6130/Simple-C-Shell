#include "header.h"

char home_global[1024];
char prev_dir[1024];
char prompt[4096];
char user_input[1024];
pid_t sheel;
void tokenize_commands(char *user_input, char *and_args[1024], int *flag_bg, int *and_count)
{
    char *args[1024];
    int argc = 0;
    char *token = strtok(user_input, ";");
    while (token != NULL)
    {
        args[argc] = token;
        argc++;
        token = strtok(NULL, ";");
    }
    args[argc] = NULL;

    for (int x = 0; x < argc; x++)
    {
        if (strncmp(args[x], "log", 3) != 0)
        {
            log_append(args[x], home_global);
        }
    }
    for(int i = 0; i < argc; i++){
        int last_bg = 0;
        if (args[i][strlen(args[i]) - 1] == '&')
        {
            last_bg = 1;
        }
        char *token = strtok(args[i], "&");
        while(token != NULL){
            and_args[*and_count] = token;
            flag_bg[*and_count] = 1;
            (*and_count)++;
            token = strtok(NULL, "&");
        }
        and_args[*and_count] = NULL;
        if (!last_bg)
        {
            flag_bg[*and_count - 1] = 0;
        }
    }
}

int main()
{
    signal(SIGINT, signal_handler);  // Ctrl-C
    signal(SIGTSTP, signal_handler);  // Ctrl-Z
    signal(SIGQUIT, signal_handler);  // Ctrl-D

    FILE *fp = fopen("log.txt", "w"); // for log to append commands
    fclose(fp);
    FILE *fpp = fopen("pid_store.txt", "w"); // for log to append commands
    fclose(fpp);
    sheel=getpid();
    char *and_args[1024];
    int flag_bg[1024] = {0};
    get_home(home_global);
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    get_shell_prompt(prompt);
    while (1)
    {
        // printf("rev:%d\n",revisiting);
        int and_count = 0;
        printf("%s", prompt);

        if (fgets(user_input, sizeof(user_input), stdin) == NULL)
        {
            perror("fgets");
            exit(1);
        }
        
        if(strncmp(user_input,"exit",4)==0){
        
            exit(0);
        }
        if (user_input[0] == '\n')
        {
            continue;
        }

        user_input[strcspn(user_input, "\n")] = '\0';
        char user_input_clone[1024];
        strcpy(user_input_clone, user_input);
        tokenize_commands(user_input_clone, and_args, flag_bg, &and_count);
        execute_pipe_redirect(and_count, and_args, flag_bg);
    }
    return 0;
}