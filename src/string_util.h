
#include "ubus_util.h"



#define START_STRING "Common Name,Real Address,Bytes Received,Bytes Sent,Connected Since"
#define END_STRING "ROUTING TABLE"

int read_client_list(int sockfd);