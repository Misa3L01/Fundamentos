#ifndef TIENDA_CARRITO_H
#define TIENDA_CARRITO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "Tienda_Estructuras.h" // Para incluir la estructura Item
#include "Tienda_Variado.h" // Para usar funciones varias
#include "audio.h" // Para usar funciones de audio
#include "Tienda_Inventario.h" // Para usar funciones de inventario

#define PRODUCTOS_POR_PAGINA 30

void MenuInventarioCarrito(Producto *inventario, Producto **carrito) {
    int total = 0;
    Producto *aux = inventario;
    while (aux) {
        total++;
        aux = aux->right;
    }

    Producto **productos = malloc(sizeof(Producto*) * total);
    aux = inventario;
    for (int i = 0; i < total; i++) {
        productos[i] = aux;
        aux = aux->right;
    }

    int pagina = 0;
    int seleccion = 0;
    int tecla;
    int totalPaginas = (total + PRODUCTOS_POR_PAGINA - 1) / PRODUCTOS_POR_PAGINA;

    while (1) {
        limpiarPantalla();
        printf(ANSI_GREEN "+------------------- Inventario (Pag %d/%d) -------------------+\n" ANSI_RESET, pagina + 1, totalPaginas);
        int inicio = pagina * PRODUCTOS_POR_PAGINA;
        int fin = inicio + PRODUCTOS_POR_PAGINA;
        if (fin > total) fin = total;
        for (int i = inicio; i < fin; i++) {
            if (i == inicio + seleccion) {
                printf(ANSI_CYAN "-> %-4d %-30s %-8d $%.2f\n" ANSI_RESET,
                    productos[i]->ID, productos[i]->Nombre, productos[i]->stock, productos[i]->Precio);
                }
            else
                printf("   %-4d %-30s %-8d $%.2f\n",
                    productos[i]->ID, productos[i]->Nombre, productos[i]->stock, productos[i]->Precio);
        }
        printf("\nUsa flechas (arriba/abajo) para moverte, (izq/der) para cambiar pagina, Enter para agregar al carrito, ESC para salir.\n");

        tecla = _getch();
        if (tecla == 224) { // Teclas especiales
            tecla = _getch();
            if (tecla == 72 && seleccion > 0) { // Flecha arriba
                seleccion--;
                reproducir_en_slot(8, 8, 0, 0.1f);
            } else if (tecla == 80 && seleccion < (fin - inicio - 1)) { // Flecha abajo
                seleccion++;
                reproducir_en_slot(9, 9, 0, 0.1f);
            } else if (tecla == 75 && pagina > 0) { // Flecha izquierda
                pagina--;
                reproducir_en_slot(12, 12, 0, 0.3f);
                int productosEnPagina = (pagina == totalPaginas - 1) ? (total % PRODUCTOS_POR_PAGINA) : PRODUCTOS_POR_PAGINA;
                if (productosEnPagina == 0) productosEnPagina = PRODUCTOS_POR_PAGINA;
                if (seleccion >= productosEnPagina) seleccion = productosEnPagina - 1;
            } else if (tecla == 77 && pagina < totalPaginas - 1) { // Flecha derecha
                pagina++;
                reproducir_en_slot(12, 12, 0, 0.3f);
                int productosEnPagina = (pagina == totalPaginas - 1) ? (total % PRODUCTOS_POR_PAGINA) : PRODUCTOS_POR_PAGINA;
                if (productosEnPagina == 0) productosEnPagina = PRODUCTOS_POR_PAGINA;
                if (seleccion >= productosEnPagina) seleccion = productosEnPagina - 1;
            }
        } else if (tecla == 13) { // Enter
            reproducir_en_slot(13, 13, 0, 0.3f);
            int idx = pagina * PRODUCTOS_POR_PAGINA + seleccion;
            if (idx < total) {
                Producto *nuevo = (Producto*)malloc(sizeof(Producto));
                memcpy(nuevo, productos[idx], sizeof(Producto));
                nuevo->right = *carrito;
                *carrito = nuevo;
                printf("Producto agregado al carrito!\n");
                Sleep(200);
            }
        } else if (tecla == 27) { // ESC
            reproducir_en_slot(14, 14, 0, 0.3f);
            break;
        }
    }
    free(productos);
}

void guardarCarritoCSV(Producto *carrito, const char *rutaArchivo) {
    FILE *archivo = fopen(rutaArchivo, "a");
    if (!archivo) {
        printf("No se pudo guardar el carrito en %s\n", rutaArchivo);
        reproducir_en_slot(11, 11, 0, 0.3f); // Reproduce un sonido de error
        Sleep(2000);
        return;
    }
    Producto *aux = carrito;
    while (aux) {
        fprintf(archivo, "%d,%s,%s,%s,%d,%.2f\n",
            aux->ID, aux->Nombre, aux->Categoria, aux->Subcategoria, aux->stock, aux->Precio);
        aux = aux->right;
    }
    fclose(archivo);
}

#endif // TIENDA_CARRITO_H