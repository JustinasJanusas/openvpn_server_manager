#include "string_util.h"

static int get_clients(char *buffer)
{
    while( read_lock ){
        sleep(0.2);
    }
    write_lock = 1;
    int rc = 1;
    char *ptr1, *ptr2;
    struct node *tmp = NULL; 

    if((ptr1 = strstr(buffer, START_STRING)) && (ptr2 = strstr(buffer, END_STRING))){
        rc = 0;
        ptr1 += strlen(START_STRING)+2;
        ptr2[0] = '\0';
        int len = 0;
        free_all_nodes();
        syslog(LOG_DEBUG, "get ready");
        while( (len = strlen(ptr1)) > 2 ){
            syslog(LOG_DEBUG, "len %d", strlen(ptr1));
            printf("here\n");
            ptr2 = strstr(ptr1, "\r");
            ptr2[0] = '\0';
            add_new_node(ptr1);
            syslog(LOG_DEBUG, "SAD %s", head->address);
            ptr1 = ptr2+2; 
        }
	}
    syslog(LOG_DEBUG, "head -> %s", head->address);
    write_lock = 0;
    return rc;
}

int read_client_list( int sockfd )
{
    int keep_looking = 1;
    int char_count;
    char buffer[1024];
    printf("in\n");
    while( keep_looking ){
        char_count = recv(sockfd, buffer, 1023, 0);
        buffer[char_count] = '\0';
        if( char_count > 0 ){
            keep_looking = get_clients(buffer);
        }
    }
    printf("out\n");
    syslog(LOG_DEBUG, "head -> %s", head->con_time);
    syslog(LOG_DEBUG, "IT HAPPENED");
    return 0;
}