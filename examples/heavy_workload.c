#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

int 
send_message(
        int socket_desc, 
        struct sockaddr_in server_addr,
        socklen_t server_struct_length,
        char *message) {
    printf("%s\n", message);
    if(sendto(socket_desc, message, 128, 0,
         (struct sockaddr*)&server_addr, server_struct_length) < 0){
        printf("Unable to send message\n");
        return -1;
    }
    return 0;
}

void
loop(int iteration) {
    for (int i = 0 ; i < iteration ; i++) {
        printf("%d\n", i);
    }
}

int main(void){
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    socklen_t server_struct_length = sizeof(server_addr);
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    pid_t pid = getpid();

    char *start_message = (char*)malloc(128 * sizeof(char));
    char *stop_message = (char*)malloc(128 * sizeof(char));    
    
    int i = 0;
    for (i = 0 ; i < 500 ; i++) {
        sprintf(start_message, "%s %d %s%d", "start", pid, "main", i);
        send_message(socket_desc, server_addr, server_struct_length, start_message);
        if(recvfrom(socket_desc, server_message, sizeof(server_message), 0, (struct sockaddr*)&server_addr, &server_struct_length) < 0){
            printf("Error while receiving server's msg\n");
            return -1;
        }
        loop(100);
        sprintf(stop_message, "%s %s%d", "stop", "main", i);
        send_message(socket_desc, server_addr, server_struct_length, stop_message);
        if(recvfrom(socket_desc, server_message, sizeof(server_message), 0, (struct sockaddr*)&server_addr, &server_struct_length) < 0){
            printf("Error while receiving server's msg\n");
            return -1;
        }
    }

    char *report_message = (char*)malloc(128 * sizeof(char) + 14 * sizeof(char));
    sprintf(report_message, "%s %s", "report", "report_c.json");
    send_message(socket_desc, server_addr, server_struct_length, report_message);
    if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
         (struct sockaddr*)&server_addr, &server_struct_length) < 0){
        printf("Error while receiving server's msg\n");
        return -1;
    }

    // Close the socket:
    close(socket_desc);
    free(start_message);
    free(stop_message);
    free(report_message);
    
    return 0;
}

