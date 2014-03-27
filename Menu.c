#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
	
	//Variables que almacenarán los datos del contacto para enviarle mensajes
	char conversacionNombre[100];
	char conversacionIP[100];
	int conversacionPuerto;
	
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
					printf("Ingrese el nombre del contacto a buscar: ");
					scanf("%s",busquedaNombre); //Obtener el nombre del contacto a buscar
					while (getchar()!='\n');
					printf("busquedaNombre = %s\n",busquedaNombre);
					
					while (numero < numContacto){ //Analizar contactos
						if (strncmp(contactos[numero].nombre,busquedaNombre,100) == 0){
							encontrado = 0;
							break;
						}
						numero++;
					}
					if (encontrado == 0){ //Contacto encontrado
						strncpy(conversacionNombre,contactos[numero].nombre,100);
						strncpy(conversacionIP,contactos[numero].ip,100);
						conversacionPuerto = contactos[numero].puerto;

						printf(Azul "Conversación con: %s" ResetColor,contactos[numero].nombre);
						//Chat
					}
					else
						printf(Rojo "El contacto no existe." ResetColor "\n");
					numero = 0;
					encontrado = 1;
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
	printf("\nHasta la próxima.");
	//mandarlo a limpiar sockets con close() (y otras conexiones, si las hay) como dijo el profe "atexit"
	return 0;
}
