#ifndef TIENDA_ADMIN_H
#define TIENDA_ADMIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h> // Para usar system("cls") y otras funciones de Windows
#include <conio.h> // Para poder usar getch()

#include "Tienda_Inventario.h" // Para poder usar las funciones de inventario
#include "Tienda_Variado.h" // Para usar funciones varias
#include "Tienda_Estructuras.h" // Para usar la estructura Producto
#include "audio.h" // Para usar funciones de audio

#define LLAVE_MAESTRA "HQDZE3HVMBELNRHL"

int verificarAdmin() {
    char llaveArchivo[100];
    FILE *archivo = fopen("recursos/output/llave.txt", "r");

    if (!archivo) {
        printf(ANSI_RED"No se pudo abrir el archivo de llave.\n"ANSI_RESET);
		reproducir_en_slot(11, 11, 0, 0.3f); // Reproduce un sonido de error
        Sleep(2000);
        return 0;
    }

    if (!fgets(llaveArchivo, sizeof(llaveArchivo), archivo)) {
        printf(ANSI_RED"Error al leer la llave del archivo.\n"ANSI_RESET);
		reproducir_en_slot(11, 11, 0, 0.3f); // Reproduce un sonido de error
        Sleep(2000);
        fclose(archivo);
        return 0;
    }
    fclose(archivo);

    // Eliminar salto de línea si existe
    size_t len = strlen(llaveArchivo);
    if (len > 0 && llaveArchivo[len - 1] == '\n') {
        llaveArchivo[len - 1] = '\0';
    }

    if (strcmp(llaveArchivo, LLAVE_MAESTRA) == 0) {
        printf(ANSI_GREEN"Acceso concedido.\n"ANSI_RESET);
		Sleep(500);
        return 1;
    } else {
        printf(ANSI_YELLOW"No se encontraron permisos de administrador.\n"ANSI_RESET);
		reproducir_en_slot(11, 11, 0, 0.3f); // Reproduce un sonido de error
        Sleep(2000);
        return 0;
    }
}

void menuAdmin(Producto **raiz) {
	fade_in_slot_existente(3, 0.1, 600); // Reproduce la música de fondo del administrador

	if (verificarAdmin()) {
		int seleccion = 0;
		const int numOpciones = 4;
		char *opciones[] = {
			"Ver inventario",
			"Agregar producto",
			"Eliminar producto",
			"Salir"
		};
		int salir = 0;
		while (!salir) {
				limpiarPantalla();
				// Dibuja el cuadro con color
    printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);
    printf(ANSI_GREEN "|" ANSI_RESET "  " ANSI_YELLOW "   --- Menu Administrador ---   " ANSI_GREEN "    |\n" ANSI_RESET);
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
					reproducir_en_slot(7, 7, 0, 0.3f);
				}
				else if (tecla == 80 && seleccion < numOpciones - 1) {// Flecha abajo
					seleccion++;
					reproducir_en_slot(7, 7, 0, 0.3f);
				}
			} else if (tecla == 13) { // Enter
				switch (seleccion) {
					case 0:
						limpiarPantalla();
						printf(
    ANSI_RED    "%-5s "   ANSI_GREEN  "%-33s "  ANSI_YELLOW "%-20s "
    ANSI_BLUE   "%-20s "  ANSI_MAGENTA "%-8s "  ANSI_CYAN   "%-10s" ANSI_RESET "\n",
    "ID", "Nombre", "Categoria", "Subcategoria", "Stock", "Precio"
);
						VerInventario(*raiz);
						system("pause");
						break;
					case 1:
						limpiarPantalla();
						printf("Agregar producto:\n");
						agregarProducto(raiz);
						// Guardar el inventario actualizado en el archivo
						FILE *archivo = fopen("recursos/output/Inventario.csv", "w");
						if (archivo) {
							GuardarInventarioEnArchivo(*raiz, "recursos/output/Inventario.csv");
							fclose(archivo);
						} else {
							printf(ANSI_RED"Error al guardar el inventario.\n"ANSI_RESET);
							reproducir_en_slot(11, 11, 0, 0.3f); // Reproduce un sonido de error
        					Sleep(2000);
							getchar();
						}
						printf(ANSI_GREEN"Producto agregado, Inventario actualizado.\n"ANSI_RESET);
						//limpiarPantalla();
						system("pause");
						break;
					case 2:
						//printf("Coloque la ID a eliminar (0 para cancelar): ");
						printf("Ingrese el ID a eliminar (");
						printf(ANSI_BLUE"0"ANSI_RESET") para cancelar: ");

						int ID;
						Producto* temp;
						scanf("%d", &ID);
						getchar();
						
						if(ID == 0){
						printf("Regresando...\n");
						Sleep(700);
						break;
						}

						if (search(raiz, ID)) {
							printf(ANSI_GREEN"Producto eliminado.\n"ANSI_RESET);
						} else {
							printf(ANSI_YELLOW"Producto no encontrado.\n"ANSI_RESET);
							Sleep(500);
						}
						GuardarInventarioEnArchivo(*raiz, "recursos/output/Inventario.csv");
						system("pause");
						break;
					case 3:
						salir = 1;
						fade_out_pause_slot(3, 600); // Pausa la música de fondo del administrador con fade out
						break;
				}
			}
		}
	} else {
		printf("Acceso denegado.\n");
		system("pause");
	}
} 

#endif // TIENDA_ADMIN_H
