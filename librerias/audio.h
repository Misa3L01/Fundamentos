#ifndef AUDIO_H
#define AUDIO_H

#include "fmod.h"
#include <stdio.h>
#include <windows.h>

// Cantidad máxima de sonidos activos en simultáneo
#define MAX_SLOTS 16

// Sistema de sonido
FMOD_SYSTEM *audio_system;

// Canciones y efectos 
const char *canciones[] = {
    "recursos/audio/MenuV2.wav", // 0: Música de fondo del menú
    "recursos/audio/MusicaCliente.mp3", // 1: Música de fondo del cliente
    "recursos/audio/MusicaCajero.mp3", // 2: Música de fondo del cajero
    "recursos/audio/AdministradorMusica.mp3", // 3: Música de fondo del administrador
    "recursos/audio/PagoEfectivo.mp3", // 4: Efecto de sonido al pagar con efectivo
    "recursos/audio/PagoTarjeta.wav", // 5: Efecto de sonido al pagar con tarjeta
    "recursos/audio/InicioMusica.mp3", // 6: Música de inicio
    "recursos/audio/MenuEffect.wav", // 7: Efecto de sonido al navegar hacia arriba
    "recursos/audio/EffectDown.wav", // 8: Efecto de sonido al navegar hacia abajo
    "recursos/audio/EffectUp.wav", // 9: Efecto de sonido al navegar hacia arriba
    "recursos/audio/ScannerSonido.mp3", // 10: Efecto de sonido del escáner
    "recursos/audio/ErrorSonido.mp3", // 11: Efecto de sonido de error
    "recursos/audio/PasoPagina.wav" // 12: Efecto de sonido al pasar página
};

// Arrays de punteros para sonidos y canales
FMOD_SOUND *sonidos[MAX_SLOTS] = {NULL};
FMOD_CHANNEL *canales[MAX_SLOTS] = {NULL};

// Inicializar FMOD
int init_audio() {
    FMOD_RESULT result;
    unsigned int version = 0x00020300;

    result = FMOD_System_Create(&audio_system, version);
    if (result != FMOD_OK) {
        printf("Error al crear sistema FMOD\n");
        return -1;
    }

    result = FMOD_System_Init(audio_system, 512, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK) {
        printf("Error al inicializar FMOD\n");
        return -1;
    }

    return 0;
}

void precargar_sonidos() {
    for (int i = 0; i < sizeof(canciones) / sizeof(canciones[0]); i++) {
        if (sonidos[i])
            FMOD_Sound_Release(sonidos[i]);

        FMOD_RESULT result;
        result = FMOD_System_CreateSound(audio_system, canciones[i], FMOD_DEFAULT, 0, &sonidos[i]);
        if (result != FMOD_OK) {
            printf("Error al precargar: %s\n", canciones[i]);
            continue;
        }

        // Si querés que la música de fondo sea en loop:
        if (i == 0 || i == 1 || i == 2 || i == 3 || i == 6) // las que sean de fondo
            FMOD_Sound_SetMode(sonidos[i], FMOD_LOOP_NORMAL);

        // Reproducimos en su canal
        result = FMOD_System_PlaySound(audio_system, sonidos[i], NULL, 1, &canales[i]); // 1 = start paused
        if (result != FMOD_OK) {
            printf("Error al crear canal para: %s\n", canciones[i]);
            continue;
        }

        // Seteamos volumen inicial en 0
        FMOD_Channel_SetVolume(canales[i], 0.0f);
    }
}


// Actualizar FMOD (debe llamarse en cada iteración del loop)
void actualizar_audio() {
    FMOD_System_Update(audio_system);
}

// Cerrar FMOD y liberar todo
void close_audio() {
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (sonidos[i])
            FMOD_Sound_Release(sonidos[i]);
    }

    FMOD_System_Close(audio_system);
    FMOD_System_Release(audio_system);
}

// Reproduce un sonido en un slot, con loop opcional y volumen deseado
void reproducir_en_slot(int slot, int id, int loop, float volumen) {
    if (slot < 0 || slot >= MAX_SLOTS) {
        printf("Slot inválido\n");
        return;
    }

    if (id < 0 || id >= sizeof(canciones) / sizeof(canciones[0])) {
        printf("ID de sonido inválido\n");
        return;
    }

    if (sonidos[slot])
        FMOD_Sound_Release(sonidos[slot]);

    FMOD_RESULT result;
    result = FMOD_System_CreateSound(audio_system, canciones[id], FMOD_DEFAULT, 0, &sonidos[slot]);
    if (result != FMOD_OK) {
        printf("Error al cargar: %s\n", canciones[id]);
        return;
    }

    if (loop)
        FMOD_Sound_SetMode(sonidos[slot], FMOD_LOOP_NORMAL);

    result = FMOD_System_PlaySound(audio_system, sonidos[slot], NULL, 0, &canales[slot]);
    if (result != FMOD_OK) {
        printf("Error al reproducir: %s\n", canciones[id]);
        return;
    }

    // Setear volumen
    FMOD_Channel_SetVolume(canales[slot], volumen);
}

// Apaga gradualmente el sonido de un slot y lo pausa
void fade_out_pause_slot(int slot, int duracion_ms) {
    if (slot < 0 || slot >= MAX_SLOTS) return;
    if (!canales[slot]) return;

    float volumen;
    FMOD_Channel_GetVolume(canales[slot], &volumen);

    int pasos = 20;
    int delay_por_paso = duracion_ms / pasos;
    float decremento = volumen / pasos;

    for (int i = 0; i < pasos; i++) {
        volumen -= decremento;
        if (volumen < 0.0f) volumen = 0.0f;
        FMOD_Channel_SetVolume(canales[slot], volumen);
        Sleep(delay_por_paso);
    }

    // Pausa en lugar de detener
    FMOD_Channel_SetPaused(canales[slot], 1);
}

void fade_in_slot_existente(int slot, float volumen_final, int duracion_ms) {
    if (slot < 0 || slot >= MAX_SLOTS) return;
    if (!canales[slot]) return;

    FMOD_Channel_SetPaused(canales[slot], 0); // Despausar canal

    int pasos = duracion_ms / 50; // ejemplo: 50ms por paso
    if (pasos <= 0) pasos = 1;

    float paso_volumen = volumen_final / pasos;
    float vol_actual = 0.0f;

    for (int i = 0; i < pasos; i++) {
        FMOD_Channel_SetVolume(canales[slot], vol_actual);
        vol_actual += paso_volumen;
        if (vol_actual > volumen_final) vol_actual = volumen_final;
        Sleep(50);
    }

    // Asegurar volumen final exacto
    FMOD_Channel_SetVolume(canales[slot], volumen_final);
}

/*void fade_in_slot_existente(int slot, int duracion_ms) {
    if (slot < 0 || slot >= MAX_SLOTS) return;
    if (!canales[slot]) return;

    FMOD_BOOL paused;
    FMOD_Channel_GetPaused(canales[slot], &paused);
    if (paused)
        FMOD_Channel_SetPaused(canales[slot], 0); // reanudar

    float volumen = 0.0f;
    int pasos = 20;
    int delay_por_paso = duracion_ms / pasos;
    float incremento = 1.0f / pasos;

    for (int i = 0; i < pasos; i++) {
        volumen += incremento;
        if (volumen > 1.0f) volumen = 1.0f;
        FMOD_Channel_SetVolume(canales[slot], volumen);
        Sleep(delay_por_paso);
    }
}*/

// Reproduce un sonido con fade-in pero en un slot específico recien creado
void reproducir_fade_in_en_slot(int slot, int id, int loop, float volumen_final, int duracion_ms) {
    if (slot < 0 || slot >= MAX_SLOTS) {
        printf("Slot inválido\n");
        return;
    }

    if (id < 0 || id >= sizeof(canciones) / sizeof(canciones[0])) {
        printf("ID de sonido inválido\n");
        return;
    }

    if (sonidos[slot])
        FMOD_Sound_Release(sonidos[slot]);

    FMOD_RESULT result;
    result = FMOD_System_CreateSound(audio_system, canciones[id], FMOD_DEFAULT, 0, &sonidos[slot]);
    if (result != FMOD_OK) {
        printf("Error al cargar: %s\n", canciones[id]);
        return;
    }

    if (loop)
        FMOD_Sound_SetMode(sonidos[slot], FMOD_LOOP_NORMAL);

    result = FMOD_System_PlaySound(audio_system, sonidos[slot], NULL, 0, &canales[slot]);
    if (result != FMOD_OK) {
        printf("Error al reproducir: %s\n", canciones[id]);
        return;
    }

    // Fade in gradual
    float volumen = 0.0f;
    FMOD_Channel_SetVolume(canales[slot], volumen);

    int pasos = 20; // más pasos = transición más suave
    int delay_por_paso = duracion_ms / pasos;
    float incremento = volumen_final / pasos;

    for (int i = 0; i < pasos; i++) {
        volumen += incremento;
        if (volumen > volumen_final) volumen = volumen_final;
        FMOD_Channel_SetVolume(canales[slot], volumen);
        Sleep(delay_por_paso);
    }
}

// Apaga gradualmente el sonido de un slot con fade-out
void fade_out_slot(int slot, int duracion_ms) {
    if (slot < 0 || slot >= MAX_SLOTS) return;
    if (!canales[slot]) return;

    float volumen;
    FMOD_Channel_GetVolume(canales[slot], &volumen);

    int pasos = 20;
    int delay_por_paso = duracion_ms / pasos;
    float decremento = volumen / pasos;

    for (int i = 0; i < pasos; i++) {
        volumen -= decremento;
        if (volumen < 0.0f) volumen = 0.0f;
        FMOD_Channel_SetVolume(canales[slot], volumen);
        Sleep(delay_por_paso);
    }

    FMOD_Channel_Stop(canales[slot]);

    if (sonidos[slot])
        FMOD_Sound_Release(sonidos[slot]);

    sonidos[slot] = NULL;
    canales[slot] = NULL;
}

#endif
