/*
 ============================================================================
 Name        : proj_server.c
 Author      : fj
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdlib.h>//write
#include <sys/types.h> //pid_t

struct Node {
	int data;
	struct Node *next;
};
int socket_desc, new_socket, c;
struct sockaddr_in server, client;
char portc[6];
pid_t pid;
void reverse(struct Node** head_ref);
void swap(struct Node *a, struct Node *b);
void dodawanie(struct Node *start);
void odejmowanie(struct Node *start);
void rosnaco(struct Node *start);
void malejaco(struct Node *start);
void send_list(struct Node *start);
void print_list(struct Node *start);
void deleteList(struct Node** head_ref);
void push(struct Node **start, int val);
int main(int argc, char *argv[]) {

	puts("\nWprowadz adres Port\n");
	fgets(portc, sizeof(portc), stdin);
	int port = atoi(portc);
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		puts("Problem dolaczenia gniazda");
	}
	puts("Dolaczono gniazdo strumieniowe");
	listen(socket_desc, 3);
	puts("SERWER TCP: Czekam na polaczenie...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *) &client,
			(socklen_t*) &c);
	if (new_socket < 0) {
		perror("Problem z akceptacja poloczenia");
	}
	pid = fork();
	puts("Polaczenie zaakceptowane");

	while (1) {
		if (pid == 0) {
			struct Node *start = NULL;

			int read_size;
			char client_message[3000];

			while (1) {
				while ((read_size = recv(new_socket, client_message, 3000, 0))
						> 0) {
					puts("SERWER: Dane odebrano");
					puts(client_message);

					if (client_message[0] == '/') {
						puts("jestem w komendy");

						if (client_message[1] == 'd') {
							memset(client_message, 0, sizeof client_message);

							dodawanie(start);
							deleteList(&start);

						} else if (client_message[1] == 'r') {
							memset(client_message, 0, sizeof client_message);

							print_list(start);
							rosnaco(start);
							print_list(start);
							send_list(start);
							deleteList(&start);

						} else if (client_message[1] == 'o') {
							memset(client_message, 0, sizeof client_message);
							reverse(&start);
							odejmowanie(start);
							deleteList(&start);

						} else if (client_message[1] == 'm') {
							memset(client_message, 0, sizeof client_message);

							print_list(start);
							malejaco(start);
							print_list(start);
							send_list(start);
							deleteList(&start);

						} else {
						}

					} else {
						push(&start, atoi(client_message));
						memset(client_message, 0, sizeof client_message);

					}
				}
			}
		} else {
			new_socket = accept(socket_desc, (struct sockaddr *) &client,
					(socklen_t*) &c);
			if (new_socket < 0) {
				perror("Problem z akceptacja poloczenia");
			}
			pid = fork();

		}

	}
	return 0;
}

void swap(struct Node *a, struct Node *b) {
	int temp = a->data;
	a->data = b->data;
	b->data = temp;
}

void dodawanie(struct Node *start) {
	struct Node * current = start;
	int i = 0;
	while (current != NULL) {
		printf("%d\n", current->data);
		i = i + current->data;
		current = current->next;
	}
	char str[] = "Wynik dodawania :%d\n";
	char str2[1000];
	sprintf(str2, str, i);
	printf("%s\n", str2);
	write(new_socket, str2, strlen(str2));
}

void odejmowanie(struct Node *start) {
	struct Node * current = start;
	int i = current->data;
	current = current->next;
	while (current != NULL) {
		printf("%d\n", current->data);
		i = i - current->data;
		current = current->next;

	}
	char str[] = "Wynik odejmowania:%d\n";
	char str2[1000];
	sprintf(str2, str, i);
	printf("%s\n", str2);
	write(new_socket, str2, strlen(str2));
}
 void reverse(struct Node** head_ref)
{
    struct Node* prev   = NULL;
    struct Node* current = *head_ref;
    struct Node* next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head_ref = prev;
}

void rosnaco(struct Node *start) {
	int swapped, i;
	struct Node *ptr1;
	struct Node *lptr = NULL;
	if (ptr1 == NULL)
		return;
	do {
		swapped = 0;
		ptr1 = start;

		while (ptr1->next != lptr) {
			if (ptr1->data > ptr1->next->data) {
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}

void malejaco(struct Node *start) {
	int swapped, i;
	struct Node *ptr1;
	struct Node *lptr = NULL;
	/* Checking for empty list */
	if (ptr1 == NULL)
		return;
	do {
		swapped = 0;
		ptr1 = start;

		while (ptr1->next != lptr) {
			if (ptr1->data < ptr1->next->data) {
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}

void send_list(struct Node *start) {
	struct Node * current = start;
	char str[] = "Posortowane liczby: ";
	write(new_socket, str, strlen(str));
	char str2[100];
	while (current != NULL) {
		sprintf(str2, "%d, ", current->data);
		write(new_socket, str2, strlen(str2));
		current = current->next;
	}
}

void print_list(struct Node *start) {
	struct Node * current = start;
	while (current != NULL) {
		printf("%d\n", current->data);
		current = current->next;
	}
}

void deleteList(struct Node** head_ref) {
	/* deref head_ref to get the real head */
	struct Node* current = *head_ref;
	struct Node* next;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	*head_ref = NULL;
}

void push(struct Node **start, int val) {
	struct Node * current = start;
	struct Node *ptr1 = (struct Node*) malloc(sizeof(struct Node));

	ptr1->data = val;
	ptr1->next = *start;
	*start = ptr1;

}

