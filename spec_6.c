#include "header.h"
void handle_sigchld(int sig) {
    int saved_errno = errno;
    int status;
    pid_t pid;
    
    char log_path[4096];
    snprintf(log_path, sizeof(log_path), "%s/%s",home_global,"pid_store.txt");
    // Wait for any child process (non-blocking)
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            // Open the pid_store.txt file
            FILE *fp = fopen(log_path, "r");
            if (fp == NULL) {
                perror("Error opening file");
                continue;
            }
            
            // Search for the corresponding command in the file
            char line[1024];
            char command[1024];
            int found = 0;
            while (fgets(line, sizeof(line), fp)) {
                int stored_pid;
                sscanf(line, "%s %d", command, &stored_pid);
                if (stored_pid == pid) {
                    found = 1;
                    break;
                }
            }
            fclose(fp);
            
            // Print the command associated with the PID
            if (found) {
                if (WIFEXITED(status)) {
                    printf("%s (%d) completed normally with exit status %d\n",  command,pid, WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("%s (%d) terminated by signal %d\n",  command,pid, WTERMSIG(status));
                }
                
                // Remove the processed entry from the file
                // Create a temporary file
                FILE *temp_fp = fopen("temp_pid_store.txt", "w");
                fp = fopen(log_path, "r");
                if (temp_fp == NULL || fp == NULL) {
                    perror("Error processing file");
                    continue;
                }
                
                while (fgets(line, sizeof(line), fp)) {
                    int stored_pid;
                    sscanf(line, "%s %d", command, &stored_pid);
                    if (stored_pid != pid) {
                        fprintf(temp_fp, "%s", line);  // Write back all other entries
                    }
                }
                fclose(fp);
                fclose(temp_fp);
                
                
                remove(log_path);
                rename("temp_pid_store.txt",log_path);
            }
        }
    }

    errno = saved_errno;  
}