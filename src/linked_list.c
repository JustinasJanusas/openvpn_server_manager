#include "linked_list.h"

struct node *head;

struct node* create_node(char *line)
{
    struct node *node;
    char *delim = ",";
    node = (struct node*) malloc(sizeof(struct node));
    node->next = NULL;
    strncpy(node->common_name, strtok(line, delim), 30);
    strncpy(node->address, strtok(NULL, delim), 30);
    node->rec_bytes = atol(strtok(NULL, delim));
    node->sent_bytes = atol(strtok(NULL, delim));
    strncpy(node->con_time, strtok(NULL, delim), 30);
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

