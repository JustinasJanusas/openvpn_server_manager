#include "string_util.h"

int get_clients(char *buffer, char *lastline, int *start_found)
{
    int rc = 1;
    char *ptr1, *ptr2;
    struct node *tmp = NULL; 
    strncpy(lastline, strrchr(buffer, '\n'), 199);
    if( *start_found ){
        if(ptr1 = strstr(buffer, START_STRING)){
            ptr1 += strlen(START_STRING)+2;
        }
        else{
            ptr1 = buffer;
        }
        while(ptr1[0] == '\n' || ptr1[0] == '\r'){
            ptr1++;
        }
    }
    else{
        *start_found = 1;
        ptr1 = strstr(buffer, START_STRING);
        if( !ptr1 ){
            return 1;
        }
        if(strlen(ptr1) < strlen(START_STRING)+3){
            return 1;
        }
        ptr1 += strlen(START_STRING)+2;
    }
    if( ptr1 ){
        if( ptr2 = strstr(buffer, END_STRING) ){
            ptr2[0] = '\0';
            rc = 0;
        }
        int len = 0;
        while( (len = strlen(ptr1)) > 2 ){
            ptr2 = strstr(ptr1, "\r");
            if( ptr2 ){
                ptr2[0] = '\0';
                add_new_node(ptr1);
                ptr1 = ptr2+2; 
            }
            else{
                break;
            }
        }
	}
    write_lock = 0;
    return rc;
}

