#include"header.h"
int compare_strings(const void *a, const void *b) {
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return strcmp(str_a, str_b);
}


void reveal_l_dir(char *path, char *filename) {
    struct stat fileStat;
    char fullPath[4096];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, filename);

    if (stat(fullPath, &fileStat) == -1) {
        printf("dir: is %s\n", fullPath);
        perror("stat");
        return;
    }

    struct passwd *pwd = getpwuid(fileStat.st_uid);
    struct group *grp = getgrgid(fileStat.st_gid);

    if (S_ISDIR(fileStat.st_mode)) {
        printf("\x1B[94m");  // Blue for directories
    } else if (fileStat.st_mode & S_IXUSR) {
        printf("\x1B[32m");  // Green for executables
    }

    printf("%c", (fileStat.st_mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (fileStat.st_mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (fileStat.st_mode & S_IROTH) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXOTH) ? 'x' : '-');
    printf(" %ld ", fileStat.st_nlink);
    printf("%s %s ", pwd->pw_name, grp->gr_name);
    printf("%5lld ", (long long) fileStat.st_size);
    printf("%.12s ", ctime(&fileStat.st_mtime) + 4);
    printf("%s\x1B[0m\n", filename);  // Reset color after printing filename
}

void reveal_l(char *filename) {
    struct stat fileStat;
    if (stat(filename, &fileStat) == -1) {
        printf("dir: is %s\n", filename);
        perror("stat");
        return;
    }

    struct passwd *pwd = getpwuid(fileStat.st_uid);
    struct group *grp = getgrgid(fileStat.st_gid);

    if (S_ISDIR(fileStat.st_mode)) {
        printf("\x1B[94m");  // Blue for directories
    } else if (fileStat.st_mode & S_IXUSR) {
        printf("\x1B[32m");  // Green for executables
    }

    printf("%c", (fileStat.st_mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (fileStat.st_mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (fileStat.st_mode & S_IROTH) ? 'r' : '-');
    printf("%c", (fileStat.st_mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (fileStat.st_mode & S_IXOTH) ? 'x' : '-');
    printf(" %ld ", fileStat.st_nlink);
    printf("%s %s ", pwd->pw_name, grp->gr_name);
    printf("%5lld ", (long long) fileStat.st_size);
    printf("%.12s ", ctime(&fileStat.st_mtime) + 4);
    printf("%s\x1B[0m\n", filename);  // Reset color after printing filename
}

char **reveal_a(char *path, int *count) {
    char **filenames = malloc(4096 * sizeof(char *));
    *count = 0;
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }
    while ((entry = readdir(dir)) != NULL) {
        filenames[(*count)++] = strdup(entry->d_name);
    }
    filenames[*count] = NULL;
    closedir(dir);
    return filenames;
}

char **reveal_files(char *path, int *count) {
    char **filenames = malloc(4096 * sizeof(char *));
    *count = 0;
    struct dirent *entry;
    struct stat fileStat;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }
    char fullPath[4096];
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
        if (stat(fullPath, &fileStat) == -1) {
            perror("stat");
            continue;
        }
        filenames[(*count)++] = strdup(entry->d_name);
    }
    filenames[*count] = NULL;
    closedir(dir);
    return filenames;
}

void reveal(int argc_count, char **args, char *home_dir_store) {
    int flag_a = 0;
    int flag_l = 0;
    char pwd[4096];
    getcwd(pwd,4096);
    char *path = pwd;

    for (int i = 1; i < argc_count; i++) {
        if (args[i][0] == '-') {
            for (int j = 1; args[i][j] != '\0'; j++) {
                if (args[i][j] == 'l') {
                    flag_l = 1;
                } else if (args[i][j] == 'a') {
                    flag_a = 1;
                } else {
                    printf("INVALID argument: -%c\n", args[i][j]);
                    return;
                }
            }
        } else {
            path = args[i];
        }
    }
    if(strcmp(path,"~")==0){
            strcpy(path,home_dir_store);
            // printf("path:%s\n",path);
        
    }
    struct stat path_stat;
    if (stat(path, &path_stat) == -1) {
        perror("stat");
        return;
    }

    
    if (S_ISREG(path_stat.st_mode)) {
        if ((flag_a && !flag_l) || (!flag_a && !flag_l)) {
            printf("%s\n", path);
        } else if (flag_l || (flag_l && flag_a)) {
            reveal_l(path);
        }
    } else if (S_ISDIR(path_stat.st_mode)) {
        int count = 0;
        
        
        char **filenames = NULL;
        if (strncmp(path, home_dir_store, strlen(home_dir_store)) != 0) {
            char temp[4096];
            snprintf(temp, sizeof(temp), "%s/%s", home_dir_store, path);
            strcpy(path, temp);
        }

        else if(strcmp(path,"~")==0){
            strcpy(path,home_dir_store);
            // printf("path:%s\n",path);
        }
        if (flag_a && flag_l) {
            filenames = reveal_a(path, &count);
        } else if (!flag_a && flag_l) {
            filenames = reveal_files(path, &count);
        } else if (flag_a && !flag_l) {
            filenames = reveal_a(path, &count);
        }

        if (filenames != NULL) {
            qsort(filenames, count, sizeof(char *), compare_strings);
            for (int i = 0; filenames[i] != NULL; i++) {
                if (flag_l) {
                    reveal_l_dir(path, filenames[i]);
                } else {
                    printf("%s\n", filenames[i]);
                }
                free(filenames[i]);
            }
            free(filenames);
        }
    }
}