#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <curl/curl.h>
#include <netdb.h>
extern char home_global[1024];
extern char prev_dir[1024];
extern char prompt[4096];
extern char user_input[1024];
extern pid_t sheel;
void get_home(char *home_dir_store);
void get_shell_prompt(char *prompt);
void hop(char *arg);

int compare_strings(const void *a, const void *b);
void print_file_info(const char *fullPath, const struct stat *fileStat, const char *filename);
void reveal_l_dir(char *path, char *filename);
void reveal_l(char *filename);
char **reveal_a(char *path, int *count);
char **reveal_files(char *path, int *count);
void reveal(char *cmds);

void log_append(char *command, char *home_dir);
void log_display(char *home_dir);
void log_purge(char *home_dir);
void delete_old(char *filename);
int lines_in_file(char *filename);
void execute_log_line(int line_num, char *filename);
void log_fun(char *command);
void tokenize_commands(char *user_input, char *and_args[1024], int *flag_bg, int *and_count);

void proclore_info(pid_t pid);

void print_result(const char *path, int is_dir);
void new_seek(char *argv);
void search_dir(const char *dir_path, const char *target, int flag_d, int flag_f, int flag_e, int *match_count, char *prompt);

void trim_spaces(char *str);
char *extract_filename(const char *ptr);
int handle_input_redirection(char *cmd);
int handle_output_redirection(char *cmd);
int check_for_manual_commands(char *arg);
void execute_pipe_redirect(int and_count, char **and_args, int *flag_bg);

void handle_sigchld(int sig);


void print_process_info(const int pid, const char *process_name, const char *state);
void list_shell_processes();


void ping_command(char *input);
void signal_handler(int signum);

void fg_func(char* command);
void bg_func(char* command);

int iMan(char *command_name);


//neonate
void set_raw_mode(struct termios *orig_term_attr);
void reset_terminal_mode(struct termios *orig_term_attr);
void handle_signal(int sig);
int get_most_recent_pid();
void run_neonate(char *args);

int get_most_recent_pid();


