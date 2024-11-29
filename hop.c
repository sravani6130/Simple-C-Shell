#include "header.h"
void hop(int argc_count,char **args,char *prompt,char *home_dir_store,char*prev_dir)
{
    uid_t uid;
    struct utsname uts;
    char *username;
    char cwd[1024];
    
    uid = getuid(); // Get username
    username = getpwuid(uid)->pw_name;
    uname(&uts); // Get system name
    getcwd(cwd, sizeof(cwd)); // Get current working directory
    
    int len = strlen(home_dir_store);
    
    int i=1;
    while(i<argc_count){
        
        if(strcmp(args[i],"~")==0){
            //change to home and print home
            
            // printf("homee:%s\n",prev_dir);
            if (strcmp(home_dir_store, cwd) == 0){
                // sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename,home_dir_store); 
                printf("%s\n",home_dir_store);
            }else{
                strcpy(prev_dir, cwd);
            chdir(home_dir_store);
            getcwd(cwd,1024);
            sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~$\x1B[0m> ", username, uts.nodename);
            // printf("/home/%s\n",username);
            printf("%s\n",home_dir_store);
            }
        }
        else if(strcmp(args[i],".")==0){
            getcwd(cwd,1024);
            printf("%s\n",cwd);
        }
        //change one dir down to it
        else if(strcmp(args[i],"..")==0){
            if (strcmp(home_dir_store, cwd) == 0) {
                 // Already in the home directory
                chdir(cwd);
                getcwd(cwd, sizeof(cwd));
                // sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m%s$\x1B[0m> ", username, uts.nodename,cwd); 
                printf("%s\n", cwd); 
            } else {
                // Save current directory before changing
                strcpy(prev_dir, cwd);
                char *parent = strrchr(cwd, '/');
                if (parent != NULL) {
                    *parent = '\0'; // Move up one directory
                    chdir(cwd);
                    getcwd(cwd, sizeof(cwd)); // Update cwd after changing directory
                    if (strcmp(home_dir_store, cwd) == 0) {
                        sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~$\x1B[0m> ", username, uts.nodename); 
                        // printf("/home/%s%s\n", username, &cwd[len]);
                        printf("%s\n", cwd); 
                    } else {
                        // printf("/home/%s%s\n", username, &cwd[len]);
                        printf("%s\n", cwd); 
                        sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename, &cwd[len]);
                    }            
                }
            }
        }
        
        // "-"to previous directory
        else if(strcmp(args[i],"-")==0){
            // printf("%s\n",prev_dir);
            if(strcmp(prev_dir, cwd) == 0){
                 printf("No change in directory has taken place.\n");
            } else {
                    // Proceed with changing to the previous directory
                    chdir(prev_dir);
                    getcwd(cwd, 1024);
                    // printf("/home/%s%s\n", username, &prev_dir[len]);
                    printf("%s\n",prev_dir); 
                    sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename, &prev_dir[len]);
                }

            // chdir(prev_dir);
            // // printf("%s\n",prev_dir);
            // getcwd(cwd,1024);
            // printf("/home/%s%s\n",username,&prev_dir[len]);
            // sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename, &prev_dir[len]);

        }

        // if path is given
        else if (strncmp(args[i], "~/", 2) == 0){
            char target_dir[1024];
            snprintf(target_dir, sizeof(target_dir), "%s/%s", home_dir_store, args[i] + 2);
            strcpy(prev_dir, cwd); // Save current directory before changing
           
            if (chdir(target_dir) == 0) {
                getcwd(cwd, sizeof(cwd)); // Update cwd after changing directory
                printf("%s\n", cwd);
                sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~/%s$\x1B[0m> ", username, uts.nodename, &cwd[len + 1]);
            } else {
                perror("chdir");
                printf("INVALID DIRECTORY NAME\n");
            }
        }
        //to change directory to specified
        else{
            char newcwd[444444];
            //copying ced to prevcwd
            snprintf(newcwd, sizeof(newcwd), "%s/%s", cwd, args[i]);
            strcpy(prev_dir,cwd);
            if(chdir(newcwd)==0){
                getcwd(cwd,1024);
                // printf("/home/%s%s\n",username,&newcwd[len]);
                printf("%s\n", newcwd); 
                
                sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename, &newcwd[len]);
            }else{
                perror("chdir");
                printf("INVALID DIRECTORY NAME\n");
            }
        }
        i++;
    }
}