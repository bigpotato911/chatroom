#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client_node.h"
#define PORT_NUM 8888

enum message_type {
	LOGIN,
	MESSAGE,
	LOGOUT,
	UPDATE_ALL_ONLINE_PERSON_INFO
};

typedef struct client client_t;

struct client {
	int client_fd;
	char login_name[20];
	struct sockaddr_in client_addr;
};

#define PACKET_MAX_SIZE 1400

int main(int argc, char  *argv[])
{
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == -1) {
		perror("listen socket");
		exit(1);
	}

	int reuse = 1;
	int ret;
	ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, 
		&reuse, sizeof(int));
	if (ret == -1) {
		perror("setsockopt");
		exit(1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(listen_fd, (struct sockaddr *)&server_addr,
		sizeof(struct sockaddr_in));
	if (ret == -1) {
		perror("bind");
		exit(2);
	}

	ret = listen(listen_fd, 20);
	if (ret == -1) {
		perror("listen");
		exit(3);
	}

	client_node_t *head = NULL;
	client_node_t *temp;
	fd_set readset;
	FD_ZERO(&readset);
	int max_fd = listen_fd;

	FD_SET(listen_fd, &readset);
	
	while (1) {
	

		temp = head;
		while (temp != NULL) {
			int temp_fd = ((client_t *)temp->data)->client_fd;
			if (temp_fd > max_fd) {
				max_fd = temp_fd;
			}
			FD_SET(temp_fd, &readset);
			temp = temp->next;
		}
		ret = select(max_fd+1, &readset, NULL, NULL, NULL);
		if (ret < 0) {
			perror("select");
			exit(4);
		} else if (ret > 0) {
			if (FD_ISSET(listen_fd, &readset)) {
				struct sockaddr_in client_addr;
			
				socklen_t client_len = sizeof(client_addr);
				int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr,
				 &client_len);
				if (client_fd == -1) {
					perror("accept");
					exit(5);
				}
				client_t client;
				client.client_fd = client_fd;
				bzero(client.login_name, sizeof(client.login_name));

				int recv_count = recv(client_fd, client.login_name,
					sizeof(client.login_name), 0);
				if (recv_count < 0) {
					perror("recv");
				}
				memcpy(&client.client_addr, &client_addr, client_len);
				insert_node(head, &client, sizeof(client_t));
			}
		}
	}
	return 0;
}