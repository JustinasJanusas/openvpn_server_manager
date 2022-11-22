#include <signal.h>
#include <stdio.h>
#include <argp.h>
#include <unistd.h>
#include <fcntl.h>


#include "ubus_util.h"



volatile sig_atomic_t daemonize = 1;

const char *argp_program_version =  "openvpn_server_manager 1.0.0";
const char *argp_program_bug_address =  "<bug-gnu-utils@gnu.org>";
static char doc[] = "openvpn_server_manager -- outputs server clients";



static struct argp_option options[] = {
    {"port", 'p', "port", 0, "Port"},
	{"server_name", 'n', "server_name", 0, "Server name"},
    { 0 }
};

struct arguments{
    int port;
	char server_name[30];
};



static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;
	switch ( key ){
        case 'p':
            arguments->port = atoi(arg);
            break;
		case 'n':
			strncpy(arguments->server_name, arg, 29);
			arguments->server_name[29] = '\0';
			break;
		default:
			return ARGP_ERR_UNKNOWN;
			
	}
	return 0;
}

static struct argp argp = { options, parse_opt, 0, doc };

static void term_proc(int sigterm) 
{
	daemonize = 0;
}

int init(pthread_t **thread_id, struct ubus_context **ctx, struct argp *argp, 
		int argc, char **argv, struct arguments *arguments)
{
	int rc = 0;
	struct sigaction action;
	struct flock lock, savelock;
	
	//set sigaction
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = term_proc;
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGINT, &action, NULL);
	//parse args
	argp_parse(argp, argc, argv, 0, 0, arguments);
	//socket
	rc = management_setup(&sockfd, arguments->port);
	if( rc ){
		syslog(LOG_ERR, "Failed to setup management socket: %d", rc);
		return rc;
	}
	rc = ubus_setup(ctx, arguments->server_name);
	if( rc ){
		return rc;
	}
	rc = create_ubus_thread(thread_id);
	if( rc ){
		return 4;
	} 
	return rc;
}

void cleanup(pthread_t **thread_id, struct ubus_context **ctx, int err)
{
	switch (err)
	{
		case 0:
			break;
		case 1:
			goto cleanup_final;
		case 2:
			goto cleanup_socket;
		case 3:
			goto cleanup_uloop;
		case 4:
			goto cleanup_ubus;
		default:
			break;
	}
	if(*thread_id){
		pthread_cancel(**thread_id);
		free(*thread_id);
	}
	free_all_nodes();
	cleanup_ubus:
		ubus_free(*ctx);
	cleanup_uloop:
		uloop_done();
	cleanup_socket:
		close(sockfd);
	cleanup_final:
		syslog(LOG_INFO, "openvpn_server_manager was stopped");
		closelog();
}

int main(int argc, char **argv)
{
	openlog("openvpn_server_manager", LOG_PID, LOG_USER);
	int rc = 0;
	pthread_t *thread_id;
	struct ubus_context *ctx;
	struct arguments arguments;
	rc = init(&thread_id, &ctx, &argp, argc, argv, &arguments);
	if( rc ){
		goto cleanup;
	}
	int sent = 0;
	int counter = 0;
	syslog(LOG_INFO, "openvpn_server_manager started successfully");
	while( daemonize ) {
		sent = send(sockfd, "status\n", sizeof("status\n"), 0);
		if( sent == -1 ){
			syslog(LOG_ERR, "Failed to send data to management socket");
			counter++;
			if( counter > 5 ){
				daemonize = 0;
				continue;
			}
			sleep(10);
			management_setup(&sockfd, arguments.port);
			continue;
		}
		else{
			counter = 0;
		}
		read_client_list(sockfd);
		sleep(10);
		
	}
	int lastrc = rc;
	rc = 0;
	cleanup:
		cleanup(&thread_id, &ctx, rc);
		return lastrc;
}
