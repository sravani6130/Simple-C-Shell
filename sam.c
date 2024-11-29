#define LOG_FILE_NAME "log.txt"

void log_append(char *command, char *home_dir) {
    char log_path[1024];
    snprintf(log_path, sizeof(log_path), "%s/%s", home_dir, LOG_FILE_NAME);

    FILE *fp = fopen(log_path, "a");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

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

void log_fun(char *command, char *home_dir) {
    if (strcmp(command, "log") == 0) {
        log_display(home_dir);
    } else if (strcmp(command, "log purge") == 0) {
        log_purge(home_dir);
    } else {
        // Handle other cases
    }
}
