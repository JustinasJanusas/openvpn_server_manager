#include "linked_list.h"



#define START_STRING "Common Name,Real Address,Bytes Received,Bytes Sent,Connected Since"
#define END_STRING "ROUTING TABLE"

int get_clients(char *buffer, char *last_line, int *start_found);