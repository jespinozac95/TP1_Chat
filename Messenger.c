/*________________________________________________________________________
					
					Instituto Tecnológico de Costa Rica
					     Lenguajes de Programación	
					     Primera Tarea Programada 
					            Chat en C
					
					Realizado por: 
					        * Josué Espinoza Castro 
							* Mauricio Gamboa Cubero
							* Andrés Pacheco Quesada

					Abril del 2014
__________________________________________________________________________*/



	//----------------------------------------------------------//
	                //Importación de Librerías//
	//----------------------------------------------------------//

#include <sys/types.h>  //Librería utilizada para la creación de sockets y salidas del sistema con exit()
#include <sys/socket.h> //Librería utilizada para la creación de sockets
#include <stdio.h>      //Librería utilizada para las entradas y salidas
#include <unistd.h>     //Librería para el uso de fork()
#include <netinet/in.h> //Librería necesaria para la implementación de la función bind() 
#include <netdb.h>      //Librería utilizada para el manejo de errores
#include <stdlib.h>     //Librería necesaria para las salidas del sistema con exit()
#include <string.h>     //Librería utilizada para el manejo de chars como strings
#include <ctype.h>      //Librería utilizada para el manejo de chars
#include <signal.h>     //Libreria para kill un process

	//----------------------------------------------------------//
	                   //Definición de Macros//
	//----------------------------------------------------------//

//Macro para especificar el tamaño máximo del arreglo del mensaje recibido
#define MAXTAM 256 

//Macros para los colores de texto
#define Rojo "\x1b[31m"
#define Verde "\x1b[32m"
#define Amarillo "\x1b[33m"
#define Azul "\x1b[34m"
#define Morado "\x1b[35m"
#define Celeste "\x1b[36m"
#define ResetColor "\x1b[0m"

	//----------------------------------------------------------//
	            //Definición de Estructuras a Utilizar//
	//----------------------------------------------------------//
	
//Estructura definida como "contacto"
typedef struct{
	char nombre[100];
	char ip[100];
	int puerto;
} contacto;

	//----------------------------------------------------------//
	              //Creación de Variables Globales//
	//----------------------------------------------------------//

//Variable de tipo contacto array para almacenar un máximo de 100 contactos para el chat
contacto contactos[100];

//Variables que almacenarán información de los sockets del servidor para cerrarlos al final del programa
int sock, sock_conectado; 


	//----------------------------------------------------------//
	                       //Funciones//
	//----------------------------------------------------------//
	
//Funcion: recibir mensajes (1 a la vez)
//Entradas: int del puerto que utilizará el socket del servidor y un char pointer al nombre del contacto a establecer conexión con.
//Restricciones: -
//Salidas: - (impresiones en pantalla)
void servidor(int puertoServidor, char *nombre[])
{

	int i, salidaCliente=1; //i: para limpiar el buffer con un loop. salidaCliente: condición para terminar conversación	
	int estado=0; //estado: para saber si las operaciones de bind y listen se hicieron exitosamente
	char buffer1[MAXTAM]="";

	struct sockaddr_in infoSocket; //estructura para almacenar información del socket del servidor
	struct sockaddr_in infoClient; //estructura para manejar información del socket cliente

	if ((sock=socket(PF_INET,SOCK_STREAM,0))==-1){ //crear el socket y almacenar información en sock (variable global)
		perror("Error en sockets");
		exit(0);
		}
	
	infoSocket.sin_family=AF_INET; //almacenar información del protocolo ARPA (Host name -> Internet address)
	infoSocket.sin_port=htons(puertoServidor); //asignar el puerto al socket mediante conversión htons
	infoSocket.sin_addr.s_addr=INADDR_ANY; //para aceptar cualquier conexión

	bzero(&(infoSocket.sin_zero),8); //limpiar la información del socket entrante
	 
	estado=bind(sock,(struct sockaddr *)&infoSocket, sizeof(struct sockaddr_in)); //unión del socket entrante
		
	if (estado==-1){ //validación de error
	 perror("Error bind");
	 exit(0);
	}
	
	estado=listen(sock,5); //escuchar al cliente

	if (estado==-1){ //validación de error
			perror("Error listen");
			exit(0);
	}
			
	for(;;){ //iteracion para estar escuchando y escuchando
		
		socklen_t tamEst; //variable de dirección de tipo socket
		tamEst=sizeof(struct sockaddr_in); //le asigna a la variable el largo de la dirección del socket

		if ((sock_conectado=accept(sock,(struct sockaddr *)&infoClient,&tamEst))==-1){ //establecer y aceptar conexión entrante
			perror("accept");
			exit(-1);
		}
		
		estado = read(sock_conectado, buffer1, sizeof(buffer1)); //leer mensaje del cliente 
		salidaCliente = strcmp(buffer1,"salir\n"); //comparar lo leído con el mensaje de salida

		if (salidaCliente==0){ //El cliente terminó la conversación
			printf("Se terminó la conversacion con %s.\n",nombre);
		}

		else{ //El cliente envió un mensaje
			printf(Rojo "%s: " ResetColor "%s\n",nombre,buffer1);
			for(i=0;i<=255;i++) buffer1[i]=0; //limpiar el buffer1
		}
	}
}

//Funcion: enviar mensajes (1 a la vez)
//Entradas: char pointer al IP del contacto que se lee en un arreglo y int del puerto que utilizará el socket del servidor.
//Restricciones: -
//Salidas: - (impresiones en pantalla)
int cliente (char *IP[],int puerto){
		sleep(3); //esperar 3 segundos para que el servidor de la otra máquina haya comenzado
		int sockCliente; //variable para la asignación del socket
		char buffer[MAXTAM]; //donde se almacenará el mensaje
		int estado; //variable para validación de errores
		struct hostent *he; //para interpretar el IP de la máquina del contacto
		struct sockaddr_in servidor; //permite trabajar con funciones que involucran direcciones de internet
		int i, salidaCliente=1;	 //variables para iterar al limpiar el buffer y validar el fin de la conversación
	
		if ((he=gethostbyname(IP))==NULL){ //Obtiene la dirección IP de acuerdo a lo ingresado por el usuario
			printf("IP no accedible.\n");
			exit(0);
		}
	
		if((sockCliente=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){ //Crea el socket 
			printf("Error al crear el socket.\n");
			exit(-1);
		}
		
		servidor.sin_family=AF_INET; //usar el protocolo ARPA
		servidor.sin_port=htons(puerto); //asignar el puerto para el servidor
		servidor.sin_addr=*((struct in_addr *)he->h_addr); //asignarle la dirección del socket como el IP del contacto
		bzero(&(servidor.sin_zero),8); //restaurar información
		
		estado=connect(sockCliente, (struct sockaddr *)&servidor, sizeof(servidor)); //conectarse al servidor

	if (estado==-1){ //validación de la conexión
			perror("Error connect()");
			exit(-1);
		}
	
	fgets(buffer,256,stdin); //obtener el mensaje del cliente
	salidaCliente=strcmp(buffer,"salir\n"); //comparar el mensaje obtenido con el mensaje de salida
	
	if(salidaCliente==0){ //Salir
		send(sockCliente,buffer,256,0); //enviarle el mensaje de salida al servidor para terminar la conversación
		printf("Se terminó la conversación.\n");
		close(sockCliente); //cerrar el socket
		return 1;
	}
    else{ //Enviar un mensaje válido
		printf(Celeste "Yo: " ResetColor "%s\n",buffer); //imprimir en pantalla el mensaje que manda
		send(sockCliente,buffer,256,0); //enviarle el mensaje válido al servidor
		for(i=0;i<=255;i++) buffer[i]=0; //limpiar buffer
		close(sockCliente); //cerrar el socket
		return 0;
	}
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
	
	//Impresiones de inicio/bienvenida
	
	printf("Bienvenido al ");
	printf(Rojo "Chat" ResetColor);
	printf(Verde "Más" ResetColor);
	printf(Amarillo "Genial" ResetColor);
	printf(Verde "Del" ResetColor);
	printf(Rojo "Mundo\n" ResetColor "\n");
	
//----------------------------------------------------------//
//--------Verificación del archivo de configuración---------//
//----------------------------------------------------------//
	
	int PuertoServidor; //variable local para saber el puerto con el que escuchará el servidor
	FILE *configuracionR; //variable de tipo file pointer para leer el archivo
	configuracionR = fopen("Configuracion.txt","r"); //abrir el .txt de Contactos
	
	if (configuracionR == NULL){ //Si el archivo no existe, lo crea
		printf("Como esta es su primera vez usando el programa, necesitamos que ingrese \nel número del puerto de su computador donde desea escuchar los mensajes.\n");
		printf("Digite el número del puerto de escucha a continuación: \n(numero entre 1024 y 9999) \n\n");
		printf("Ingrese el Puerto de escucha a utilizar (Número y No ingrese espacios): ");
		
		//Obtener el Puerto del usuario
		scanf("%i",&PuertoServidor);
		while (getchar()!='\n'); //limpiar el input del usuario
		
		//Ciclo de validación para el puerto
		while ((PuertoServidor < 1024) || (PuertoServidor > 9999)){
			printf("Ingrese el Puerto (No ingrese espacios y num entre 1024 y 9999): ");
			scanf("%i",&PuertoServidor);
			while (getchar()!='\n');
		}
		
		//Crear y escribir en el archivo
		FILE *configuracionW; //escribir
		configuracionW = fopen("Configuracion.txt","w+"); //crea el archivo
		fprintf(configuracionW,"%i",PuertoServidor); //escribe el puerto ingresado por el usuario en el archivo
		fclose(configuracionW); //cierra el manejador de archivos
		
		//Mensaje de éxito
		printf("\nGracias! Se ha creado también un archivo 'Configuracion.txt' que almacena este dato para próximos usos del programa.\n");
		printf(Amarillo "\nPara poder correr bien el programa, debe reiniciarlo por cuestiones de tiempos de ejecución." ResetColor "\n");
		return 0;
	}
	else{
		//Leer el archivo Configuracion.txt 
		fscanf(configuracionR,"%i",&PuertoServidor);
	}
	fclose(configuracionR); //cerrar el manejador de archivos
	
//----------------------------------------------------------//
//------------Leer los contactos y almacenarlos------------ //
//----------------------------------------------------------//	
	
	FILE *archivo; //definicion de una variable puntero de tipo de FILE para abrir el archivo deseado
	archivo = fopen("Contactos.txt","r"); //abrir el .txt de Contactos
	
	if (archivo == NULL){ //si el archivo no existe, lo crea
		FILE *crear;
		crear = fopen("Contactos.txt","w+"); //crear el archivo 'Contactos.txt'
		fclose(crear);
		
		printf("No se encontró el archivo 'Contactos.txt', se ha creado un archivo de texto en blanco con ese nombre.\n");
		printf("Este archivo es necesario para el funcionamiento del programa.\n");
		printf("Si tiene el archivo pero no está en la carpeta donde está este archivo, consígalo y actualice la información en un solo archivo 'Contactos.txt'.\n\n");
		printf(Amarillo "\nPara poder correr bien el programa, debe reiniciarlo por cuestiones de tiempos de ejecución." ResetColor "\n");
		return 0;
	}
	
	int numContacto = 0; //numero de contacto actual en el arreglo
	
	//Variables para ir llenando cada contacto, de tamaño 100 porque así se almacenará palabra por palabra
	char leerNombre[100]; //nombre
	char leerIp[100]; //ip
	int leerPuerto; //puerto
	
	//Ciclo para leer 3 palabras (nombre, ip y puerto) cada vez
	while ((fscanf(archivo,"%s%s%i",leerNombre,leerIp,&leerPuerto) == 3) && (numContacto<100)) { //mientras se pueda
		
		strncpy(contactos[numContacto].nombre,leerNombre,100); //copiar el contenido al contacto
		strncpy(contactos[numContacto].ip,leerIp,100); //copiar el contenido al contacto
		contactos[numContacto].puerto = leerPuerto; //asignar el contenido al contacto

		numContacto++;
	}
	fclose(archivo); //cerrar el archivo de lectura
//----------------------------------------------------------//
//--------------------Menú de opciones----------------------//
//----------------------------------------------------------//
	
	//Impresiones del menú
	
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
   
    int salir; //Variable para salir de la conversacion
    
	int encontrado = 1; //Variable para saber si se encontró el contacto requerido
	
	pid_t pid; //Variable para el fork
	
	int servidorEnUso = 0; //Variable para no volver a ejecutar el servidor en el fork
	int clienteEnUso = 0; //Variable para no volver a ejecutar el cliente en el fork
	
	//Ciclo del menu --> para que vuelva al menu despues de usar una funcionalidad
	while (opcion != 0){
		switch(opcion){
			
			//-----------------------------------------------------------------
								//---------------------------------------------
				case 1: 		//      ---> Agregar un contacto <---
								//---------------------------------------------
			//-----------------------------------------------------------------
			
				if (numContacto == 100){ //llegó al límite de cantidad de contactos
					printf(Rojo "Error, ha llegado al límite de cantidad de contactos.\n" ResetColor "\n");
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
				printf("Se ingresó  --> CONTACTO # %d\n",numContacto); //impresion de confirmación
				imprimirContacto(&contactos[numContacto]); //impresion de confirmación
				numContacto++;
				
				//Escribir el nuevo contacto en Contactos.txt
				FILE *file;
				file= fopen("Contactos.txt","a"); //a para append,
				fprintf(file,"%s %s %i\n",inputNombre,inputIP,inputPuerto);
				fclose(file);
				
				printf(Verde "Se ha añadido el contacto correctamente.\n" ResetColor "\n");
				break;

			//-----------------------------------------------------------------
								//---------------------------------------------
				case 2: 		//      ---> Visualizar contactos <---
								//---------------------------------------------
			//-----------------------------------------------------------------
			
				if (numContacto == 0){ //no hay contactos
					printf(Azul "No hay contactos actualmente.\n" ResetColor);
					break;
				}
				else{
					while (numero < numContacto){ //imprimir todos los contactos existentes
						printf("------------CONTACTO # %d\n",numero);
						imprimirContacto(&contactos[numero]);
						numero++;
					}
				}
				numero = 0; //Reiniciar valor de numero para proximas impresiones
				break;
				
			//-----------------------------------------------------------------
								//---------------------------------------------
				case 3: 		//      ---> Comenzar el chat <---
								//---------------------------------------------
			//-----------------------------------------------------------------
			
				if (numContacto == 0){ //no tiene contactos para enviarle
					printf(Azul "Usted no puede enviar mensajes porque no tiene contactos, agregue alguno.\n" ResetColor);
				}
				else {
					//Busca un contacto para comenzar la conversacion
					printf("Ingrese el nombre del contacto a mensajear: ");
					scanf("%s",busquedaNombre); //Obtener el nombre del contacto a buscar
					while (getchar()!='\n');
						
					while (numero < numContacto){ //Analizar contactos
						if (strncmp(contactos[numero].nombre,busquedaNombre,100) == 0){
							encontrado = 0;
							break;
						}
						numero++;
					}
					
					if (encontrado == 0){ //Contacto encontrado
						printf(Verde "\nContacto Válido: %s" ResetColor "\n",contactos[numero].nombre);

						pid = fork(); //bifurcación del proceso en 2. 
									  //desde aquí en adelante se ejecutará el programa desde los if y else if siguientes
									  //un proceso en cada condición
									  
						if (pid > 0){ //recibir (proceso padre)
							while (salir != 1){ //en realidad, nunca sale 
								servidor(PuertoServidor,contactos[numero].nombre); //llamada a la función servidor
							}
						}
					
						else if (pid == 0){ //enviar (proceso hijo)
							while (salir != 1){
								salir = cliente(contactos[numero].ip,contactos[numero].puerto); //llamada a la función cliente
								kill(pid,0); //matar el proceso hijo
							}
						}
						
						else{ //hubo un error en el fork (cuando pid = -1)
							perror("Error en el fork.\n");
							salir = 1;
						}
					}
							
					else //No existe el contacto, entonces no puede ejecutar el chat porque servidor ocupa el nombre del contacto
							printf(Rojo "El contacto no existe." ResetColor "\n");
							
					numero = 0; //reset para proximas busquedas
					encontrado = 1; //reset para proximas busquedas
				}
				break;
		}
			
			//Mostrar las opciones del menú de nuevo (loop)
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
	
	close(sock_conectado);	//cerrar el socket del servidor cuando hace una conexión
	close(sock); //cerrar el socket del servidor de escucha
	
	//Mensaje de salida
	printf("\nHasta la próxima.\n");
	printf("Presione CTRL + C para salir de la aplicación.\n");
	return 0;
}
