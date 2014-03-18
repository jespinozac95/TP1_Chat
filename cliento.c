#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

#define PUERTO 3550

#define MAXTAM 100

int main (int argc, char *texto[])
	{
		int sock, nbytes;
		char buffer[MAXTAM];
		struct hostent *he;
		struct sockaddr_in servidor;
	
	if (argc !=2){
		printf("Escriba: %s <Dirección IP>\n",texto[0]);
		printf(texto[0]);
		exit(-1);
	}
	
	if ((he=gethostbyname(texto[1]))==NULL){
		printf("gethostbyname() error\n");
		exit(0);
		}
	
	if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){
		printf("socket() error\n");
		exit(-1);
	}
		
	servidor.sin_family=AF_INET;
	servidor.sin_port=htons(PUERTO);
	servidor.sin_addr=*((struct in_addr *)he->h_addr);
	bzero(&(servidor.sin_zero),8);
	
	
	/*if (inet_aton(he,&servidor.sin_addr)==0)
		{
			perror("Error inet_aton");
			exit(0);
		}*/
		
	
	if (connect(sock, (struct sockaddr *)&servidor, sizeof(struct sockaddr_in))<0)
		{	
			perror("Error connect()");
			exit(0);
		}
		
	if ((nbytes=recv(sock,buffer,MAXTAM,IPPROTO_TCP))==-1){
		printf("Error en recv");
		exit(0);
		}
	buffer[nbytes]='\0';
	
	printf("Mensaje del servidor: %s\n",buffer);
	
	close(sock);
	

		
	/*if ((sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
		printf("Error socket");
		exit(-1);
	}*/
	
		

}
