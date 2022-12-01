
#include <unistd.h>

#include "string_util.h"


extern int sockfd;

int management_setup(int *sockfd, int port);
int read_client_list( int sockfd );