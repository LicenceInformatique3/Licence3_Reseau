#ifndef NHELLO_SER2__H
#define NHELLO_SER2__H

typedef struct Client{
	int nbHello;
	struct sockaddr_un adr_client;
}Client;

typedef struct Clients{
	Client * clients;
	int size;
}Clients;

#endif /* NHELLO_SER2__H */
