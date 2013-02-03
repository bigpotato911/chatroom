#ifndef CLIENT_NODE_H
#define CLIENT_NODE_H

typedef struct client_node client_node_t;

struct client_node{
	void *data;
	client_node_t *next;
};

void insert_node(client_node_t *head, void *data, int len);
int del_node(client_node_t *head, void *data, int len);
void print_list(client_node_t *head, void (*print)(void *));
client_node_t *search(client_node_t *head, int (*search_cond)(void *));
void free_list(client_node_t *head);

#endif