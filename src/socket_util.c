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
        return 1;
    }
    rc = connect(*sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    
    if( rc ){
        syslog(LOG_ERR, "Failed to connect to server: %s", strerror(errno));
        
    }
    return rc;
}


