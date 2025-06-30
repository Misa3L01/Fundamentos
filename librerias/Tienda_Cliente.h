#ifndef TIENDA_CLIENTE_H 
#define TIENDA_CLIENTE_H 

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <conio.h> // Para poder usar getch() y _getch()
#include <windows.h> // Para poder usaer system("pause")

#include "Tienda_Estructuras.h" // Para incluir la estructura Cliente
#include "Tienda_Variado.h" // Para usar funciones varias
#include "Tienda_Inventario.h" // Para usar funciones de inventario
#include "Tienda_Carrito.h" // Para usar funciones de carrito
#include "audio.h" // Para usar funciones de audio

void registrarCliente() {
    FILE *archivo = fopen("recursos/output/clientes.csv", "a"); // Abre el archivo con el nombre "clientes.csv" en modo append
    if (!archivo) { // Verifica si el archivo se abrio correctamente
        printf("No se pudo abrir el archivo de clientes.\n");
        return;
    }
    Cliente nuevo;
    printf("Nombre: ");
    fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin);
    nuevo.nombre[strcspn(nuevo.nombre, "\n")] = 0; // Elimina el salto de línea al final del nombre
    printf("Usuario: ");
    fgets(nuevo.usuario, sizeof(nuevo.usuario), stdin);
    nuevo.usuario[strcspn(nuevo.usuario, "\n")] = 0; // Elimina el salto de línea al final del usuario
    printf("Contrasenia: ");
    fgets(nuevo.contrasena, sizeof(nuevo.contrasena), stdin);
    nuevo.contrasena[strcspn(nuevo.contrasena, "\n")] = 0; // Elimina el salto de línea al final de la contraseña
    fprintf(archivo, "%s,%s,%s\n", nuevo.nombre, nuevo.usuario, nuevo.contrasena);
    fclose(archivo); // Cierra el archivo

    char dir[100] = "recursos/carritos/";
    strcat(dir, nuevo.nombre);
    char a[10] = ".csv";
    strcat(dir, a);

    system("pause");

    FILE *carrito = fopen(dir, "w"); // Crea un archivo de carrito para el usuario
    if (!carrito) { // Verifica si el archivo se abrio correctamente
        printf("No se pudo crear el carrito del usuario %s.\n", nuevo.usuario);
        return;
    }
    fprintf(carrito,"0\n");
    fclose(carrito);

    printf("Registro exitoso!\n");
    system("pause");
}

int loginCliente(char* nombreCliente) {
    char usuario[30], contrasena[30];
    printf("Usuario: ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = 0;
    printf("Contrasenia: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = 0;

    FILE *archivo = fopen("recursos/output/clientes.csv", "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de clientes.\n");
        return 0;
    }
    Cliente temp; // Variable temporal para almacenar los datos leidos
    int encontrado = 0;
    while (fscanf(archivo, "%49[^,],%29[^,],%29[^\n]\n", temp.nombre, temp.usuario, temp.contrasena) == 3) { // Lee los datos del archivo
        if (strcmp(usuario, temp.usuario) == 0 && strcmp(contrasena, temp.contrasena) == 0) { // Compara el usuario y la contraseña
            // Si coinciden, se coloca encontrado a 1 y gracias a eso se sale del bucle
            encontrado = 1;
            strncpy(nombreCliente, temp.nombre, 50);
            nombreCliente[49] = '\0'; // Asegura el fin de cadena
            break;
        }
    }
    fclose(archivo);
    if (encontrado) { // Si se encontro el usuario y la contraseña
        printf("!Bienvenido, %s!\n", nombreCliente); // Muestra el nombre del cliente
        system("pause");
        return 1;
    } else {
        printf("Usuario o contrasenia incorrectos.\n"); // Si no se encontro el usuario y la contraseña
        system("pause");
        return 0;
    }
}

void menuCliente(Producto **raiz) {
   Producto *Lista = NULL;
    fade_in_slot_existente(1, 0.1, 600); // Reproduce la música del menú en loop, con fade in gradual

    const char* opciones[] = {
        "Soy cliente nuevo (Registrarse)",
        "Ya tengo cuenta (Login)",
        "Salir"
    };

    const int numOpciones = sizeof(opciones) / sizeof(opciones[0]);
    int opcion = 0;
    int tecla;
    while (1) {
				limpiarPantalla();
				// Dibuja el cuadro con color
    printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);
    printf(ANSI_GREEN "|" ANSI_RESET "     " ANSI_YELLOW "   --- Menu Clientes ---   " ANSI_GREEN "      |\n" ANSI_RESET);
    printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);

    for (int i = 0; i < numOpciones; i++) {
        printf(ANSI_GREEN "|" ANSI_RESET);
        if (i == opcion)
            printf(ANSI_CYAN " -> %-33s " ANSI_RESET, opciones[i]);
        else
            printf("    %-33s ", opciones[i]);
        printf(ANSI_GREEN "|\n" ANSI_RESET);
    }
    printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);

        tecla = _getch();
        if (tecla == 224) { // Teclas especiales
            tecla = _getch();
            if (tecla == 72 && opcion > 0) {// Flecha arriba
                opcion--;
                reproducir_en_slot(7, 7, 0, 0.3f);
            }
            else if (tecla == 80 && opcion < numOpciones - 1) {// Flecha abajo
                opcion++;
                reproducir_en_slot(7, 7, 0, 0.3f);
            }
        } else if (tecla == 13) { // Enter
            switch (opcion) {
                case 0:
                    registrarCliente();
                    break;
                case 1:
                    char nombreCliente[50] = "";
                    if (loginCliente(nombreCliente)) {    //Se accede al carrito del cliente 
                        char direccion[50] = "recursos/carritos/";
                        strcat(direccion, nombreCliente);
                        strcat(direccion, ".csv");

                        CargarCarritoALista(&Lista, direccion);  //Se guarda el carrito en una lista

                        const char* opciones[] = {
                            "Ver productos",
                            "Ver carrito",
                            "Salir"
                        };

                        const int numOpciones = sizeof(opciones) / sizeof(opciones[0]);
                        int opcion = 0;
                        int tecla;
                        while (1) {  //Menu clientes
                            limpiarPantalla();
                            // Dibuja el cuadro con color
                            printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);
                            printf(ANSI_GREEN "|" ANSI_RESET "     " ANSI_YELLOW "   --- Menu Clientes ---   " ANSI_GREEN "      |\n" ANSI_RESET);
                            printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);

                            for (int i = 0; i < numOpciones; i++) {
                                printf(ANSI_GREEN "|" ANSI_RESET);
                                if (i == opcion)
                                    printf(ANSI_CYAN " -> %-33s " ANSI_RESET, opciones[i]);
                                else
                                    printf("    %-33s ", opciones[i]);
                                printf(ANSI_GREEN "|\n" ANSI_RESET);
                            }
                            printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);

                            tecla = _getch();
                            if (tecla == 224) { // Teclas especiales
                                tecla = _getch();
                                if (tecla == 72 && opcion > 0) {// Flecha arriba
                                    opcion--;
                                    reproducir_en_slot(7, 7, 0, 0.3f);
                                }
                                else if (tecla == 80 && opcion < numOpciones - 1) {// Flecha abajo
                                    opcion++;
                                    reproducir_en_slot(7, 7, 0, 0.3f);
                                }
                            } else if (tecla == 13) { // Enter
                                switch (opcion) {
                                case 0: 
                                    MenuInventarioCarrito(*raiz, &Lista);
                                    guardarCarritoCSV(Lista, direccion); // Guarda el carrito en el archivo del cliente
                                    system("pause"); // muestre el inventario, y agregar a la lista
                                    break;
                                case 1:

                                    const char* opciones2[] = {
                            "Editar carrito",
                            "Confirmar carrito",
                            "Salir"
                        };

                        const int numOpciones2 = sizeof(opciones2) / sizeof(opciones2[0]);
                        int opcion2 = 0;
                        int tecla;
                        int out = 1;
                        while (out) {  //Menu carrito
                            limpiarPantalla();
            
                            MostrarLista(Lista);

                            printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);
                            printf(ANSI_GREEN "|" ANSI_RESET "     " ANSI_YELLOW "    ---Menu Carrito  ---   " ANSI_GREEN "      |\n" ANSI_RESET);
                            printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);

                            for (int i = 0; i < numOpciones2; i++) {
                                printf(ANSI_GREEN "|" ANSI_RESET);
                                if (i == opcion2)
                                    printf(ANSI_CYAN " -> %-33s " ANSI_RESET, opciones2[i]);
                                else
                                    printf("    %-33s ", opciones2[i]);
                                printf(ANSI_GREEN "|\n" ANSI_RESET);
                            }
                            printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);

                            tecla = _getch();
                            if (tecla == 224) { // Teclas especiales
                                tecla = _getch();
                                if (tecla == 72 && opcion2 > 0) {// Flecha arriba
                                    opcion2--;
                                    reproducir_en_slot(7, 7, 0, 0.3f);
                                }
                                else if (tecla == 80 && opcion2 < numOpciones2 - 1) {// Flecha abajo
                                    opcion2++;
                                    reproducir_en_slot(7, 7, 0, 0.3f);
                                }
                            } else if (tecla == 13) { // Enter
                                  switch(opcion2){
                                    case 0:
                                    printf("coming soon\n");
                                        break;
                                    case 1:
                                        if(Lista == NULL){   //Si el carrito no esta vacio, abre el archivo y cambia la variable numerica
                                            printf("El carrito esta vacio\n");
                                            Sleep(500);
                                            break;
                                        }

                                        int subtout = 1;
                                        do{
                                            char act;
                                            printf("Desea confirmar su carrito? (S o N): ");
                                            act = getchar();
                                            limpiarBuffer();
                                            switch(act){
                                                case 'S': case 's':
                                                    FILE *carrito = fopen(direccion, "r+");
                                                    if(carrito == NULL){
                                                        printf("No se pudo abrir el archivo\n");
                                                        Sleep(500);
                                                        break;
                                                    }
                                                    fprintf(carrito, "1");
                                                    fclose(carrito);
                                                    printf("Se confirmo la compra\n");
                                                    Sleep(500);
                                                    subtout = 0;
                                                    break;
                                                case 'N': case 'n':
                                                     subtout = 0;
                                                    break;         
                                            }
                                        } while(subtout);
                                        break;
                                    case 2:
                                        out = 0;
                                        break;
                                  }  
                            }
                        }
                                break;
                                case 2:
                                    return;   
                                }
                            } 
                        } 
                    } 
                    break;
                case 2:
                    fade_out_pause_slot(1, 600); // Desvanece la música de fondo
                    return;
                }
            }
        }
    }
#endif // TIENDA_CLIENTE_H
