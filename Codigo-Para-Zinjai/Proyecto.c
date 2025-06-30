#include <stdio.h>
#include <stdlib.h> 
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "Tienda_Estructuras.h"
#include "Tienda_Admin.h"
#include "Tienda_Cliente.h"
#include "Tienda_Variado.h"
#include "Tienda_Inventario.h"
#include "Tienda_Cajero.h"
#include "Tienda_Imagen.h"
//#include "audio.h"
	
int main() {
	Producto *raiz = NULL;
	Precarga(&raiz);

	// Inicializa el sistema de audio
	//init_audio();
	//if (!audio_system) {
	//	printf("Error al inicializar el sistema de audio.\n");
	//	return -1;
	//}
	// Carga los sonidos y los deja en pausa para evitar creaciones innecesarias de canales de audio
	//precargar_sonidos();

	//fade_in_slot_existente(6, 0.3, 600); // Reproduce la musica de fondo

	// Reproduce la animacion de inicio
	cargar_frames(); 
    limpiarPantalla();
    reproducir_animacion_ascii();
    liberar_frames();

	//fade_out_slot(6, 600); // Desvanece la musica de fondo
	
	int seleccion = 0;
	const int numOpciones = 4;
	char *opciones[] = { 
		"Cliente",
		"Empleado",
		"Administrador",
		"Salir"
	};

	limpiarPantalla(); // Limpia la pantalla
	//fade_in_slot_existente(0, 1.0, 600); // Reproduce la música del menú en loop, con fade in gradual
			
	while (1) {
		limpiarPantalla(); // Limpia la pantalla para cada iteracion con el menu
		//actualizar_audio(); // Actualiza el audio en cada iteración para evitar problemas de sincronización

		// Recuadro con color
		printf(ANSI_GREEN "+--------------------------------------+\n" ANSI_RESET);
    	printf(ANSI_GREEN "|" ANSI_RESET "      " ANSI_YELLOW "   --- Minimercado ---   " ANSI_GREEN "       |\n" ANSI_RESET);
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
			//reproducir_en_slot(7, 7, 0, 0.3f); // Reproduce un sonido al navegar
		}
		else if (tecla == 80 && seleccion < numOpciones - 1) {// Flecha abajo
			seleccion++;
			//reproducir_en_slot(7, 7, 0, 0.3f); // Reproduce un sonido al navegar
		}
	} else if (tecla == 13) { // Enter
		switch(seleccion) {
		case 0: 
			//fade_out_pause_slot(0, 600); // Desvanece la música de fondo
			menuCliente(&raiz);
			//fade_in_slot_existente(0, 1.0, 600); // Reproduce la música del menú en loop, con fade in gradual
			break;
		case 1: 
			//fade_out_pause_slot(0, 600); // Desvanece la música de fondo
			MenuCobro(raiz);
			//fade_in_slot_existente(0, 1.0, 600); // Reproduce la música del menú en loop, con fade in gradual
			break;
		case 2: 
			//fade_out_pause_slot(0, 600); // Desvanece la música de fondo
			menuAdmin(&raiz);	
			//fade_in_slot_existente(0, 1.0, 600); // Reproduce la música del menú en loop, con fade in gradual
			break;
		case 3: 
			printf(ANSI_BLUE "Saliendo...\n" ANSI_RESET);
			//fade_out_slot(0, 600); // Desvanece la música de fondo
			CerrarImagen(); // Cerrar cualquier imagen abierta
			// Libera los recursos de audio
			//close_audio();
			return 0;
		}	
	}
}	
	return 0;
}
