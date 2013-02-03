#include <stdlib.h>
#include <string.h>

#include "client_node.h"

void insert_node(client_node_t *head, void *data, int len) {
	client_node_t *temp = (client_node_t *)malloc(sizeof(client_node_t));
	temp->data = malloc(len);
	memcpy(temp->data, data, len);

	temp->next = head;
	head = temp;
}

int del_node(client_node_t *head, void *data, int len) {
	client_node_t *temp = head;
	client_node_t *pre;

	if (temp == NULL) {
		return -1;
	}

	while (temp != NULL) {
		if (memcmp(data, temp->data, len) == 0) {
			if (temp == head) {
				head = temp->next;
				free(temp->data);
				free(temp);
				return 0;
			} else {
				pre->next = temp->next;
				free(temp->data);
				free(temp);
				return 0;
			}
		} else {
			pre = temp;
			temp = temp->next;
		}

	}
	return 1;
}

void print_list(client_node_t *head, void (*print)(void *)) {
	client_node_t *temp = head;
	while (temp != NULL) {
		print(temp->data);
		temp = temp->data;
	}
}

client_node_t *search(client_node_t *head, int (*search_cond)(void *)) {
	client_node_t *temp = head;
	while (temp != NULL) {
		int ret = search_cond(temp->data);
		if (ret == 1) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void free_list(client_node_t *head) {
	client_node_t *temp = head;
	while (temp != NULL) {
		head = temp->next;
		free(head->data);
		free(head);
		temp = head;
	}
}

