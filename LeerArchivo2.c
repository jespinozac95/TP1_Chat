#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estructura definida como "contacto"
typedef struct{
	char* nombre;
	char* ip;
	char* puerto;
} contacto;

//Funcion: imprimir los miembros de un contacto
//Entradas: un puntero hacia el contacto a imprimirle los miembros
//Restricciones: -
//Salidas: - (impresiones en pantalla)
void imprimirContacto(contacto* ptr) {
    if (ptr->nombre){
		printf("Nombre = %s\n",ptr->nombre);
		printf("IP = %s\n",ptr->ip);
		printf("Puerto = %s\n\n",ptr->puerto);
	}
}

//Funcion de inicio main()
int main(){
	//Cargar contactos
	
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
	char leerPuerto[100]; //puerto
		
	//Ciclo para leer 3 palabras (nombre, ip y puerto) cada vez
	while ((fscanf (archivo, "%s%s%s",leerNombre,leerIp,leerPuerto) == 3) && (numContacto<100)) {
		printf("LEYENDO: %s %s %s\n",leerNombre,leerIp,leerPuerto); //impresion de prueba
		
		contactos[numContacto].nombre = leerNombre;
		contactos[numContacto].ip = leerIp;
		contactos[numContacto].puerto = leerPuerto;
		
		printf("PRUEBA --> CONTACTO # %d\n",numContacto); //impresion de prueba
		imprimirContacto(&contactos[numContacto]); //impresion de prueba

		numContacto++;
	}
	fclose(archivo); //cerrar el archivo de lectura
	
	return 0;
}
