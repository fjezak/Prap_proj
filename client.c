#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in server;
	char dzialanie[1000], wynik[2000];
	char adress[] = "127.0.0.1";
	char portc[] = "8888";
	char liczbac[60];
	char komenda[30];
	int flaga = 1;
	int flaga2 = 1;

	// puts("Wprowadz adres IP Serwera\n");
	// fgets( adress, sizeof(adress),stdin);

	// puts("\nWprowadz adres Port\n");
	// fgets( portc, sizeof(portc),stdin);

	printf("adress = %s\n", adress);
//printf("adres serwera: %s ",adress);
	int port = atoi(portc);
	printf("port: %d\n", port);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("Nie można utworzyć gniazda");
	}
	puts("Gniazd utworzone");

	server.sin_addr.s_addr = inet_addr(adress);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("Brak połączenia");
		return 1;
	}

	puts("Połączono\n");

	while (flaga2) {

		flaga = 1;
		printf("wpisz pierwszą liczbę \n");
		fgets(liczbac, sizeof(liczbac), stdin);
		send(sock, liczbac, strlen(liczbac), 0);

		while (flaga) {
			printf("wpisz kolejną liczbę \n ");
			fgets(liczbac, sizeof(liczbac), stdin);

			if (liczbac[0] == 'N') {
				flaga = 0;
			} else {
				send(sock, liczbac, strlen(liczbac), 0);
			}

		}

		printf(
				"Co chcesz treraz robić: /dodawanie , /odejmowanie, /rosnąco /malejąco  ");
		fgets(dzialanie, sizeof(dzialanie), stdin);
		puts(dzialanie);

		if (send(sock, dzialanie, strlen(dzialanie), 0) < 0) {
			puts("Wysyłanie nie udane");

			return 1;
		}

		if (recv(sock, wynik, 2000, 0) < 0) {
			puts("Odbieranie nie udane");
			break;
		}

		puts("WYNIK :");
		puts(wynik);
		memset(wynik, 0, strlen(wynik));

		printf("Jeszcze raz?? T/N \n");
		fgets(komenda, sizeof(komenda), stdin);
		if (komenda[0] == 'N')
			flaga2 = 0;

	}
	printf("KONIEC\n");

	close(sock);
	return 0;
}
