#include "header.h"
void get_home(char *home_dir_store) {
    getcwd(home_dir_store, 1024);
}
void get_shell_prompt(char *prompt) {
    uid_t uid;
    struct utsname uts;
    char *username;
    char cwd[1024];
    char home[1024];

    uid = getuid(); // Get username
    username = getpwuid(uid)->pw_name;

    uname(&uts); // Get system name

    getcwd(cwd, sizeof(cwd)); // Get current working directory
    struct passwd *pw;
    pw = getpwuid(uid);
    strcpy(home, pw->pw_dir);

    if (strncmp(cwd, home, strlen(home)) == 0) {
        sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~$\x1B[0m> ", username, uts.nodename);
        // sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename,cwd+strlen(home));
    } else {
        char *last_dir;
        // Extract the last directory name from cwd
        last_dir = strrchr(cwd, '/');
        if (last_dir != NULL) {
            last_dir++; // Move past the '/' character
        } else {
            last_dir = cwd; // In case cwd has no '/' character
        }

        // sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m%s$\x1B[0m> ", username, uts.nodename, last_dir);

        sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m%s$\x1B[0m> ", username, uts.nodename, cwd);
    }
}

