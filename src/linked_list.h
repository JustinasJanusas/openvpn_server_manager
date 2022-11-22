#include <syslog.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>



struct node {
    char common_name[30];
    char address[30];
    long rec_bytes;
    long sent_bytes;
    char con_time[30];
    struct node *next; 
};

extern struct node *head;

static volatile int read_lock = 0;
static volatile int write_lock = 0;
struct node* create_node(char *line);
void add_new_node(char *line);
void free_all_nodes();
struct node* get_head();