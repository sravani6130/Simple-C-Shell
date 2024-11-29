#include "header.h"

char home_global[1024];
int main() {
get_home(home_global);
    char prompt[1024];  // Changed to a single character array
    char *args[100];    // Array to hold pointers to each argument
    int arg_count;
    char user_input[1024];
    char prev_dir[44444];
    static char home_dir_store[1024];
    get_home(home_dir_store);
    int flag_in=0;
    strcpy(prev_dir,home_dir_store);
    // printf("%s\n",home_dir_store);
    get_shell_prompt(prompt);  // Pass a single char array
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    while (1) {
        if(!flag_in){
        printf("%s", prompt);

        }
        // Get user input
        if(flag_in){
        flag_in=0;

        }
        else if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            perror("fgets");
    
            continue;
        }
        
        user_input[strcspn(user_input, "\n")] = '\0'; 
        char command_given[1024];
        strcpy(command_given,user_input);
        char*token_colon=strtok(user_input,";");
        char*args_colon[4096];
        int colon_count=0;
        while (token_colon != NULL) {
            
        
            args_colon[colon_count++] = token_colon;
              // Store each token in args array
            token_colon = strtok(NULL, ";");
        }
        char*args_and[4096];
        int and_count=0;
        int args_bg[4096]={0};
        for(int i=0;i<colon_count;i++){
            int last_bg=0;
            if(args_colon[i][strlen(args_colon[i])-1]=='&'){
                last_bg=1;
            }
                char*token_and=strtok(args_colon[i],"&");
                while (token_and != NULL) {
                    
                
                    args_and[and_count++] = token_and;
                    // Store each token in args array
                    args_bg[and_count-1]=1;
                    token_and = strtok(NULL, "&");
                }
            if(!last_bg){
                args_bg[and_count-1]=0;
            }
        }
        
        for(int i=0;i<and_count;i++){

        char *token = strtok(args_and[i], " ");  // Pass a single char array
        arg_count = 0;

        
        while (token != NULL) {
            
            args[arg_count++] = token;
              // Store each token in args array
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;  // Null-terminate the array of arguments

        // log_append(args_and,home_dir_store);
        // for(int i=0;i<arg_count;i++){

        //     if(i<(arg_count-1) && strncmp(args[i],"log",3)==0){
                
        //         if(i<(arg_count-2) && strncmp(args[i+1],"execute",7)==0){
                    
        //         }
        //     }
        // }
        
        if(strncmp(args[0],"log",3)!=0){
            
            log_append(command_given,home_dir_store);
        }

          

        int pid=fork();
        //spec 3 ->hop
        if(pid<0){
            perror("failed\n");
        }else if(pid==0){
                if (arg_count > 0 && strncmp(args[0], "hop", 3) == 0) {
                    // printf("yes, hop is compared\n");
                    if(strcmp(command_given,"hop")==0){
                        chdir(home_dir_store);
                        uid_t uid;
                        struct utsname uts;
                        char *username;
                        uid = getuid(); // Get username
                        username = getpwuid(uid)->pw_name;
                        uname(&uts);
                        sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~$\x1B[0m> ", username, uts.nodename);
                        printf("/home/%s\n",username);
                    }else{
                        hop(arg_count, args,prompt,home_dir_store,prev_dir);  // Pass the number of arguments and the args array to hop
                    }
                }
                else if(arg_count>0 && strncmp(args[0],"reveal",6)==0){
                    // printf("yes reveal is compared\n");
                    // log_append(command_given);
                    
                    if(strcmp(command_given,"reveal")==0){
                        int count=0;//this is working
                        char **filenames = malloc(4096 * sizeof(char *));
                        filenames=reveal_a(".",&count);
                        qsort(filenames, count, sizeof(char *), compare_strings);
                        for (int i = 0; filenames[i] != NULL; i++) {
                            printf("%s\n", filenames[i]);  // Just print file names
                            free(filenames[i]); 
                        }
                    }
                    else{
                       
                        reveal(arg_count,args,home_dir_store);
                    }
                }
                else if(arg_count>0 && strncmp(args[0],"log",3)==0){
                    // printf("%s\n",command_given);
                    log_fun(command_given,home_dir_store,user_input,&flag_in);
                }
                else if(arg_count>0 && strncmp(args[0],"proclore",8)==0){
                    pid_t process_id;
                    if(arg_count==2){
                        process_id=atoi(args[1]);
                    }
                    else{
                        process_id=getpid();
                    }
                    proclore_info(process_id);
                }
                else if(arg_count>0 && strncmp(args[0],"seek",4)==0){
                    new_seek(arg_count,args,prompt);
                }
                // else if(arg_count>0 && strncmp(args[0],"sleep",5)==0){
                //     int time=atoi(args[1]);
                //     if(time>2){
                //         uid_t uid;
                //         struct utsname uts;
                //         char *username;
                //         uid = getuid(); // Get username
                //         username = getpwuid(uid)->pw_name;
                //         uname(&uts);
                //         printf("sleep is taking more than 2 seconds\n");
                //         sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94msleep: %ds$\x1B[0m> ", username, uts.nodename,time);
                //     }
                // }
                else{
                    if(execvp(args[0],args)==-1){
                        // perror("failed\n");
                        printf("ERROR:%s is not valid command\n",args[0]);
                    }
                }
                
            }
    
        else{
            if(!args_bg[i]){
               waitpid(pid, NULL, 0);    
            }else{
                printf("%d\n",pid);
                char log_path[4096];
                snprintf(log_path, sizeof(log_path), "%s/%s", home_dir_store,"pid_store.txt");
             
                FILE *fp = fopen(log_path, "a");
                if (fp == NULL) {
                    perror("Error opening file");
                    
                }
                fprintf(fp,"%s %d\n",args[0],pid);
                fclose(fp);
            }
        }
        
        
        }
    }

    
    return 0;
}
