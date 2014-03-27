#include <sys/types.h> // Librería utilizada para la creación de sockets
#include <sys/socket.h> //Librería utilizada para la creación de sockets
#include <stdio.h> //Librería utilizada para las entradas y salidas
#include <unistd.h>
#include <netinet/in.h> //Librería necesaria para la implementación de la función bind() 
#include <netdb.h>

#define PUERTO 8884 //Macro que establece el puerto a utilizar

#define MAXTAM 256 //Macro para especificar el tamaño máximo del arreglo del mensaje recibido


#define puerto 8884
#define MAXTAM 256
//~ #include <sys/types.h>
//~ #include <sys/socket.h>
//~ #include <stdio.h>
//~ #include <unistd.h>
//~ #include <netinet/in.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

servidor()
{
int i, salidaServidor = 1, salidaCliente=1;	
int estado=0;
int pid=0;
char buffer[MAXTAM]="";
char buffer1[MAXTAM]="";
char buffer2[MAXTAM]=""; 

struct sockaddr_in infoSocket;

struct sockaddr_in infoClient;

int sock, sock_conectado, tamEst;

if ((sock=socket(PF_INET,SOCK_STREAM,0))==-1)
	{
	perror("Error en sockets");
	exit(0);
	}


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
 estado=bind(sock,(struct sockaddr *)&infoSocket, sizeof(struct sockaddr_in));
 if (estado==-1)
	{
	 perror("Error bind");
	 exit(0);
	}
estado=listen(sock,5);

if (estado==-1)
	{
		perror("Error listen");
		exit(0);
	}
for(;;)

{
	tamEst=sizeof(struct sockaddr_in);
	if ((sock_conectado=accept(sock,(struct sockaddr *)&infoClient,&tamEst))==-1)
		{
			perror("accept");
			exit(-1);
		}
	
	pid=fork();
	
	switch(pid)
	{
		case -1:
			perror("Error en fork()");
			exit(1);
		case 0:
			//close(sock);
			
			//if (sock_conectado=accept(sock, (struct sockaddr *) &infoClient, &tamEst))
			if (-1 == getpeername(sock_conectado, (struct sockaddr *) &infoClient, &tamEst))
			{	
				perror("Error accept()");
				exit(-1);
			}
			else 
			{
				printf("Conexion Establecida. Presione S para salir\n");
				//printf("Se obtuvo informacion desde %d\n",infoClient.sin_addr); 
				
				send(sock_conectado, "Bienvenido al servidor. Digite <Salir> para terminar la conversacion. \n",50,0);
				//buffer[i]='\0';
				
				while ((salidaServidor!=0) && (salidaCliente !=0)){
				
				
				
				estado = read(sock_conectado, buffer1, sizeof(buffer1));
			
				salidaCliente = strcmp(buffer1,"salir\n");

				if (salidaCliente==0){

				printf("cliente: el cliente termino la conversacion");
				close(sock_conectado);
				break; 
				}
				else{
				printf(ANSI_COLOR_RED "cliente: " ANSI_COLOR_RESET "%s\n" ANSI_COLOR_RESET, buffer1);
				for(i=0;i<=255;i++) buffer1[i]=0;}
				
				
				
				printf(ANSI_COLOR_BLUE  "SERVIDOR: "  ANSI_COLOR_RESET);
				fgets(buffer,2560,stdin);
				salidaServidor = strcmp(buffer,"salir\n");
				if (salidaServidor==0){
				printf("servidor: se termino la conversacion");
				send(sock_conectado,"salir\n",256,0);
				close(sock_conectado);
				break; }
				else{
				send(sock_conectado,buffer,256,0);
				for(i=0;i<=255;i++) buffer[i]=0;
			}
			}
			}
		
			
		
	close(sock_conectado);	

}
}
}


int cliente (char *texto[])    //int argc, char *texto[]
	{
		int sock, nbytes; //Números enteros para asignación del socket y variable de asignación de la función recv
		char buffer[MAXTAM]; //Donde se almacenará el mensaje
		char buffer1[MAXTAM];
		int estado;
		struct hostent *he; 
		struct sockaddr_in servidor; //Permite trabajar con funciones que involucran direcciones de internet
		int i, salidaServidor = 1, salidaCliente=1;	
	

	
	
	if ((he=gethostbyname(texto))==NULL){ //Obtiene la dirección IP de acuerdo a lo ingresado por el usuario    texto[0]
		printf("gethostbyname() error\n");
		exit(0);
		}
	
	if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){ //Crea el socket 
		printf("socket() error\n");
		exit(-1);
	}
		
	servidor.sin_family=AF_INET;
	servidor.sin_port=htons(PUERTO);
	servidor.sin_addr=*((struct in_addr *)he->h_addr);
	bzero(&(servidor.sin_zero),8);
	
	

		
	estado=connect(sock, (struct sockaddr *)&servidor, sizeof(servidor));
	if (estado==-1) //Condición que solicita el valor
		{	
			perror("Error connect()");
			exit(-1);
		}
		
		
	while((salidaServidor!=0) && (salidaCliente !=0))
	
				{
			
					estado= read(sock,buffer,sizeof(buffer));
					salidaServidor = strcmp(buffer,"salir\n");
					
					if (salidaServidor==0){
					send(sock,buffer,256,0);
					printf("servidor: se termino la conversacion");
					close(sock);
					break; 
					}
					else {
					printf("servidor: %s",buffer);
					for(i=0;i<=255;i++) buffer[i]=0;
					}
					
					//bzero((char*)&buffer,sizeof(buffer));
					
					printf("CLIENTE: ");
					fgets(buffer1,256,stdin);
					salidaCliente=strcmp(buffer1,"salir\n");
					if(salidaCliente==0){
					send(sock,buffer1,256,0);
					close(sock);
				    }
				    else {
					send(sock,buffer1,256,0);
					for(i=0;i<=255;i++) buffer1[i]=0;}
				}
			


	close(sock);
}









main(){
	
	char buffermain[MAXTAM]=""; 
	printf("Escriba 1 para enviar mensaje (cliente), 2 para conectarse al chat (servidor)");
	fgets(buffermain,256,stdin);
	if (strcmp(buffermain,"1\n")){
		servidor(); 
	}
	else {
		char buffer6[MAXTAM]=""; 
		printf("Escriba la direccion IP del contacto: \n");
	    scanf("%s",&buffer6); 
	    cliente(buffer6); 
		
	}
	
}
