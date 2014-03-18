#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>

#define puerto 3550

main()
{
//int socket1;
struct sockaddr_in infoSocket;

struct sockaddr_in infoClient;

int sock, sock_conectado, tamEst;

if ((sock=socket(PF_INET,SOCK_STREAM,0))==-1)
	{
	perror("Error en sockets");
	exit(0);
	}
/*struct in_addr
	{
	unsigned long int s_addr;
	};
	struct sockaddr_in
	{
	int sin_family;
	unsigned short int sin_port;
	struct in_addr sin_addr;
	};*/

infoSocket.sin_family=AF_INET;
infoSocket.sin_port=htons(puerto);
infoSocket.sin_addr.s_addr=INADDR_ANY;

bzero(&(infoSocket.sin_zero),8);
//int bind(int socket1,struct sockaddr_in *addr, int adddrlen); 
/* adddr: puntero a la estructura sockaddr_in convertida a sockaddr 
 * addrlen: tamaño de la estructura apuntada por el puntero addr
 * devuelve:
 * 			0 si sucede correctamente
 * 			<0 si existe error*/
 if (bind(sock,(struct sockaddr *)&infoSocket, sizeof(struct sockaddr_in))!=0)
	{
	 perror("Error bind");
	 exit(0);
	}

if (listen(sock,5)!=0)
	{
		perror("Error listen");
		exit(0);
	}
while(1){
	tamEst=sizeof(struct sockaddr_in);
	if ((sock_conectado=accept(sock,(struct sockaddr *)&infoClient,&tamEst))==-1)
		{
			perror("accept");
			exit(-1);
		}
		
printf("Se obtuvo informacion desde %s\n", inet_ntoa(infoClient.sin_addr));
 
	send(sock_conectado,"Bienvenido al servidor.\n",35,0);
	close(sock_conectado);	
	}
}
