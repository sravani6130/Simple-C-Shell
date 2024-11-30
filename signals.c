#include "header.h"
void handle_sigchld(int sig)
{
    int saved_errno = errno;
    int status;
    pid_t pid;

    char log_path[4096];
    snprintf(log_path, sizeof(log_path), "%s/%s", home_global, "pid_store.txt");
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {

        if (WIFEXITED(status) || WIFSIGNALED(status))
        {
            FILE *fp = fopen(log_path, "r");
            if (fp == NULL)
            {
                perror("Error opening file");
            }
            else
            {
                char line[1024];
                char command[1024];
                int found = 0;

                while (fgets(line, sizeof(line), fp))
                {
                    int stored_pid;
                    char *last_space = strrchr(line, ' ');
                    if (last_space != NULL)
                    {
                        *last_space = '\0';                          
                        stored_pid = atoi(last_space + 1);           
                        strncpy(command, line, sizeof(command) - 1); 
                        command[sizeof(command) - 1] = '\0';         
                    
                        if (stored_pid == pid)
                        {
                            found = 1;
                            break;
                        }
                    }
                }
                fclose(fp);

                
                if (found)
                {
                    if (WIFEXITED(status))
                    {
                        printf("%s (%d) completed normally with exit status %d\n", command, pid, WEXITSTATUS(status));
                    }
                    else if (WIFSIGNALED(status))
                    {
                        printf("%s (%d) terminated by signal %d\n", command, pid, WTERMSIG(status));
                    }

                  
                    FILE *temp_fp = fopen("temp_pid_store.txt", "w");
                    fp = fopen(log_path, "r");
                    if (temp_fp == NULL || fp == NULL)
                    {
                        perror("Error processing file");
                        continue;
                    }

                    while (fgets(line, sizeof(line), fp))
                    {
                        int stored_pid;
                        sscanf(line, "%s %d", command, &stored_pid);
                        if (stored_pid != pid)
                        {
                            fprintf(temp_fp, "%s", line); 
                        }
                    }
                    fclose(fp);
                    fclose(temp_fp);

                    remove(log_path);
                    rename("temp_pid_store.txt", log_path);
                }
            }
        }

        errno = saved_errno;
    }
}