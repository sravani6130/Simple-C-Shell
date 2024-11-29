#include"header.h"
void proclore_info(pid_t pid){
    char stat_path[1024];
    char path[1024];
    char status;
    int pgid;
    long vsize;
    sprintf(stat_path, "/proc/%d/stat", pid);
    FILE *stat_file = fopen(stat_path, "r");
    if (!stat_file) {
        perror("fopen");
        return ;
    }
    fscanf(stat_file, "%*d %*s %c", &status);
    fclose(stat_file);

    // Get process group
    char pgid_path[1024];
    sprintf(pgid_path, "/proc/%d/stat", pid);
    FILE *pgid_file = fopen(pgid_path, "r");
    if (!pgid_file) {
        perror("fopen");
        return ;
    }
    fscanf(pgid_file, "%*d %*s %*c %d", &pgid);
    fclose(pgid_file);

    // Get virtual memory size
    char vsize_path[1024];
    sprintf(vsize_path, "/proc/%d/status", pid);
    FILE *vsize_file = fopen(vsize_path, "r");
    if (!vsize_file) {
        perror("fopen");
        return ;
    }
    char line[1024];
    while (fgets(line, 1024, vsize_file)) {
        if (strstr(line, "VmSize:")) {
            sscanf(line, "%*s %ld", &vsize);
            break;
        }
    }
    fclose(vsize_file);

    // Get executable path
    char exe_path[1024];
    sprintf(exe_path, "/proc/%d/exe", pid);
    ssize_t len = readlink(exe_path, path, 1024);
    if (len == -1) {
        perror("readlink");
        return ;
    }
    path[len] = '\0';

    pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);
    // Print process information
    printf("pid : %d\n", pid);
    printf("process status : ");
    switch (status) {
        case 'R':
            printf("R");
            break;
        case 'S':
            printf("S");
            break;
        case 'Z':
            printf("Z");
            break;
        default:
            printf("Unknown");
    }
    if (pgid == fg_pgid &&(status == 'R' || status == 'S')) {
        printf("+");
    }
    printf("\n");
    printf("Process Group : %d\n", pgid);
    printf("Virtual memory : %ld\n", vsize);
    printf("executable path : %s\n", path);

}