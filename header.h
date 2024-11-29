#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/utsname.h>
#include<pwd.h>
#include<ctype.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include<errno.h>
#include<termios.h>
extern char home_global[1024];
void get_home(char *home_dir_store);
void get_shell_prompt(char*prompt);
void hop(int argc_count,char **args,char*prompt,char*home_dir_store,char*prev_dir);
void reveal(int argc_count,char **args,char *home_dir_store);
void reveal_l(char *filename);
char ** reveal_a(char *path,int *count);
char ** reveal_files(char *path,int *count);
void reveal_l_dir(char *path,char *filename);
void log_fun(char *command, char *home_dir,char *user_input,int *flag);
void log_display(char *home_dir); 
void log_purge(char *home_dir);
void log_append(char *command, char *home_dir);
void delete_old(char *filename);
int lines_in_file(char *filename);
void execute_log_line(int line_num, char *filename,char *user_input,int *flag);
int compare_strings(const void *a, const void *b);
void proclore_info(pid_t pid);
void handle_sigchld(int sig) ;
void print_result(const char *path, int is_dir);
void search_directory(const char *dir_path, const char *target, int flag_d, int flag_f, int flag_e, int *match_count,char *prompt);
void seek(int argc, char **argv,char *prompt) ;
void new_seek(int argc, char **argv,char* prompt);
void search_dir(const char *dir_path, const char *target, int flag_d, int flag_f, int flag_e, int *match_count, char *prompt);