#include "header.h"

void trim_spaces(char *str)
{
    char *start = str;
    char *end;

    while (isspace((unsigned char)*start))
    {
        start++;
    }

    if (*start == '\0')
    {
        *str = '\0';
        return;
    }

    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end))
    {
        end--;
    }

    *(end + 1) = '\0';
    memmove(str, start, strlen(start) + 1);
}

char *extract_filename(const char *ptr)
{
    while (*ptr == ' ' || *ptr == '<' || *ptr == '>')
        ptr++;

    const char *end = ptr;
    while (*end && *end != ' ' && *end != '\n')
        end++;

    size_t length = end - ptr;
    char *filename = (char *)malloc(length + 1);
    if (filename)
    {
        strncpy(filename, ptr, length);
        filename[length] = '\0';
    }

    return filename;
}

int handle_input_redirection(char *cmd)
{
    char *ptr = strchr(cmd, '<');
    if (ptr)
    {

        char *filename = extract_filename(ptr);
        // printf("%s\n",filename);
        int fd = open(filename, O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDIN_FILENO) < 0)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd);
        free(filename);
        // char *args[1024];
        // int argc = 0;
        // char *token = strtok(cmd, " ");
        // while (token != NULL)
        // {
        //     if (strncmp(token, "<", 1) != 0)
        //     {
        //         args[argc] = malloc(strlen(token) + 1); // allocate memory for the string
        //         strcpy(args[argc], token);              // copy the string
        //         argc++;
        //     }
        //     else
        //     {
        //         break;
        //     }
        //     token = strtok(NULL, " ");
        // }

        // if (execvp(args[0], args) == -1)
        // {
        //     perror("execvp");
        //     exit(EXIT_FAILURE);
        // }
        return 1;
    }
    return 0;
}

int handle_output_redirection(char *cmd)
{
    char *ptr = strchr(cmd, '>');
    if (ptr)
    {

        int flags = O_WRONLY | O_CREAT;
        if (*(ptr + 1) == '>')
        {
            flags |= O_APPEND;
            ptr++;
        }
        else
        {
            flags |= O_TRUNC;
        }
        char *filename = extract_filename(ptr);
        // printf("%s\n",filename);
        int fd = open(filename, flags, 0644);
        if (fd < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd);
        free(filename);
        // char *args[1024];
        // int argc = 0;
        // char *token = strtok(cmd, " ");
        // while (token != NULL)
        // {
        //     if (strncmp(token, ">", 1) != 0)
        //     {
        //         args[argc] = malloc(strlen(token) + 1); // allocate memory for the string
        //         strcpy(args[argc], token);              // copy the string
        //         argc++;
        //     }
        //     else
        //     {
        //         break;
        //     }
        //     token = strtok(NULL, " ");
        // }

        // args[argc] = NULL;
        // if (execvp(args[0], args) == -1)
        // {
        //     perror("execvp");
        //     exit(EXIT_FAILURE);
        // }
        return 1;
    }
    return 0;
}

int check_for_manual_commands(char *arg)
{
    if (strncmp(arg, "hop", 3) == 0)
    {
        hop(arg);
        return 0; // Return 0 to indicate a manual command was handled
    }
    else if (strncmp(arg, "reveal", 6) == 0)
    {
        reveal(arg);
        return 0;
    }
    else if (strncmp(arg, "log", 3) == 0)
    {

        log_fun(arg);

        return 0;
    }
    else if (strncmp(arg, "proclore", 8) == 0)
    {
        pid_t process_id;
        char *token = strtok(arg, " ");
        if (token != NULL && strcmp(token, "proclore") == 0)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                process_id = atoi(token);
            }
            else
            {
                process_id = getpid();
            }
        }
        else
        {
            process_id = getpid();
        }
        proclore_info(process_id);
        return 0;
    }
    else if (strncmp(arg, "seek", 4) == 0)
    {
        new_seek(arg);
        return 0;
    }
    else if (strncmp(arg, "activities", 10) == 0)
    {

        list_shell_processes();
        return 0;
    }
    else if (strncmp(arg, "ping", 4) == 0)
    {
        struct sigaction sa;
        sa.sa_handler = signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        if (sigaction(SIGINT, &sa, NULL) == -1)
        {
            perror("Error setting up SIGINT handler");
            exit(EXIT_FAILURE);
        }

        if (sigaction(SIGTERM, &sa, NULL) == -1)
        {
            perror("Error setting up SIGTERM handler");
            exit(EXIT_FAILURE);
        }
        ping_command(arg);
        return 0;
    }
    else if (strncmp(arg, "fg", 2) == 0)
    {
        fg_func(arg);
        return 0;
    }
    else if (strncmp(arg, "bg", 2) == 0)
    {
        bg_func(arg);
        return 0;
    }
    else if (strncmp(arg, "iMan", 4) == 0)
    {
        char *tokens[10];
        int i = 0;
        char *token = strtok(arg, " ");
        while (token != NULL)
        {
            tokens[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        tokens[i] = NULL;
        // fetch_man_page(tokens[1]);
        iMan(tokens[1]);
        return 0;
    }
    else if (strncmp(arg, "neonate", 7) == 0)
    {
        run_neonate(arg);
        fflush(STDIN_FILENO);
        return 0;
    }
    return 1;
}

void execute_pipe_redirect(int and_count, char **and_args, int *flag_bg)
{
    int i = 0;
    while (i < and_count)
    {
        int bg = flag_bg[i];
        char *args[1024];
        int argc = 0;
        char *token = strtok(and_args[i], "|");

        while (token != NULL)
        {
            trim_spaces(token);
            args[argc] = token;
            argc++;
            token = strtok(NULL, "|");
        }
        args[argc] = NULL;

        int num_pipes = argc - 1;
        int pipefd[2 * num_pipes];
        pid_t pid;
        int status;

        for (int k = 0; k < num_pipes; k++)
        {
            if (pipe(pipefd + k * 2) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        for (int cmd_index = 0; cmd_index < argc; cmd_index++)
        {
            prctl(PR_SET_PDEATHSIG, SIGHUP);
            pid = fork();
            if (pid == 0)
            { 
                if (cmd_index > 0)
                { 
                    if (dup2(pipefd[(cmd_index - 1) * 2], STDIN_FILENO) < 0)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                }

                if (cmd_index < num_pipes)
                { 
                    if (dup2(pipefd[cmd_index * 2 + 1], STDOUT_FILENO) < 0)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                }

                for (int k = 0; k < 2 * num_pipes; k++)
                {
                    close(pipefd[k]);
                }

                int input = handle_input_redirection(args[cmd_index]);
                int output = handle_output_redirection(args[cmd_index]);

                if (!check_for_manual_commands(args[cmd_index]))
                {
                    // printf("hi:%d\n",*revisiting);
                    // exit(0); // Exit after handling the manual command
                    if(strncmp(args[cmd_index],"hop",3)!=0)
                    {
                        exit(0);
                    }
                }
                else
                {
                    bool in_quotes = false;
                    char *cmd_args[1024];
                    int cmd_argc = 0;
                    char *p = args[cmd_index];
                    for (; *p; ++p)
                    {
                        if (*p == '"' || *p == '\'')
                        {
                            if (in_quotes)
                            {
                                *p = '\0';
                            }
                            in_quotes = !in_quotes;
                        }
                        else if (!in_quotes && *p == ' ')
                        {
                            *p = '\0';
                            cmd_args[cmd_argc++] = args[cmd_index];
                            if (*(p + 1) == '"' || *(p + 1) == '\'' || *(p + 1) == '<' || *(p + 1) == '>')
                            {
                                if (*(p + 2) == '>')
                                {
                                    args[cmd_index] = p + 3;
                                }
                                else
                                {
                                    args[cmd_index] = p + 2;
                                }
                            }
                            else
                            {
                                args[cmd_index] = p + 1;
                            }
                        }
                    }
                    cmd_args[cmd_argc++] = args[cmd_index];
                    cmd_args[cmd_argc] = NULL;

                    char *cmd_args_new[1024];
                    int count = 0;
                    for (int z = 0; z < cmd_argc; z++)
                    {
                        // printf("%s\n",cmd_args[z]);
                        trim_spaces(cmd_args[z]);
                        if (cmd_args[z][0] == ' ' || cmd_args[z][0] == '\0' || cmd_args[z][0] == '\n')
                        {
                            continue;
                        }
                        cmd_args_new[count++] = cmd_args[z];
                    }
                    if (input || output)
                    {
                        for (int i = 1; i < count; i++)
                        {
                            cmd_args_new[i] = NULL;
                        }
                    }

                    if (execvp(cmd_args_new[0], cmd_args_new) < 0)
                    {
                        // perror("execvp");
                        printf("INVALID ARGUMENT\n");
                        exit(EXIT_FAILURE);
                    }
                    // exit(0);
                }
            }
            else if (pid < 0)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (cmd_index > 0)
            {
                close(pipefd[(cmd_index - 1) * 2]); 
            }
            if (cmd_index < num_pipes)
            {
                close(pipefd[cmd_index * 2 + 1]); 
            }
            if (bg)
            {
                printf("%d\n", pid);
                char log_path[4096];
                snprintf(log_path, sizeof(log_path), "%s/%s", home_global, "pid_store.txt");

                FILE *fp = fopen(log_path, "a");
                if (fp == NULL)
                {
                    perror("Error opening file");
                }

                fprintf(fp, "%s %d\n", args[cmd_index], pid);

                fclose(fp);
            }
        }

        
        for (int k = 0; k < 2 * num_pipes; k++)
        {
            close(pipefd[k]);
        }

        if (!bg)
        {
            for (int k = 0; k < argc; k++)
            {
                // waitpid(-1, &status, 0);
                waitpid(pid, NULL, 0);
            }
        }

        i++;
    }
}
