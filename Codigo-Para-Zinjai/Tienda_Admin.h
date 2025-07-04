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
//#include "audio.h" // Para usar funciones de audio

#define LLAVE_MAESTRA "HQDZE3HVMBELNRHL"

int verificarAdmin() {  //Sistema de seguridad del Administrador
    char llaveArchivo[100];
    FILE *archivo = fopen("llave.txt", "r");

    if (!archivo) {
        printf("No se pudo abrir el archivo de llave.\n");
        return 0;
    }

    if (!fgets(llaveArchivo, sizeof(llaveArchivo), archivo)) {
        printf("Error al leer la llave del archivo.\n");
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
        printf("Acceso concedido.\n");
        return 1;
    } else {
        printf("No se encontraron permisos de administrador.\n");
        return 0;
    }
}

void menuAdmin(Producto **raiz) {
	//CargarInventarioDesdeArchivo(raiz, "recursos/output/Inventario.csv");
	//fade_in_slot_existente(3, 0.1, 600); // Reproduce la música de fondo del administrador

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
					//reproducir_en_slot(7, 7, 0, 0.3f);
				}
				else if (tecla == 80 && seleccion < numOpciones - 1) {// Flecha abajo
					seleccion++;
					//reproducir_en_slot(7, 7, 0, 0.3f);
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
						FILE *archivo = fopen("Inventario.csv", "w");
						if (archivo) {
							GuardarInventarioEnArchivo(*raiz, "Inventario.csv");
							fclose(archivo);
						} else {
							printf("Error al guardar el inventario.\n");
							getchar();
						}
						printf("Producto agregado, Inventario actualizado.\n");
						limpiarPantalla();
						system("pause");
						break;
					case 2:
						printf("Coloque la ID a eliminar: ");
						int ID;
						//Producto* temp;
						scanf("%d", &ID);
						getchar();
						//search(raiz, ID);
						if (search(raiz, ID)) {
							printf("Producto eliminado.\n");
						} else {
							printf("Producto no encontrado.\n");
						}
						GuardarInventarioEnArchivo(*raiz, "Inventario.csv");
						system("pause");
						break;
					case 3:
						salir = 1;
						//fade_out_slot(3, 600); // Desvanece la música de fondo del administrador
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
