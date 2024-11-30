#include "header.h"
int compare_ints(const void *a, const void *b) {
    int *x = (int *)a;
    int *y = (int *)b;
    return *x - *y;
}
void list_shell_processes()
{
    struct dirent *entry;
    DIR *dir = opendir("/proc");
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the entry name is a number (process ID)
        if (isdigit(*entry->d_name))
        {
            char status_path[2560];
            snprintf(status_path, sizeof(status_path), "/proc/%s/status", entry->d_name);

            FILE *fp = fopen(status_path, "r");
            if (fp == NULL)
            {
                continue;
            }

            char line[256];
            char process_name[256] = {0};
            char state[256] = {0};
            pid_t ppid = -1;

            while (fgets(line, sizeof(line), fp))
            {
                if (strncmp(line, "Name:", 5) == 0)
                {
                    sscanf(line, "Name:\t%255[^\n]", process_name);
                }
                else if (strncmp(line, "State:", 6) == 0)
                {
                    sscanf(line, "State:\t%255[^\n]", state);
                }
                else if (strncmp(line, "PPid:", 5) == 0)
                {
                    sscanf(line, "PPid:\t%d", &ppid);
                }
            }

            fclose(fp);

            // Check if the process's parent PID matches the shell's PID
            int array[1024];
            int count = 0;
            if (ppid == sheel)
            {
                // print_process_info(entry->d_name, process_name, state);
                array[count++] = atoi(entry->d_name);
            }
            qsort(array,count,sizeof(int),compare_ints);
            for(int j=0;j<count;j++){
                print_process_info(array[j], process_name, state);
            }
        }
    }

    closedir(dir);
}

void print_process_info(const int pid, const char *process_name, const char *state)
{
    // if (strncmp(state, "R", 1) == 0)
    // {
    //     printf("%s : %s - Running\n", pid, process_name);
    // }
    // else
    // {
    //     printf("%s : %s - Stopped\n", pid, process_name);
    // }
    if (strcmp(state, "R"))
    {
        printf("%d : %s - Running\n", pid, process_name);
        
    }
    else if (strcmp(state, "Z"))
    {
        printf("%d : %s - Stopped\n", pid, process_name);
       
    }
    else if (strcmp(state, "S"))
    {
        printf("%d : %s - Running\n", pid, process_name);
        
    }
    else
    {
        printf("%d : %s - Stopped\n", pid, process_name);
        
    }
}