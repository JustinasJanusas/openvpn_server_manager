#include "ubus_util.h"


static int list_connected_devices(struct ubus_context *ctx, 
			struct ubus_object *obj, struct ubus_request_data *req, 
			const char *method, struct blob_attr *msg);

static int disconnect_device(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

static const struct blobmsg_policy disconnect_policy[] = {
	[0] = { .name = "common_name", .type = BLOBMSG_TYPE_STRING },
};

static const struct ubus_method management_methods[] = {
	UBUS_METHOD_NOARG("get", list_connected_devices),
	UBUS_METHOD("disconnect", disconnect_device, disconnect_policy)
};

static struct ubus_object_type management_object_type;
	

static struct ubus_object management_object;


static int list_connected_devices(struct ubus_context *ctx, 
			struct ubus_object *obj, struct ubus_request_data *req, 
			const char *method, struct blob_attr *msg)
{
    while( write_lock ){
        sleep(0.2);
    }
    read_lock = 1;
    struct blob_buf b = {};
	struct blob_buf b2;
    blob_buf_init(&b, 0);
    struct node *tmp = head;
	void *s;
	while( write_lock ){
		sleep(0.2);
	}
	char info[300];
	void* array = blobmsg_open_nested(&b, "client_info", true);
    while ( tmp )
    {
		void* cookie = blobmsg_open_nested( &b, NULL, false );
		
		blobmsg_add_string(&b, "name", tmp->common_name);
		blobmsg_add_string(&b, "address", tmp->address);
		blobmsg_add_u64(&b, "bytes_received", tmp->rec_bytes);
		blobmsg_add_u64(&b, "bytes_sent", tmp->sent_bytes);
		blobmsg_add_string(&b, "conection_time", tmp->con_time);

		blobmsg_close_table ( &b, cookie );
		tmp = tmp->next;
    }
	blobmsg_close_array(&b, array);
    ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);
    read_lock = 0;
	uloop_done();
    return 0;
}

static int disconnect_device(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
    struct blob_attr *tb[1];
	struct blob_buf b = {};
	
	blobmsg_parse(disconnect_policy, 1, tb, blob_data(msg), blob_len(msg));
	
	if (!tb[0])
		return UBUS_STATUS_INVALID_ARGUMENT;
	char* client_name = blobmsg_get_string(tb[0]);
	blob_buf_init(&b, 0);
	char message[100] = "kill ";
	strncat(message, client_name, 90);
	strcat(message, "\n");  
	send(sockfd, message, sizeof(message), 0);
	char buff[300];
	recv(sockfd, buff, 299, 0);
	blobmsg_add_string(&b, "response", buff);
	ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);
	return 0;
}

struct ubus_context *ctx;
int ubus_setup(char *server_name){
    int rc = 0;
	rc = uloop_init();
	if( rc ){
		syslog(LOG_ERR, "Failed to initiate uloop: %d", rc);
		return 2;
	}
    ctx = ubus_connect(NULL);
	if (!ctx) {
		syslog(LOG_ERR, "Failed to connect to ubus\n");
		return 3;
	}
    ubus_add_uloop(ctx); 
	char ubus_name[40] = "openvpn.";
	strncat(ubus_name, server_name, 30);
    management_object_type = (struct ubus_object_type)
							UBUS_OBJECT_TYPE(ubus_name , management_methods);
    management_object = (struct ubus_object){
        .name = ubus_name,
        .type = &management_object_type,
        .methods = management_methods,
        .n_methods = ARRAY_SIZE(management_methods),
    };
	rc = ubus_add_object(ctx, &management_object);
	if( rc ){
		syslog(LOG_ERR, "Error adding ubus object: %d", rc);
		return 4;
	}
    return 0;
}


static void *run_thread(char *server_name)
{
	int rc = 0;
	rc = ubus_setup(server_name);
	if( rc ){
		switch (rc)
		{
		case 3:
			goto thread_cleanup_uloop;
		case 4:
			goto thread_cleanup_ubus;
		default:
			goto thread_cleanup;
		}
	}
	uloop_run();
	thread_cleanup_ubus:
		ubus_free(ctx);
	thread_cleanup_uloop:
		uloop_done();
	thread_cleanup:
		pthread_exit(0);
}

int create_ubus_thread(pthread_t **thread_id, char *server_name)
{
	int rc = 0;
	*thread_id = (pthread_t *)malloc(sizeof(pthread_t));
    rc = pthread_create(*thread_id, NULL, run_thread, server_name);
	if( rc ){
		syslog(LOG_ERR, "Failed to create a new thread: %d", rc);
		free(*thread_id);
		return rc;
	}
	return rc;
}