#include "header.h"

void hop(char *temp_arg) {
    char *args[1024];
    int argc = 0;
    char *token = strtok(temp_arg, " ");
    
    
    while (token != NULL) {
        args[argc] = token;
        argc++;
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;

    char cwd[1024];  
    uid_t uid;
    struct utsname uts;
    char *username;

    uid = getuid(); 
    username = getpwuid(uid)->pw_name;
    uname(&uts); 

    
    for (int i = 1; i < argc; i++) {
        if (strcmp(args[i], "~") == 0) {
            if (getcwd(prev_dir, sizeof(prev_dir)) == NULL) {  
                perror("getcwd");
                return;
            }
            if (chdir(home_global) == -1) {
                perror("chdir");
            } else {
                getcwd(cwd, sizeof(cwd));
                snprintf(prompt, sizeof(prompt), "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~$\x1B[0m> ", username, uts.nodename);
                printf("%s\n", home_global);
            }
        } 
        else if (strcmp(args[i], "-") == 0) {
            char temp_dir[1024];
            if (getcwd(temp_dir, sizeof(temp_dir)) == NULL) {
                perror("getcwd");
                return;
            }
            if (chdir(prev_dir) == -1) {
                perror("chdir");
            } else {
                printf("Changed to previous directory: %s\n", prev_dir);
                strcpy(prev_dir, temp_dir);
                getcwd(cwd, sizeof(cwd));
                snprintf(prompt, sizeof(prompt), "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename, &prev_dir[strlen(home_global)]);
            }
        } 
        else if (strcmp(args[i], "..") == 0) {
            if (getcwd(prev_dir, sizeof(prev_dir)) == NULL) {  
                perror("getcwd");
                return;
            }
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                char *last_dir = strrchr(cwd, '/');
                if (last_dir != NULL) {
                    *last_dir = '\0';  
                }
                if (chdir(cwd) == -1) {
                    perror("chdir");
                } else {
                    printf("Moved to parent directory: %s\n", cwd);
                    if (strcmp(home_global, cwd) == 0) {
                        snprintf(prompt, sizeof(prompt), "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~$\x1B[0m> ", username, uts.nodename);
                    } else {
                        snprintf(prompt, sizeof(prompt), "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename, &cwd[strlen(home_global)]);
                    }
                }
            } else {
                perror("getcwd");
            }
        } 
        else if (strncmp(args[i], "~/", 2) == 0) {
            char target_dir[2000];
            snprintf(target_dir, sizeof(target_dir), "%s/%s", home_global, args[i] + 2);
            if (getcwd(prev_dir, sizeof(prev_dir)) == NULL) {  
                perror("getcwd");
                return;
            }
            if (chdir(target_dir) == -1) {
                perror("chdir");
                printf("INVALID DIRECTORY NAME\n");
            } else {
                getcwd(cwd, sizeof(cwd));
                snprintf(prompt, sizeof(prompt), "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~/%s$\x1B[0m> ", username, uts.nodename, &cwd[strlen(home_global) + 1]);
                printf("%s\n", cwd);
            }
        } 
        else {
            if (getcwd(prev_dir, sizeof(prev_dir)) == NULL) {  
                perror("getcwd");
                return;
            }
            if (chdir(args[i]) == -1) {
                perror("chdir");
                printf("INVALID DIRECTORY NAME\n");
            } else {
                getcwd(cwd, sizeof(cwd));
                printf("Changed to directory: %s\n", cwd);
                if (strcmp(cwd, home_global) == 0) {
                    snprintf(prompt, sizeof(prompt), "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename, &cwd[strlen(home_global)]);
                    // printf("%s\n",prompt);
                } else {
                    snprintf(prompt, sizeof(prompt), "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~/%s$\x1B[0m> ", username, uts.nodename, &cwd[strlen(home_global)]);
                    // printf("%s\n",prompt);
                }
            }
        }
    }
}
