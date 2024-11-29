#include"header.h"
#define LOG_FILE_NAME "log.txt"
void log_append(char *command, char *home_dir) {
    char log_path[1024];
    snprintf(log_path, sizeof(log_path), "%s/%s", home_dir, LOG_FILE_NAME);

    FILE *fp = fopen(log_path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    char last_line[4096] = "";
    char line[4096];
    while (fgets(line, sizeof(line), fp) != NULL) {
        strcpy(last_line, line);
    }
    last_line[strcspn(last_line, "\n")] = '\0';
    fclose(fp);
    if (strcmp(last_line, command) == 0) {
        // Last line is the same as the command, so do not append
        return;
    }
    fp = fopen(log_path, "a");
    int count = lines_in_file(log_path);
    if (count < 15) {
        fprintf(fp, "%s\n", command);
    } else {
        fclose(fp);  // Close the file before deleting or reopening
        delete_old(log_path);

        // Reopen the file after deleting old entries
        fp = fopen(log_path, "a");
        if (fp == NULL) {
            perror("Error opening file");
            return;
        }
        fprintf(fp, "%s\n", command);
    }
    fclose(fp);
}

void log_display(char *home_dir) {
    char log_path[1024];
    snprintf(log_path, sizeof(log_path), "%s/%s", home_dir, LOG_FILE_NAME);

    FILE *fp = fopen(log_path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    char line[4096];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }
    fclose(fp);
}

void log_purge(char *home_dir) {
    char log_path[1024];
    snprintf(log_path, sizeof(log_path), "%s/%s", home_dir, LOG_FILE_NAME);

    FILE *fp = fopen(log_path, "w");
    if (fp != NULL) {
        fclose(fp);
    }
}

void delete_old(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *temp_fp = fopen("temp.txt", "w");
    if (temp_fp == NULL) {
        perror("Error opening temporary file");
        fclose(fp);
        return;
    }

    char buffer[256];
    // Skip the first line
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        perror("Error reading file");
        fclose(fp);
        fclose(temp_fp);
        return;
    }

    // Copy the remaining lines to the temporary file
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        fputs(buffer, temp_fp);
    }

    fclose(fp);
    fclose(temp_fp);

    // Delete the original file and rename the temporary file
    if (remove(filename) != 0) {
        perror("Error deleting file");
    }

    if (rename("temp.txt", filename) != 0) {
        perror("Error renaming file");
    }
}

int lines_in_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    int count = 0;
    char c;

    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }

    fclose(fp);
    return count;
}
void execute_log_line(int line_num, char *filename,char *user_input,int *flag) {
    int current_line = 0;
    char line[4096];
    
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    int total_lines=lines_in_file(filename);
    int target=total_lines-line_num+1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        current_line++;
        if (current_line == target) {
            // printf("Executing line %d: %s", line_num, line);
            strcpy(user_input,line);
            *flag=1;

        }
    
    }

    if (current_line < line_num) {
        printf("Line %d not found. The file has only %d lines.\n", line_num, current_line);
    }

    fclose(fp);
}
void log_fun(char *command, char *home_dir,char *user_input,int *flag) {
    if (strcmp(command, "log") == 0) {
        log_display(home_dir);
    } else if (strcmp(command, "log purge") == 0) {
        log_purge(home_dir);
    }
    else{
        char *args[4096];    
        int arg_count;
        char command_append[4096];
        strcpy(command_append,command);
        char *token = strtok(command, " ");  
        arg_count = 0;

        while (token != NULL) {
            args[arg_count++] = token;  
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        char log_path[1024];
        snprintf(log_path, sizeof(log_path), "%s/%s", home_dir, LOG_FILE_NAME);
        if (strcmp(args[0], "log") == 0 && strcmp(args[1], "execute") == 0) {
        int line_num = atoi(args[2]);

        int no_lines = lines_in_file(log_path);

        if (line_num <= 0 || line_num > no_lines) {
            printf("Invalid line number. The file contains %d lines.\n", no_lines);
        } else {
            execute_log_line(line_num,log_path,user_input,flag);
        }
    }
    }
}