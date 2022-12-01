#include <libubus.h>
#include <pthread.h>

#include "socket_util.h"

int ubus_setup(char *server_name);
int create_ubus_thread(pthread_t **thread_id, char *server_name);