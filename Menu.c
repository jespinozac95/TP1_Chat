#include <sys/types.h> // Librería utilizada para la creación de sockets
#include <sys/socket.h> //Librería utilizada para la creación de sockets
#include <stdio.h> //Librería utilizada para las entradas y salidas
#include <unistd.h>
#include <netinet/in.h> //Librería necesaria para la implementación de la función bind() 
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#define PUERTO 8884 //Macro que establece el puerto a utilizar
#define MAXTAM 256 //Macro para especificar el tamaño máximo del arreglo del mensaje recibido

//Macros para los colores de texto
#define Rojo "\x1b[31m"
#define Verde "\x1b[32m"
#define Amarillo "\x1b[33m"
#define Azul "\x1b[34m"
#define Morado "\x1b[35m"
#define Celeste "\x1b[36m"
#define ResetColor "\x1b[0m"

//Estructura definida como "contacto"
typedef struct{
	char nombre[100];
	char ip[100];
	int puerto;
} contacto;

void servidor(int puerto)
{
int i, salidaServidor = 1, salidaCliente=1;	
int estado=0;
int pid=0;
char buffer[MAXTAM]="";
char buffer1[MAXTAM]="";

struct sockaddr_in infoSocket;

struct sockaddr_in infoClient;

int sock, sock_conectado; 
//tamEst;

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
	
	socklen_t tamEst; 
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
				
				send(sock_conectado, "Bienvenido al servidor. Digite <Salir> para terminar la conversacion. \n",72,0);
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
				printf(Rojo "cliente: " ResetColor "%s\n" ResetColor, buffer1);
				for(i=0;i<=255;i++) buffer1[i]=0;}
				
				
				
				printf(Azul  "SERVIDOR: "  ResetColor);
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

void cliente (char *texto[],int puerto)    //int argc, char *texto[] !IP,Puerto!
	{
		printf("\nPrueba: texto = %s\n",texto);
		int sock; //Número enteros para asignación del socket
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
	servidor.sin_port=htons(puerto);
	//servidor.sin_addr.s_addr=INADDR_ANY; 
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


//Funcion: imprimir los miembros de un contacto
//Entradas: un puntero hacia el contacto a imprimirle los miembros
//Restricciones: -
//Salidas: - (impresiones en pantalla)
void imprimirContacto(contacto *ptr) {
    if (ptr->nombre){
		printf("            Nombre = %s\n",ptr->nombre);
		printf("            IP = %s\n",ptr->ip);
		printf("            Puerto = %i\n\n",ptr->puerto);
	}
}

//Funcion de inicio main()
int main(){
	printf("Bienvenido al ");
	printf(Rojo "Chat" ResetColor);
	printf(Verde "Más" ResetColor);
	printf(Amarillo "Genial" ResetColor);
	printf(Verde "Del" ResetColor);
	printf(Rojo "Mundo\n" ResetColor "\n");
	
	//----------------------------------------------------------//
	         //Verificación del archivo de configuración//
	//----------------------------------------------------------//
	
	int PuertoServidor;
	FILE *configuracionR; //leer
	configuracionR = fopen("Configuracion.txt","r"); //abrir el .txt de Contactos
	
	if (configuracionR == NULL){ //Si el archivo no existe, lo crea
		printf("Como esta es su primera vez usando el programa, necesitamos que ingrese \nel número del puerto de su computador donde desea escuchar los mensajes.\n");
		printf("Digite el número del puerto de escucha a continuación: \n(numero entre 1024 y 9999) \n\n");
		printf("Ingrese el Puerto del contacto a guardar (Número y No ingrese espacios): ");
		
		//Obtener el Puerto del usuario
		scanf("%i",&PuertoServidor);
		while (getchar()!='\n');
		//Ciclo de validación para el puerto
		while ((PuertoServidor < 1024) || (PuertoServidor > 9999)){
			printf("Ingrese el Puerto (No ingrese espacios y num entre 1024 y 9999): ");
			scanf("%i",&PuertoServidor);
			while (getchar()!='\n');
		}
		//Crear y escribir en el archivo
		FILE *configuracionW; //escribir
		configuracionW = fopen("Configuracion.txt","w+");
		fprintf(configuracionW,"%i",PuertoServidor);
		fclose(configuracionW);
		
		printf("\nGracias! Se ha creado también un archivo 'Configuracion.txt' que almacena este dato para próximos usos del programa.\n");
		printf(Amarillo "\nPara poder correr bien el programa, debe reiniciarlo por cuestiones de tiempos de ejecución." ResetColor "\n");
		return 0;
	}
	else{
		//Leer el archivo Configuracion.txt 
		fscanf(configuracionR,"%i",&PuertoServidor);
	}
	fclose(configuracionR);
	
//------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------//
	            //Leer los contactos y almacenarlos//
	//----------------------------------------------------------//	
	
	FILE *archivo; //definicion de una variable puntero de tipo de FILE para abrir el archivo deseado
	archivo = fopen("Contactos.txt","r"); //abrir el .txt de Contactos
	
	if (archivo == NULL){ //si el archivo no existe, para aquí
		FILE *crear;
		crear = fopen("Contactos.txt","w+"); //crear el archivo 'Contactos.txt'
		fclose(crear);
		
		printf("No se encontró el archivo 'Contactos.txt', se ha creado un archivo de texto en blanco con ese nombre.\n");
		printf("Este archivo es necesario para el funcionamiento del programa.\n");
		printf("Si tiene el archivo pero no está en la carpeta donde está este archivo, consígalo y actualice la información en un solo archivo 'Contactos.txt'.\n\n");
		printf(Amarillo "\nPara poder correr bien el programa, debe reiniciarlo por cuestiones de tiempos de ejecución." ResetColor "\n");
		return 0;
	}
	
	contacto contactos[100]; //arreglo de contactos
	int numContacto = 0; //numero de contacto actual en el arreglo
	
	//Variables para ir llenando cada contacto, de tamaño 100 porque así se almacenará palabra por palabra
	char leerNombre[100]; //nombre
	char leerIp[100]; //ip
	int leerPuerto; //puerto
	
	//Ciclo para leer 3 palabras (nombre, ip y puerto) cada vez
	while ((fscanf(archivo,"%s%s%i",leerNombre,leerIp,&leerPuerto) == 3) && (numContacto<100)) {
		//printf("LEYENDO: %s %s %i\n",leerNombre,leerIp,leerPuerto); //impresion de prueba
		
		strncpy(contactos[numContacto].nombre,leerNombre,100);
		strncpy(contactos[numContacto].ip,leerIp,100);
		contactos[numContacto].puerto = leerPuerto;
		
		//printf("PRUEBA --> CONTACTO # %d\n",numContacto); //impresion de prueba
		//imprimirContacto(&contactos[numContacto]); //impresion de prueba

		numContacto++;
	}
	fclose(archivo); //cerrar el archivo de lectura
//------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------//
	                     //Menú de opciones//
	//----------------------------------------------------------//
	
	printf("*****Menú Principal*****\n\nSeleccione una de las siguientes opciones:\n");
	printf(Verde "1- Agregar contactos" ResetColor "\n");
	printf(Amarillo "2- Visualizar contactos" ResetColor "\n");
	printf(Rojo "3- Abrir el Chat" ResetColor "\n");
	printf("Presione 0 para salir de la aplicación.\n");
	
	int opcion; //donde se almacenará el input de opción del usuario
	scanf("%d",&opcion); //scanear el input del usuario
	while ((opcion < 0) || (opcion > 3)){ //restricciones
		printf("Ingrese un número de opción correcto: ");
		while (getchar()!='\n');  //limpiar el registro de standard input hasta la nueva línea (soluciona que "ccc" no vaya a entrar al while 3 veces, solo 1)
		scanf("%d",&opcion); //volver a scanear hasta llegar a un valor válido
	}
	
	//Variables para ingresar los campos del nuevo contacto
	char inputNombre[100];
	char inputIP[100];
	int inputPuerto;
	
	int numero = 0; //Variable para iterar en el arreglo de contactos
	char busquedaNombre[100]; //Variable para buscar un contacto en el arreglo de contactos
	
	int encontrado = 1; //Variable para saber si se encontró el contacto requerido
	
	//Ciclo del menu --> para que vuelva al menu despues de usar una funcionalidad
	while (opcion != 0){
		switch(opcion){
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			case 1: //Agregar un contacto
				if (numContacto == 100){
					printf(Rojo "Error, ha llegado al limite de cantidad de contactos.\n" ResetColor "\n");
					break;
				}
				
				//Obtener el nombre desde stdin
				printf("Ingrese el Nombre del contacto a guardar (No ingrese espacios): ");
				scanf("%s",inputNombre);
				while (getchar()!='\n');
				
				//Obtener el ip desde stdin
				printf("Ingrese el IP del contacto a guardar (No ingrese espacios): ");
				scanf("%s",inputIP);
				while (getchar()!='\n');
				int i,puntos = 0; //Variables para el ciclo de restriccion
				//Ciclo de restriccion: deben haber al menos 3 puntos en la direccion IP
				while (puntos < 3){
					for (i = 0;inputIP[i] != '\0'; i++){ //Itera sobre cada campo del arreglo de chars mientras no sean nulos
						if (inputIP[i] == '.') //Busca los puntos y los agrega a la variable
							puntos++;
					}
					if (puntos < 3){ //Volver a preguntar el IP
						printf("Ingrese el IP del contacto a guardar (Al menos 3 puntos entre los números): ");
						scanf("%s",inputIP);
						while (getchar()!='\n');
						puntos = 0;
					}
				}
				
				//Obtener el puerto desde stdin
				printf("Ingrese el Puerto del contacto a guardar (Número y No ingrese espacios): ");
				scanf("%i",&inputPuerto);
				while (getchar()!='\n');
				//Ciclo de validación para el puerto
				while ((inputPuerto < 1024) || (inputPuerto > 9999)){
					printf("Ingrese el Puerto del contacto a guardar (No ingrese espacios y num entre 1024 y 9999): ");
					scanf("%i",&inputPuerto);
					while (getchar()!='\n');
				}
				
				//Agregar el nuevo contacto al array de contactos
				strncpy(contactos[numContacto].nombre,inputNombre,100);
				strncpy(contactos[numContacto].ip,inputIP,100);
				contactos[numContacto].puerto = inputPuerto;
				printf("Se ingresó  --> CONTACTO # %d\n",numContacto); //impresion de prueba
				imprimirContacto(&contactos[numContacto]); //impresion de prueba
				numContacto++;
				
				//Escribir el nuevo contacto en Contactos.txt
				FILE *file;
				file= fopen("Contactos.txt","a"); //a para append,
				fprintf(file,"%s %s %i\n",inputNombre,inputIP,inputPuerto);
				fclose(file);
				
				printf(Verde "Se ha añadido el contacto correctamente.\n" ResetColor "\n");
				break;
				
			//////////////////////////////////////////////////////////////////////////////////////////////////////	
			//////////////////////////////////////////////////////////////////////////////////////////////////////
			case 2: //Visualizar contactos
				if (numContacto == 0){
					printf(Azul "No hay contactos actualmente.\n" ResetColor);
					break;
				}
				else{
					while (numero < numContacto){
						//printf("numero = %d,numContacto = %i\n",numero,numContacto);
						printf("------------CONTACTO # %d\n",numero);
						imprimirContacto(&contactos[numero]);
						numero++;
					}
				}
				numero = 0; //Reiniciar valor de numero para proximas impresiones
				break;
	        //////////////////////////////////////////////////////////////////////////////////////////////////////
	        //////////////////////////////////////////////////////////////////////////////////////////////////////
			case 3: //Comenzar un chat
				if (numContacto == 0){
					printf(Azul "Usted no puede enviar mensajes porque no tiene contactos, agregue alguno.\n" ResetColor);
				}
				else {
					
			
						//COMIENZA EL CHAT
						
					
						printf("Escriba 1 para enviar mensaje (cliente), 2 para conectarse al chat (servidor): ");				
						opcion = 0; //donde se almacenará el input de opción del usuario
						scanf("%d",&opcion); //scanear el input del usuario
						while ((opcion != 1) && (opcion != 2)){ //restricciones
							printf("Escriba 1 para enviar mensaje (cliente), 2 para conectarse al chat (servidor): ");
							while (getchar()!='\n');  //limpiar el registro de standard input hasta la nueva línea (soluciona que "ccc" no vaya a entrar al while 3 veces, solo 1)
							scanf("%d",&opcion); //volver a scanear hasta llegar a un valor válido
						}
	
						if (opcion == 1){ //cliente
							printf("Ingrese el nombre del contacto a mensajear: ");
							scanf("%s",busquedaNombre); //Obtener el nombre del contacto a buscar
							while (getchar()!='\n');
							//printf("busquedaNombre = %s\n",busquedaNombre);
							
							while (numero < numContacto){ //Analizar contactos
								if (strncmp(contactos[numero].nombre,busquedaNombre,100) == 0){
									encontrado = 0;
									break;
								}
								numero++;
							}
							if (encontrado == 0){ //Contacto encontrado
								printf(Verde "\nConversación válida con: %s \n" ResetColor,contactos[numero].nombre);
								cliente(contactos[numero].ip,contactos[numero].puerto); 
							}						
							else //No existe el contacto
								printf(Rojo "El contacto no existe." ResetColor "\n");
								
							numero = 0; //reset para proximas busquedas
							encontrado = 1; //reset para proximas busquedas
						}
						else  //servidor
							servidor(PuertoServidor); 
					}
				break;
		}
		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------
			
			//Mostrar las opciones del menu
			printf(Verde "\n1- Agregar contactos" ResetColor "\n");
			printf(Amarillo "2- Visualizar contactos" ResetColor "\n");
			printf(Rojo "3- Abrir el Chat" ResetColor "\n");
			printf("Presione 0 para salir de la aplicación.\n");
			
			//Volver a leer la input de opcion
			scanf("%d",&opcion); //scanear el input del usuario
			while ((opcion < 0) || (opcion > 4)){ //restricciones
				printf("Ingrese un número de opción correcto: ");
				while (getchar()!='\n');  //limpiar el registro de standard input hasta la nueva línea (soluciona que "ccc" no vaya a entrar al while 3 veces, solo 1)
				scanf("%d",&opcion); //volver a scanear hasta llegar a un valor válido
			}
	}
	printf("\nHasta la próxima.\n");
	//mandarlo a limpiar sockets con close() (y otras conexiones, si las hay) como dijo el profe "atexit"
	return 0;
}
