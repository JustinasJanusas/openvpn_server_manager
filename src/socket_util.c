#include "socket_util.h"

int sockfd = -1;

int management_setup(int *sockfd, int port)
{
    int rc = 0;
    struct sockaddr_in serv_addr;

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( *sockfd < 0 ){
        syslog(LOG_ERR, "Failed to create socket descriptor");
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    rc = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    if( rc != 1 ){
        syslog(LOG_ERR, "Invalid host adress: %d", rc);
        return 2;
    }
    rc = connect(*sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    
    if( rc ){
        syslog(LOG_ERR, "Failed to connect to server: %s", strerror(errno));
        rc = 2;
    }
    return rc;
}

int read_client_list( int sockfd )
{
    int keep_looking = 1, start_found = 0;
    int char_count;
    char buffer[1024];
    char big_buffer[1224];
    char last_line[200];
    while( read_lock ){
        sleep(0.2);
    }
    write_lock = 1;
    free_all_nodes();
    while( keep_looking ){
        
        char_count = recv(sockfd, buffer, 1023, 0);
        buffer[char_count] = '\0';
        // sprintf(buffer, "%s%s", last_line, buffer);
        strcpy(big_buffer, last_line);
        strcat(big_buffer, buffer);
        if( char_count > 0 ){
            keep_looking = get_clients(big_buffer, last_line, &start_found);
        }
        else{
            keep_looking = 0;
        }
    }

    return 0;
}