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
        while( (len = strlen(ptr1)) > 2 ){
            ptr2 = strstr(ptr1, "\r");
            ptr2[0] = '\0';
            add_new_node(ptr1);
            ptr1 = ptr2+2; 
        }
	}
    write_lock = 0;
    return rc;
}

int read_client_list( int sockfd )
{
    int keep_looking = 1;
    int char_count;
    char buffer[1024];
    while( keep_looking ){
        char_count = recv(sockfd, buffer, 1023, 0);
        buffer[char_count] = '\0';
        if( char_count > 0 ){
            keep_looking = get_clients(buffer);
        }
        else{
            keep_looking = 0;
        }
    }

    return 0;
}