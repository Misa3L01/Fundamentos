#ifndef TIENDA_VARIADO_H
#define TIENDA_VARIADO_H

// ===== COLORES =====
#define ANSI_RESET   "\x1b[0m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_WHITE   "\x1b[37m"

// ===== PANTALLA =====
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Para system("cls") en Windows

// ===== ANIMACION ASCII =====
#include <string.h>

#define TOTAL_FRAMES 50
#define FRAME_FILE_PATH "recursos/frames/%d.txt"
#define MAX_FRAME_SIZE 8126

const char* colores[] = { ANSI_CYAN, ANSI_GREEN, ANSI_YELLOW, ANSI_RESET };
char *frames[TOTAL_FRAMES];

static void limpiarPantalla() {
    system("cls");
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Cargar todos los frames desde archivos
void cargar_frames() {
    char filename[128];
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        snprintf(filename, sizeof(filename), FRAME_FILE_PATH, i);
        FILE *f = fopen(filename, "r");
        if (!f) {
            printf("Error al abrir %s\n", filename);
            getchar();
            exit(1);
        }

        frames[i] = (char *)malloc(MAX_FRAME_SIZE);
        if (!frames[i]) {
            printf("Error al asignar memoria para frame %d\n", i);
            getchar();
            exit(1);
        }

        size_t len = fread(frames[i], 1, MAX_FRAME_SIZE - 1, f);
        frames[i][len] = '\0';

        fclose(f);
    }
}

// Liberar memoria de los frames
void liberar_frames() {
    for (int i = 0; i < TOTAL_FRAMES; i++) {
        free(frames[i]);
        frames[i] = NULL;
    }
}

// Reproducir animación durante 5 segundos
void reproducir_animacion_ascii() {
    int tiempo_por_frame = 41; // ms
    int duracion_total = 8000; // ms
    int ciclos = duracion_total / tiempo_por_frame;

    for (int c = 0; c < ciclos; c++) {
        printf("\x1b[2J\x1b[H");
        printf("%s%s%s", colores[c % 4], frames[c % TOTAL_FRAMES], ANSI_RESET);
        Sleep(tiempo_por_frame);
    }
}

#endif // TIENDA_VARIADO_H