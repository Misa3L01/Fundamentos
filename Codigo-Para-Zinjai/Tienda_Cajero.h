#ifndef TIENDA_CAJERO_H
#define TIENDA_CAJERO_H

#include <stdio.h>
#include <stdlib.h> 
#include <conio.h>
#include <string.h>
#include <windows.h>
//#include "audio.h"

#include "Tienda_Estructuras.h"
#include "Tienda_Variado.h"
#include "Tienda_Inventario.h"

void MenuCobro(Producto *raiz){	  //Sistema de cobro y carga de carritos
	Producto *Lista = NULL;
	int salir = 0, subout = 1, ID;
	char act;
	float total=0, vuelto, pago;
	Producto *temp;
	Producto *Cart;

	//fade_in_slot_existente(2, 0.1, 600);
	
	
	int seleccion = 0;
			const int numOpciones = 3;
			char *opciones[] = { 
				"Ver cola de clientes",
				"Cobrar productos",
				"Salir"
			};

			while (1) {   //Menu con selecciones
				salir = 1;
				subout = 1;
				limpiarPantalla();
				// Dibuja el cuadro con color
    printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);
    printf(ANSI_GREEN "|" ANSI_RESET "      " ANSI_YELLOW "   --- Menu Cajero ---   " ANSI_GREEN "       |\n" ANSI_RESET);
    printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);

    for (int i = 0; i < numOpciones; i++) {
        printf(ANSI_GREEN "|" ANSI_RESET);
        if (i == seleccion)
            printf(ANSI_CYAN " -> %-33s " ANSI_RESET, opciones[i]);
        else
            printf("    %-33s ", opciones[i]);
        printf(ANSI_GREEN "|\n" ANSI_RESET);
    }
    printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);

int tecla = getch();
		if (tecla == 224) { // Tecla especial (flechas)
			tecla = getch();
			if (tecla == 72 && seleccion > 0) {// Flecha arriba
				seleccion--;
				//reproducir_en_slot(7, 7, 0, 0.3f);
			}
			else if (tecla == 80 && seleccion < numOpciones - 1) {// Flecha abajo
				seleccion++;
				//reproducir_en_slot(7, 7, 0, 0.3f);
			}
		} else if (tecla == 13) { // Enter
			switch(seleccion) {    //Caso 0: Cargar el carrito de un cliente
			case 0:
				Cart = NULL;
				Producto *actual;
				limpiarPantalla();
				
				FILE *Clientes = fopen("clientes.csv", "r");    //Lee el archivo de clientes y, si sus carritos estan confirmados, guarda sus nombres en una lista
				if(Clientes == NULL){
					printf("No hay clientes registrados\n");
					Sleep(500);
					break;
				}

				char Linea[200];
				char Nombre[50];
				Customer *List = NULL;
				while(fgets(Linea, sizeof(Linea), Clientes)){

    				char *token = strtok(Linea, ",");
					if (!token) continue;
					strncpy(Nombre, token, sizeof(Nombre) - 1);
					Nombre[sizeof(Nombre) - 1] = '\0';

					char dir[100];
					strcpy(dir, Nombre);//Se abre el archivo individual de cada usuario con su mismo nombre
					char fmt[10] = ".csv";
					strcat(dir, fmt);

					FILE *Persona = fopen(dir, "r"); //Se lee la primera linea de cada archivo, que corresponde a un entero (0 o 1)
					if(!Persona) continue; 	
					int estado = -1; 
					fscanf(Persona, "%d", &estado);

					if(estado){
						Customer *aux = (Customer *)malloc(sizeof(Customer));
						strcpy(aux->Nombre, Nombre);
						aux->next = List;
						List = aux; 
					}
					fclose(Persona);
				}

				if(List == NULL){
					printf("No hay carritos disponibles\n");
					Sleep(500);
					break;
				}
				int cnt = 0;
				printf("----------- Clientes en espera -----------\n\n");  //Imprime carritos disponibles
				Customer *mark = List;
				while(mark != NULL){
					cnt++;
					printf("[%d] %s\n",cnt, mark->Nombre);
					mark = mark->next;
				}
				int select;
				do{
					select = -9999;
					printf("Ingrese el numero del cliente: (Ingrese 0 para cancelar): ");
					scanf("%d", &select);
					limpiarBuffer();
					
					if(select > cnt || select < 0)
						printf("Opcion invalida\n");
					}while(select > cnt || select < 0);
				
					if(select == 0){
						printf("Regresando...\n");
						Sleep(500);
						break;
					}

						limpiarPantalla();
						mark = List;
						for(int i = 1; i < select; i++){
							mark = mark->next;
						}
						char dir[100];
						strcpy(dir,mark->Nombre);
						char fmt[10] = ".csv";
						strcat(dir, fmt);

						printf("Cliente: %s\n", mark->Nombre);  //Lee el carrito del cliente seleccionado, y lo guarda en una lista
						CargarCarritoALista(&Cart, dir);
						
						actual = Cart;
						float suma = 0;

						while(actual != NULL){    //Muestra el carrito
							printf("Nombre: %s, Categoria: %s, Precio: %.2f\n", actual->Nombre, actual->Categoria, actual->Precio);
							suma += actual->Precio;
							actual = actual->right;
						}
						int verify = 1;
						do{
							printf("\nTotal: %.2f\n", suma);
							printf("\nDesea proceder con la compra? (S o N): ");   //Pregunta para confirmar la compra
							//char put;
							act = getchar();
							limpiarBuffer();
							switch(act){
								case 'S': case 's':
									actual = Cart;
									Producto *tempo;
									while(actual != NULL){
										BuscarID(raiz, actual->ID, &tempo);  //Busca el nodo en el arbol y disminuye su stock (Si es mayor a 0)
										if(tempo->stock)
											tempo->stock--;
										actual = actual->right;
									}
									printf("Operacion exitosa\n");
									Sleep(500);
									RegistrarVenta(&Cart, suma);
									suma = 0;
									verify = 0;
									FILE *arkive = fopen(dir, "w");
									fprintf(arkive,"0");
									fclose(arkive);
									break;
								case 'N': case 'n':
									verify = 0;
									break;
								default:
									printf("Opcion invalida\n");	
							}
						}while(verify);


				break;
			case 1: 
			limpiarPantalla();
			printf("\nIngrese la ID del producto, ingrese 0 para completar la carga\nIngrese -1 para cancelar ultima operacion\n");	
	do{
		if(total > 0)
			printf("\nTotal: %.2f\n", total); 
		ID = -9999;
		printf("ID: ");
		scanf("%d", &ID); 
		limpiarBuffer();
		//reproducir_en_slot(10, 10, 0, 0.4f);
		if(ID == 0){
			if(total){   //Si ID es 0 y total es distinto de 0, se procede al menu de cobro
				limpiarPantalla();
				MostrarLista(Lista);
				printf("\nTotal: %.2f\n", total);
				do{
				salir = 0;
				printf("1- Efectivo\n");
				printf("2- Tarjeta\n");
				printf("Opcion: ");
				scanf("%c", &act);
				limpiarBuffer();
				switch(act){
				case '1':
					do{
						pago = -9999;	
						printf("Importe: ");
						scanf("%f", &pago);
						limpiarBuffer();
						vuelto = pago - total;
					}while(vuelto < 0);
					//reproducir_en_slot(4, 4, 0, 0.5f);
					printf("Vuelto: %.2f\n", vuelto);
					Sleep(500);
					break;
				case '2':
					//reproducir_en_slot(5, 5, 0, 0.5f);
					break;
				default:
					printf("Opcion invalida\n");
					salir = 1;
				}
				}while(salir); 
				printf("Operacion completa\n");
				Sleep(500);
				limpiarPantalla();
				printf("\nIngrese la ID del producto, ingrese 0 para completar la carga\nIngrese -1 para cancelar ultima operacion\n");					
				RegistrarVenta(&Lista, total);  //Se guardan los productos comprados en registro.csv
				total = 0;
			}
			else{   //Total = 0, sale del menu
			   printf("Regresando...\n");
			   subout = 0;
			}
		}
		else{
			if(ID == -1){  //Se elimina el ultimo producto
				EliminarNodo(&Lista, &total);
				Sleep(500);
			}
			else{
			if(!BuscarID(raiz, ID, &temp)){  //Se busca la ID del producto en arbol
				printf("Producto no encontrado\n");
				Sleep(500);
			}
			else{
				printf("Nombre: %s, Categoria: %s, Precio: %.2f\n", temp->Nombre, temp->Categoria, temp->Precio);
				total += temp->Precio;
				if(temp->stock){
					temp->stock--;
				}
				GuardarLista(temp, &Lista);  //Se guarda el producto en una lista
		    	}
			}
		}
			   
	   } while(subout);
				break;
			case 2: 
	   			//fade_out_pause_slot(2, 600); // Desvanece la música de fondo
				GuardarInventarioEnArchivo(raiz, "Inventario.csv");  //Se actualizan los valores de stock
				return;
			}
		}
	}
}
#endif // TIENDA_CAJERO_H
