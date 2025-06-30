#ifndef TIENDA_INVENTARIO_H
#define TIENDA_INVENTARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Tienda_Estructuras.h"
#include "Tienda_Variado.h" // Para usar funciones varias

Producto *Insertar(Producto **raiz, int ID, char Nombre[], char Categoria[], char Subcategoria[], int stock, float Precio){
	if(!(*raiz)){
		*raiz = (Producto *)malloc(sizeof(Producto));
		(*raiz)->ID = ID;
		strcpy((*raiz)->Nombre, Nombre);
		strcpy((*raiz)->Categoria, Categoria);
		strcpy((*raiz)->Subcategoria, Subcategoria);
		(*raiz)->stock = stock;
		(*raiz)->Precio = Precio;
		
		(*raiz)->left = NULL;
		(*raiz)->right = NULL;
		
		return *raiz;
	}
	if((*raiz)->ID < ID)
		 Insertar(&(*raiz)->right, ID, Nombre, Categoria, Subcategoria, stock, Precio);
	else
		if((*raiz)->ID > ID)
		Insertar(&(*raiz)->left, ID, Nombre, Categoria, Subcategoria, stock, Precio);	
		else{
		printf("El ID %d ya existe en el inventario.\n", ID);
		return NULL;
		}
	return *raiz;
}

void Precarga(Producto **raiz){
	char linea[200];
	
	FILE* archivo = fopen("recursos/output/Inventario.csv", "r");
	if (archivo == NULL) {
		perror("No se pudo abrir el archivo");
		return;
	}
		
	int ID,stock;
	char Nombre[N],Categoria[N],Subcategoria[N];
	float Precio;
		
	//fgets(linea,sizeof(linea),archivo);
		
	while(fgets(linea, sizeof(linea), archivo)){
			
		char* token = strtok(linea, ",");
		if (!token) continue;
		ID = atoi(token);
			
		token = strtok(NULL, ",");
		if (!token) continue;
		strncpy(Nombre, token, sizeof(Nombre) - 1);
		Nombre[sizeof(Nombre) - 1] = '\0';
			
		token = strtok(NULL, ",");
		if (!token) continue;
		strncpy(Categoria, token, sizeof(Categoria) - 1);
		Categoria[sizeof(Categoria) - 1] = '\0';
			
		token = strtok(NULL, ",");
		if (!token) continue;
		strncpy(Subcategoria, token, sizeof(Subcategoria) - 1);
		Subcategoria[sizeof(Subcategoria) - 1] = '\0';
		
		token = strtok(NULL, ",");
		if (!token) continue;
		stock = atoi(token);
			
		token = strtok(NULL, ",");
		if (!token) continue;
		Precio = atof(token);
			
		Insertar(raiz, ID, Nombre, Categoria, Subcategoria, stock, Precio);
	}
}

// Inserta un producto en el ABB
Producto* InsertarProducto(Producto* raiz, int ID, const char* Nombre, const char* Categoria, const char* Subcategoria, int stock, float Precio) {
    if (raiz == NULL) {
        Producto* nuevo = (Producto*)malloc(sizeof(Producto));
        if (!nuevo) return NULL;
        strncpy(nuevo->Nombre, Nombre, sizeof(nuevo->Nombre) - 1);
        nuevo->Nombre[sizeof(nuevo->Nombre) - 1] = '\0';
        strncpy(nuevo->Categoria, Categoria, sizeof(nuevo->Categoria) - 1);
        nuevo->Categoria[sizeof(nuevo->Categoria) - 1] = '\0';
        strncpy(nuevo->Subcategoria, Subcategoria, sizeof(nuevo->Subcategoria) - 1);
        nuevo->Subcategoria[sizeof(nuevo->Subcategoria) - 1] = '\0';
        nuevo->ID = ID;
        nuevo->stock = stock;
        nuevo->Precio = Precio;
        nuevo->left = nuevo->right = NULL;
        return nuevo;
    }
    if (ID < raiz->ID)
        raiz->left = InsertarProducto(raiz->left, ID, Nombre, Categoria, Subcategoria, stock, Precio);
    else if (ID > raiz->ID)
        raiz->right = InsertarProducto(raiz->right, ID, Nombre, Categoria, Subcategoria, stock, Precio);
    return raiz;
}

// Busca un producto por ID
Producto* BuscarProducto(Producto* raiz, int ID) {
    if (raiz == NULL || raiz->ID == ID)
        return raiz;
    if (ID < raiz->ID)
        return BuscarProducto(raiz->left, ID);
    else
        return BuscarProducto(raiz->right, ID);
}

// Recorrido inorden para mostrar productos
void VerInventario(Producto* raiz) {
    if (raiz == NULL) return;

    VerInventario(raiz->left);

    printf(ANSI_GREEN"%-5d"ANSI_RESET "%-35s %-20s %-20s", 
        raiz->ID, raiz->Nombre, raiz->Categoria, raiz->Subcategoria);

    if (raiz->stock == 0) { // Si el stock es 0, se muestra en rojo
        printf(ANSI_RED"%-8d"ANSI_RESET, raiz->stock);
    } else if (raiz->stock < 10) { // Si el stock es menor a 10, se muestra en amarillo
        printf(ANSI_YELLOW"%-8d"ANSI_RESET, raiz->stock);
    } else { // Si el stock es mayor o igual a 10, se muestra en verde
        printf("%-8d", raiz->stock);
    }

    printf("%-10.2f\n", raiz->Precio);

    VerInventario(raiz->right);
}

// Se carga el inventario desde un archivo CSV
void CargarInventarioDesdeArchivo(Producto** raiz, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) return;
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        int ID, stock;
        float Precio;
        char Nombre[N], Categoria[N], Subcategoria[N];
        if (sscanf(linea, "%d,%99[^,],%99[^,],%99[^,],%d,%f", &ID, Nombre, Categoria, Subcategoria, &stock, &Precio) == 6) {
            *raiz = InsertarProducto(*raiz, ID, Nombre, Categoria, Subcategoria, stock, Precio);
        }
    }
    fclose(archivo);
}

// Guarda el inventario en un archivo CSV
void GuardarInventarioEnArchivoRec(Producto* raiz, FILE* archivo) {
    if (raiz == NULL) return;
    GuardarInventarioEnArchivoRec(raiz->left, archivo);
    fprintf(archivo, "%d,%s,%s,%s,%d,%.2f\n", raiz->ID, raiz->Nombre, raiz->Categoria, raiz->Subcategoria, raiz->stock, raiz->Precio);
    GuardarInventarioEnArchivoRec(raiz->right, archivo);
}

void GuardarInventarioEnArchivo(Producto* raiz, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) return;
    GuardarInventarioEnArchivoRec(raiz, archivo);
    fclose(archivo);
}

// Libera la memoria del inventario
void LiberarInventario(Producto* raiz) {
    if (raiz == NULL) return;
    LiberarInventario(raiz->left);
    LiberarInventario(raiz->right);
    free(raiz);
}

int BuscarID(Producto *raiz, int ID, Producto **temp){
	if(!raiz)
		return 0;
	
	if(raiz->ID == ID){
		*temp = raiz;
		return 1;
	}
	if(ID > raiz->ID)
		return BuscarID(raiz->right, ID, temp);
	else
		return BuscarID(raiz->left, ID, temp);
}

void agregarProducto(Producto **raiz) {
    int ID, stock = -1;
    float Precio;
    char Nombre[N], Categoria[N], Subcategoria[N];

    printf("Ingrese el ID del producto: ");
    scanf("%d", &ID);
    while (BuscarProducto(*raiz, ID) != NULL || ID <= 0) {
        printf("El ID ya existe o es invalido. Ingrese un ID diferente: ");
        getchar();
        scanf("%d", &ID);
    }
    getchar(); // Limpiar el buffer de entrada

    printf("Ingrese el nombre del producto: ");
    fgets(Nombre, sizeof(Nombre), stdin);
    Nombre[strcspn(Nombre, "\n")] = 0;
    if (strlen(Nombre) == 0) {
        printf("El nombre no puede estar en blanco.\n");
        return;
    }

    printf("Ingrese la categoria del producto: ");
    fgets(Categoria, sizeof(Categoria), stdin);
    Categoria[strcspn(Categoria, "\n")] = 0;\
    if (strlen(Categoria) == 0) {
        printf("La categoria no puede estar en blanco.\n");
        return;
    }

    printf("Ingrese la subcategoria del producto: ");
    fgets(Subcategoria, sizeof(Subcategoria), stdin);
    Subcategoria[strcspn(Subcategoria, "\n")] = 0;
    if (strlen(Subcategoria) == 0) {
        printf("La subcategoria no puede estar en blanco.\n");
        return;
    }

    printf("Ingrese el stock del producto: ");
    scanf("%d", &stock);
	limpiarBuffer();

    while (stock < 0)
	{
		printf("Ingrese un stock valido: ");
		scanf("%d", &stock);
		limpiarBuffer();
	}

    printf("Ingrese el precio del producto: ");
    scanf("%f", &Precio);
	limpiarBuffer();

	while (Precio <= 0.0)
	{
		printf("Ingrese un precio valido: ");
		scanf("%f", &Precio);
		limpiarBuffer();
	}
	

    Insertar(raiz, ID, Nombre, Categoria, Subcategoria, stock, Precio);
}


Producto *CargarLista(Producto *temp){
	if(temp == NULL)
		return NULL;

	
	Producto *aux = (Producto *)malloc(sizeof(Producto));
	strcpy(aux->Categoria, temp->Categoria);
	strcpy(aux->Nombre, temp->Nombre);
	strcpy(aux->Subcategoria, temp->Subcategoria);
	aux->stock = temp->stock;
	aux->ID = temp->ID;
	aux->Precio = temp->Precio;
	
	aux->right = temp->right;
	aux->left = temp->left;
	
	return aux;
}
	
void GuardarLista(Producto *temp, Producto **Lista){
	Producto *aux = CargarLista(temp);
	Producto *mark;	
		
	if(!(*Lista)){
		*Lista = aux;
		aux->right = NULL;
		aux->left = NULL;
		return;
	}
	mark = *Lista;
	while(mark->right != NULL)
		mark = mark->right;
	mark->right = aux;
	aux->right = NULL;
	aux->left = NULL;
	
	return;
}
	
void BorrarLista(Producto **Lista){
	Producto *mark = *Lista;
	while(mark != NULL){
		(*Lista) = (*Lista)->right;
		free(mark);
		mark = *Lista;
	}
	free(*Lista);
	*Lista = NULL;
}
	
void RegistrarVenta(Producto **Lista, float total){
	Producto *mark = *Lista;
	if(!(*Lista)) return;
	
	FILE *archivo = fopen("recursos/output/Registro.csv", "a");
	if(archivo == NULL)
		return;
	
	time_t t = time(NULL);
	struct tm *tm_info = localtime(&t);
	char fechaHora[30];
	strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M", tm_info);
	
	fprintf(archivo, "Fecha: %s\n", fechaHora);
	fprintf(archivo, "ID,Nombre,Categoria,Subcategoria,Precio\n");
	
	while(mark != NULL){
		fprintf(archivo,"%d,%s,%s,%s,%.2f\n",mark->ID, mark->Nombre, mark->Categoria, mark->Subcategoria, mark->Precio);
		mark = mark->right;
	}
	
	fprintf(archivo,"Total %.2f\n\n", total);
	
	fclose(archivo);
	
	BorrarLista(Lista);
}
	
void Copiar(Producto *raiz, Producto *raizDir){
	strcpy(raiz->Nombre, raizDir->Nombre);
	strcpy(raiz->Categoria, raizDir->Categoria);
	strcpy(raiz->Subcategoria, raizDir->Subcategoria);
	raiz->stock = raizDir->stock;
	raiz->ID = raizDir->ID;
	raiz->Precio = raizDir->Precio;
}

	void purge(Producto **raiz){
		Producto *nodo, *temp;	
		if((*raiz)->left == NULL && (*raiz)->right == NULL){
			free(*raiz);
			*raiz = NULL;
			return;
		}
		if((*raiz)->left == NULL){
			nodo = (*raiz)->right;
			free(*raiz);
			*raiz = NULL;
			*raiz = nodo;
			return;
		}
		if((*raiz)->right == NULL){
			nodo = (*raiz)->left;
			free(*raiz);
			*raiz = NULL;
			*raiz = nodo;
			return;
		}
		nodo = (*raiz)->right;
		temp = *raiz;
		while(nodo->left != NULL){
			temp = nodo;
			nodo = nodo->left;
		}
		Copiar(*raiz, nodo);
		if(temp == *raiz){
			temp->right = nodo->right;
		}
		else{
			if(nodo->right){
				temp->left = nodo->right;
			}
			else{
				temp->left = NULL;
			}
		}
		free(nodo);
		nodo = NULL;
		return;
		
	}	

	int search(Producto **raiz, int ID){
	if(!(*raiz))
		return 0;
	if((*raiz)->ID == ID){
		purge(raiz);
		return 1;
	}
	if(search(&(*raiz)->left, ID) || search(&(*raiz)->right, ID))
		return 1;
	else
		return 0;
}

void MostrarLista(Producto *Lista){
	while(Lista != NULL){
		printf("Nombre: %s, Categoria: %s, Precio: %.2f\n", Lista->Nombre, Lista->Categoria, Lista->Precio);
		Lista = Lista->right;
	}
}

void EliminarNodo(Producto **Lista, float *total){
	if(!(*Lista)){
		printf("La lista esta vacia\n");
		return;
	}
	Producto *mark = *Lista;
	Producto *pre = *Lista;

	while(mark->right != NULL){
		pre = mark;
		mark = mark->right;
	}

	if(total != NULL)
		*total -= mark->Precio;

	if(mark == *Lista){
		free(*Lista);
		*Lista = NULL;
		return;
	}
	free(mark);
	pre->right = NULL;
	return;
}

void CargarCarritoALista(Producto ** Lista, char direccion[]){
	FILE *archivo = fopen(direccion, "r");
	if(archivo == NULL)
		return;

	char linea[200];
	Producto *aux;
	Producto *mark;
	/*int ID,stock;
	char Nombre[N],Categoria[N],Subcategoria[N];
	float Precio;*/

	fgets(linea,sizeof(linea),archivo);
		
	while(fgets(linea, sizeof(linea), archivo)){
			
		aux = (Producto *)malloc(sizeof(Producto));

		char* token = strtok(linea, ",");
		if (!token) continue;
		aux->ID = atoi(token);
			
		token = strtok(NULL, ",");
		if (!token) continue;
		strncpy(aux->Nombre, token, sizeof(aux->Nombre) - 1);
		aux->Nombre[sizeof(aux->Nombre) - 1] = '\0';
			
		token = strtok(NULL, ",");
		if (!token) continue;
		strncpy(aux->Categoria, token, sizeof(aux->Categoria) - 1);
		aux->Categoria[sizeof(aux->Categoria) - 1] = '\0';
			
		token = strtok(NULL, ",");
		if (!token) continue;
		strncpy(aux->Subcategoria, token, sizeof(aux->Subcategoria) - 1);
		aux->Subcategoria[sizeof(aux->Subcategoria) - 1] = '\0';
		
		token = strtok(NULL, ",");
		if (!token) continue;
		aux->stock = atoi(token);
			
		token = strtok(NULL, ",");
		if (!token) continue;
		aux->Precio = atof(token);
			
		aux->right = *Lista;
		aux->left = NULL;
		*Lista = aux;

	}	
	fclose(archivo);
}

#endif
