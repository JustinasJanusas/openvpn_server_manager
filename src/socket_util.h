#include <syslog.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

extern int sockfd;

int management_setup(int *sockfd, int port);