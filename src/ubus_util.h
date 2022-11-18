#include <libubus.h>
#include <pthread.h>

#include "linked_list.h"

int ubus_setup(struct ubus_context **ctx, char *server_name, pthread_t **thread_id);