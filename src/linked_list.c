#include "linked_list.h"

struct node *head;

struct node* create_node(char *line)
{
    struct node *node;
    char *delim = ",";
    node = (struct node*) malloc(sizeof(struct node));
    node->next = NULL;
    char *name = strtok(line, delim);
    char *address = strtok(NULL, delim);
    char *rec_bytes = strtok(NULL, delim);
    char *sent_bytes = strtok(NULL, delim);
    char *con_time = strtok(NULL, delim);
    if( !name || !address || !rec_bytes || !sent_bytes || !con_time ){
        return NULL;
    }
    strncpy(node->common_name, name, 30);
    strncpy(node->address, address, 30);
    node->rec_bytes = atol(rec_bytes);
    node->sent_bytes = atol(sent_bytes);
    strncpy(node->con_time, con_time, 30);
    return node;
}

void add_new_node(char *line)
{
    if( !(head) ){
        head = create_node(line);
        return;
    }
    struct node *tmp;
    tmp = head;
    while( tmp->next != NULL ){
        tmp = tmp->next;
    }
    tmp->next = create_node(line);
}

void free_all_nodes()
{
    struct node *to_delete;
    while (head) {
        to_delete = head;
        head = head->next;
        free(to_delete);
    }
}

