#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bor-util.h"
#include "bor-timer.h"
#include "nhello-ser2.h"

Clients clis;
int boucle_prime=1;

int dialoguer_avec_un_client(int soc){
	struct sockaddr_un adr_client;
	char buf1[2048],buf2[2048];
	printf("Attente message client ...\n");
	int k=bor_recvfrom_un_str(soc,buf1,sizeof(buf1),&adr_client);
	printf("Reçu %d octets de %s : \"%s\"\n",k,adr_client.sun_path,buf1 );
	//on fabrique la réponse
	time_t tps;
	time(&tps);
	sprintf(buf2,"DATE : %s",ctime(&tps));
	printf("Envoi de la réponse ...\n");
	k=bor_sendto_un_str(soc,buf2,&adr_client);
	if(k<0){
		return -1;
	}
	printf("Envoyé %d octets à %s : \"%s\"\n",k,adr_client.sun_path,buf2 );
	return 1;		//pour tester : commande netcat
}

int dialoguer_avec_un_client_nHello(int soc){
	struct sockaddr_un adr_client;
	char buf1[2048],buf2[2048],buf3[10];
	printf("Attente message client ...\n");
	int k=bor_recvfrom_un_str(soc,buf1,sizeof(buf1),&adr_client);
	printf("Reçu %d octets de %s : \"%s\"\n",k,adr_client.sun_path,buf1 );
	sprintf(buf2,"HELLO");
	sprintf(buf3,"NUMBER");
	int found = 0;
	if(strcmp(buf1,buf2) == 0){
		for(int i = 0; i < clis.size;i++){
			if(strcmp(adr_client.sun_path,clis.clients[i].adr_client.sun_path) == 0){
				clis.clients[i].nbHello++;
				found=1;
			}
		}
		if(found==0){
			clis.size++;
			clis.clients = (Client*)realloc(clis.clients,clis.size);
			clis.clients[clis.size-1].adr_client = adr_client;
			clis.clients[clis.size-1].nbHello++;
		}
	}else if(strcmp(buf1,buf3) == 0){
		//on fabrique la réponse
		for(int i = 0; i < clis.size;i++){
			sprintf(buf2,"Nb Hello : %d",clis.clients[i].nbHello);
			printf("Envoi de la réponse ...\n");
			k=bor_sendto_un_str(soc,buf2,&adr_client);
			if(k<0){
				return -1;
			}
			printf("Envoyé %d octets à %s : \"%s\"\n",k,clis.clients[i].adr_client.sun_path,buf2 );
		}
		boucle_prime=0;
	}
	return 1;		//pour tester : commande netcat
}

void capter_SIGITN(int sig){
	printf("signal %d capté\n", sig);
	boucle_prime=0;
}


int main (int argc,char * argv[]){
	if(argc-1 !=1){
		fprintf(stderr, "usage : %s socket_serveur \n",argv[0] );
		exit(1);
	}
	char * nom_serveur = argv[1];
	bor_signal(SIGINT,capter_SIGITN,0);
	struct sockaddr_un adr_serveur;
	
	int soc = bor_create_socket_un(SOCK_DGRAM,nom_serveur,&adr_serveur);
	if(soc <0){
		exit(1);
	}
	while(boucle_prime){
		int k=dialoguer_avec_un_client_nHello(soc);
		if(k<0){
			break;
		}
	}
	printf("Fermeture socket locale\n");
	close(soc);
	unlink(adr_serveur.sun_path);
	exit(0);
}
