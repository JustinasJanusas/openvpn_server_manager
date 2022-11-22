#include <libubus.h>
#include <pthread.h>

#include "socket_util.h"

int ubus_setup(struct ubus_context **ctx, char *server_name);
int create_ubus_thread(pthread_t **thread_id);