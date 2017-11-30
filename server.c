#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdlib.h>//write

int main(int argc, char *argv[]) {
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;
	int read_size;
	char *message, client_message[3000];
	char portc[6];

	struct Node {
		int data;
		struct Node *next;
	};

	struct Node *start = NULL;

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
		int i = 0;

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

	void rosnaco(struct Node *start) {
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

		char str2[10];

		while (current != NULL) {

			sprintf(str2, "%d, ", current->data);
			write(new_socket, str2, strlen(str2));

			//printf("%d\n", current->data);
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

	void push(struct Node **start, int val) {
		struct Node * current = start;

		struct Node *ptr1 = (struct Node*) malloc(sizeof(struct Node));
		ptr1->data = val;
		ptr1->next = *start;
		*start = ptr1;

	}

	puts("\nWprowadz adres Port\n");
	fgets(portc, sizeof(portc), stdin);
	int port = atoi(portc);

	//Utworz gniazdo
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
	}

	//Przygotuj addres gniazda
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//Polacz deskrypor gniazda (gniazdo) z przypisanym adresem
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		puts("Problem dolaczenia gniazda");
	}
	puts("Dolaczono gniazdo strumieniowe");

	//Nasluchiwanie na gniezdzie (serwer)
	listen(socket_desc, 3);

	//Akceptuj polaczenie przychodzace
	puts("SERWER TCP: Czekam na polaczenie...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *) &client,
			(socklen_t*) &c);
	if (new_socket < 0) {
		perror("Problem z akceptacja poloczenia");
	}

	puts("Polaczenie zaakceptowane");

	//Odpowiedz do klienta
	message = "Witam, poloczeyles sie z serwerem TCP. Czekam na dane...\n";
	write(new_socket, message, strlen(message));
#if 1
	while (1) {
		while ((read_size = recv(new_socket, client_message, 3000, 0)) > 0) {
			puts("SERWER: Dane odebrano");
			puts(client_message);

			if (client_message[0] == '/') {
				puts("jestem w komendy");

				if (client_message[1] == 'd') {
					dodawanie(start);

				} else if (client_message[1] == 'r') {

					rosnaco(start);
					print_list(start);

				} else if (client_message[1] == 'o') {
					odejmowanie(start);

				} else if (client_message[1] == 'm') {
					malejaco(start);
					print_list(start);

				} else {

				}

				//	message = "Info od serwera: odebralem dane";
				//Wyslij echo
				//write(new_socket, message, strlen(message));
			} else {
				push(&start, atoi(client_message));
				//wsk = dodaj(atoi(client_message), wsk);

			}
		}
	}
#endif

	return 0;
}
