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
	char* nombre;
	char* ip;
	int puerto;
} contacto;

//Funcion: validar inputs del usuario cuando se vaya a añadir un contacto
//Entradas: un char pointer hacia la palabra obtenida a través del stdin
//Restricciones: -
//Salidas: -
/*int validarIP(char *ip[]){
	int i, puntos, resultado; //if resultado == 1 volver a pedir el valor, resultado == 0 todo bien
	printf("Entra a validarIP = ");
	for (i = 0; *(*(ip)[i]) != '\0'; i++){
		if (*(*(ip)[i]) == '.')
			puntos++;
	}
	while (*(*ip)[i] != '\0'){
		if (*(*ip)[i] == '.'){
			puntos++;
		}
		i++;
	}
	if (puntos < 3)
		resultado = 1;
	return resultado;
}*/

//Funcion: imprimir los miembros de un contacto
//Entradas: un puntero hacia el contacto a imprimirle los miembros
//Restricciones: -
//Salidas: - (impresiones en pantalla)
void imprimirContacto(contacto* ptr) {
    if (ptr->nombre){
		printf("Nombre = %s\n",ptr->nombre);
		printf("IP = %s\n",ptr->ip);
		printf("Puerto = %i\n\n",ptr->puerto);
	}
}

//Funcion de inicio main()
int main(){
	//Cargar contactos
//------------------------------------------------------------------------------------------------------------------------------------
	FILE *archivo; //definicion de una variable puntero de tipo de FILE para abrir el archivo deseado
	archivo = fopen("Contactos.txt","r"); //abrir el .txt de Contactos
	
	if (archivo == NULL){ //si el archivo no existe, para aquí
		printf("No se encontró el archivo 'Contactos.txt', encuéntrelo y péguelo en la carpeta donde se encuentra este archivo.\n");
		printf("O cree un nuevo archivo vacío 'Contactos.txt' en la carpeta donde se encuentra este archivo.\n");
		return 0;
	}
	
	contacto contactos[100]; //arreglo de contactos
	int numContacto = 0; //numero de contacto actual en el arreglo
	
	//Variables para ir llenando cada contacto, de tamaño 100 porque así se almacenará palabra por palabra
	char leerNombre[100]; //nombre
	char leerIp[100]; //ip
	int leerPuerto; //puerto
	
	//Ciclo para leer 3 palabras (nombre, ip y puerto) cada vez
	while ((fscanf (archivo, "%s%s%i",leerNombre,leerIp,&leerPuerto) == 3) && (numContacto<100)) {
		//printf("LEYENDO: %s %s %i\n",leerNombre,leerIp,leerPuerto); //impresion de prueba
		
		contactos[numContacto].nombre = leerNombre;
		contactos[numContacto].ip = leerIp;
		contactos[numContacto].puerto = leerPuerto;
		
		//printf("PRUEBA --> CONTACTO # %d\n",numContacto); //impresion de prueba
		//imprimirContacto(&contactos[numContacto]); //impresion de prueba

		numContacto++;
	}
	fclose(archivo); //cerrar el archivo de lectura
//------------------------------------------------------------------------------------------------------------------------------------
	//Menú de opciones
	printf("Bienvenido al Chat MPJ\nSeleccione una de las siguientes opciones:\n");
	printf(Rojo "1- Agregar contactos" ResetColor "\n");
	printf(Verde "2- Visualizar contactos" ResetColor "\n");
	printf(Celeste "3- Enviar mensajes" ResetColor "\n");
	printf(Amarillo "4- Recibir mensajes" ResetColor "\n");
	printf("Presione 0 para salir de la aplicación.\n");
	
	int opcion; //donde se almacenará el input de opción del usuario
	scanf("%d",&opcion); //scanear el input del usuario
	while ((opcion < 0) || (opcion > 4)){ //restricciones
		printf("Ingrese un número de opción correcto: ");
		while (getchar()!='\n');  //limpiar el registro de standard input hasta la nueva línea (soluciona que "ccc" no vaya a entrar al while 3 veces, solo 1)
		scanf("%d",&opcion); //volver a scanear hasta llegar a un valor válido
	}
	
	//variables para ingresar los campos del nuevo contacto
	char inputNombre[100];
	char inputIP[100];
	int inputPuerto;
	
	switch(opcion){
		case 0:
			printf("\nHasta la próxima.\n");
			//mandarlo a limpiar IP's como dijo el profe
			break;
		case 1:
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
				printf("Ingrese el Puerto del contacto a guardar (No ingrese espacios): ");
				scanf("%i",&inputPuerto);
				while (getchar()!='\n');
			}
			
			//Agregar el nuevo contacto al array de contactos
			contactos[numContacto].nombre = inputNombre;
			contactos[numContacto].ip = inputIP;
			contactos[numContacto].puerto = inputPuerto;
			//printf("PRUEBA --> CONTACTO # %d\n",numContacto); //impresion de prueba
			//imprimirContacto(&contactos[numContacto]); //impresion de prueba
			numContacto++;
			
			//Escribir el nuevo contacto en Contactos.txt
			FILE *file;
			file= fopen("Contactos.txt","a"); //a para append,
			fprintf(file,"%s %s %i\n",inputNombre,inputIP,inputPuerto);
			fclose(file);
			
			printf(Verde "Se ha añadido el contacto correctamente." ResetColor "\n");
			break;
		/*case 2:
		case 3:
		case 4:*/
	}
	return 0;
}
