#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char nombre;
	char ip;
	char puerto;
} contacto;

/*void imprimirContactos(contacto[] *ptr){
	int i;
	for (i=0,i<=100,i++){
		if ((*ptr).nombre)
			printf("%c",(*ptr).nombre);
		ptr++;
	}
}*/

/*contacto[] cargarContactos(char file){
	FILE *f; 
	f = fopen(file,"r"); //abrir el .txt de Contactos
	
	contacto contactos[100]; //array de contactos
	//struct contacto actual; //contacto actual llenandose
	char c; //caracter leyendose
	
	int numContacto = 0; //numero de contactos

	int campo = 1; //variable para saber por cual campo de los atributos de contacto va: valores {1,2,3}  
	
	char nombreActual;  //variables para ir llenando cada contacto
	char ipActual;
	char puertoActual;
	
	while((c=fgetc(f)) != EOF){
		if (c=='_') //leer siguiente campo
			campo++;
		
		else if (c=='\n'){ //leer siguiente contacto
			campo = 1; //reinicializar el numero atributo del contacto a 1 para el siguiente contacto
			//llenar los contactos del arreglo
			contactos[numContacto].nombre = nombreActual;
			contactos[numContacto].ip = ipActual;
			contactos[numContacto].puerto = puertoActual;
			numContacto++;
		}	
		
		else{ //caracter
			switch (campo){
				case 1: //leyendo nombre
					strcat(nombreActual,c);
				case 2: //leyendo ip
					strcat(ipActual,c);
				case 3:	//leyendo puerto
					strcat(puertoActual,c);
			}
		}
	}
	fclose(f);
	
	return contactos;
}*/

int main(){
	//cargar contactos
	
	FILE *f; 
	f = fopen("Contactos.txt","r"); //abrir el .txt de Contactos
	
	if (f == NULL){
		printf("El archivo no existe.");
		return 0;
	}
	
	contacto contactos[100]; //array de contactos
	//struct contacto actual; //contacto actual llenandose
	//char c; //caracter leyendose
	
	int numContacto = 0; //numero de contactos

	int campo = 1; //variable para saber por cual campo de los atributos de contacto va: valores {1,2,3}  
	
	char *nombreActual;  //variables para ir llenando cada contacto
	char *ipActual;
	char *puertoActual;
	
	char oneword[100];
	char c;
	//fp1 = fopen("Contactos.txt","r");
	while (c != EOF){
		c = fscanf(f,"%s",oneword);
		printf("LEYENDO: %s \n",&oneword);
		//printf("CAMPO = %d \n",campo);
		/*switch (campo){
				case 1: //leyendo nombre
					nombreActual = &oneword;
					printf("----------nombreActual = %s \n",nombreActual);
					campo++;
				case 2: //leyendo ip
					ipActual = &oneword;
					printf("----------ipActual = %s \n",ipActual);
					campo++;
				case 3:	//leyendo puerto
					puertoActual = &oneword;
					printf("----------puertoActual = %s \n",puertoActual);
					campo = 1;
					contactos[numContacto].nombre = *nombreActual;
					//printf("PRUEBA \n CONTACTO # %d = %s  \n",numContacto,*contactos[numContacto].nombre);
					contactos[numContacto].ip = *ipActual;
					contactos[numContacto].puerto = *puertoActual;
					numContacto++;
			}*/
			
		if (campo == 1){ //leyendo nombre
			nombreActual = &oneword;
			printf("----------nombreActual = %s \n",nombreActual);
			campo++;
		}
		else if (campo == 2){ //leyendo ip
			ipActual = &oneword;
			printf("----------ipActual = %s \n",ipActual);
			campo++;
		}
		else{ //leyendo puerto
			puertoActual = &oneword;
			printf("----------puertoActual = %s \n",puertoActual);
			campo = 1;
			contactos[numContacto].nombre = *nombreActual;
			//printf("88888888888888" + contactos[numContacto].nombre);
			//contacto *puntero = &contactos[numContacto];
			printf("\n PRUEBA \n CONTACTO # %d = %s \n \n",numContacto,(contactos[numContacto].nombre));
			contactos[numContacto].ip = *ipActual;
			contactos[numContacto].puerto = *puertoActual;
			numContacto++;
		}
	}
	
	/*while((c=fgetc(f)) != EOF){
		if (c=='_') //leer siguiente campo	
			campo++;
		
		else if (c=='\n'){ //leer siguiente contacto
			campo = 1; //reinicializar el numero atributo del contacto a 1 para el siguiente contacto
			//llenar los contactos del arreglo
			contactos[numContacto].nombre = nombreActual;
			contactos[numContacto].ip = ipActual;
			contactos[numContacto].puerto = puertoActual;
			numContacto++;
		}	
		
		else{ //caracter
			switch (campo){
				case 1: //leyendo nombre
					strcat(nombreActual,c);
				case 2: //leyendo ip
					strcat(ipActual,c);
				case 3:	//leyendo puerto
					strcat(puertoActual,c);
			}
		}
	}*/
	fclose(f);
	
	//contacto Contactos[100];
	//Contactos = cargarContactos("Contactos.txt");
	
	//prueba de imprimir los contactos
	//imprimirContactos(&Contactos);
	
	int i;
	contacto *ptr = &contactos;
	for (i=0;i<100;i++){
		if (ptr->nombre){
			printf("Contacto # %d = %s \n",i,&(contactos[i].nombre));
			ptr++;
		}
	}
	
	return 0;
}
