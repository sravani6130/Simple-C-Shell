#include "header.h"

void print_result(const char *path, int is_dir) {
    if (is_dir) {
        printf("\x1B[94m%s/\x1B[0m\n", path);  // Blue for directories
    } else {
        printf("\x1B[32m%s\x1B[0m\n", path);  // Green for files
    }
}

void search_directory(const char *dir_path, const char *target, int flag_d, int flag_f, int flag_e, int *match_count, char *prompt) {
    struct dirent *entry;
    uid_t uid;
    struct utsname uts;
    char *username;
    char cwd[1024];
    uid = getuid();  // Get username
    username = getpwuid(uid)->pw_name;
    uname(&uts);
    struct stat file_stat;
    DIR *dir = opendir(dir_path);

    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char relative_path[4096];
        snprintf(relative_path, sizeof(relative_path), "%s/%s", dir_path, entry->d_name);

        if (stat(relative_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        if (strncmp(entry->d_name, target, strlen(target)) == 0) {
            // Check if the entry matches the search criteria
            if ((flag_d && S_ISDIR(file_stat.st_mode)) || (flag_f && S_ISREG(file_stat.st_mode)) || (!flag_d && !flag_f)) {
                print_result(relative_path + strlen(dir_path) + 1, S_ISDIR(file_stat.st_mode));
                (*match_count)++;

                if (flag_e) {
                    if (S_ISDIR(file_stat.st_mode)) {
                        // Store the current working directory
                        char original_dir[1024];
                        getcwd(original_dir, sizeof(original_dir));

                        if (chdir(relative_path) == -1) {
                            printf("Missing permissions for task!\n");
                        } else {
                            getcwd(cwd, sizeof(cwd));
                            sprintf(prompt, "<\x1B[32m%s@%s\x1B[0m:\x1B[94m~%s$\x1B[0m> ", username, uts.nodename, cwd);
                            printf("Changed directory to %s\n", relative_path + strlen(dir_path) + 1);
                        }

                        // Restore the original directory after changing back
                        if (chdir(original_dir) == -1) {
                            perror("chdir");
                            return;
                        }
                    } else if (S_ISREG(file_stat.st_mode)) {
                        FILE *file = fopen(relative_path, "r");
                        if (file) {
                            char line[1024];
                            while (fgets(line, sizeof(line), file)) {
                                printf("%s", line);
                            }
                            fclose(file);
                        } else {
                            printf("Missing permissions for task!\n");
                        }
                    }
                }
            }
        }

        // Recursively search directories (ignoring '.' and '..')
        if (S_ISDIR(file_stat.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            search_directory(relative_path, target, flag_d, flag_f, flag_e, match_count, prompt);
        }
    }

    closedir(dir);
}

void seek(int argc, char **argv, char *prompt) {
    int flag_d = 0, flag_f = 0, flag_e = 0;
    char *target = NULL;
    char *target_dir = ".";  // Default to current directory

    // Parse flags and arguments
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                if (argv[i][j] == 'd') {
                    flag_d = 1;
                } else if (argv[i][j] == 'f') {
                    flag_f = 1;
                } else if (argv[i][j] == 'e') {
                    flag_e = 1;
                } else {
                    printf("Invalid flags!\n");
                    return;
                }
            }
        } else {
            if (!target) {
                target = argv[i];
            } else {
                target_dir = argv[i];
            }
        }
    }

    if (flag_d && flag_f) {
        printf("Invalid flags!\n");
        return;
    }

    if (!target) {
        printf("No target specified!\n");
        return;
    }

    // Ensure the target directory path is correctly resolved
    char resolved_target_dir[4096];
    realpath(target_dir, resolved_target_dir);

    // Change to the specified directory if it's given
    if (chdir(resolved_target_dir) == -1) {
        perror("chdir");
        return;
    }

    // Now search in the specified directory
    int match_count = 0;
    search_directory(".", target, flag_d, flag_f, flag_e, &match_count, prompt);

    if (match_count == 0) {
        printf("No match found!\n");
    }
}
