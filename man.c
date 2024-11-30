#include "header.h"

#define PORT 80
#define BUFFER_SIZE 4096  

int iMan(char *command_name) {
    char request[1024];
    sprintf(request, "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_name);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 0;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "65.19.140.5", &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        close(sockfd);  
        return 0;
    }

    send(sockfd, request, strlen(request), 0);

    char buffer[BUFFER_SIZE];
    int bytes_received;
    int in_tag = 0;  
    int manpage_found = 0;  
    int no_entry_found = 0;  

    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        for (int i = 0; i < bytes_received; i++) {
            if (buffer[i] == '<') {
                in_tag = 1;  
            } else if (buffer[i] == '>') {
                in_tag = 0;  
            } else if (!in_tag) {
                putchar(buffer[i]);  
            }
        }
        if (strstr(buffer, "No manual entry for") != NULL) {
            no_entry_found = 1;
        } else {
            manpage_found = 1;
        }
    }

    if (bytes_received < 0) {
        perror("recv failed");
    }

    close(sockfd);

    if (no_entry_found) {
        manpage_found = 0;
    }

    
    return manpage_found;
}
