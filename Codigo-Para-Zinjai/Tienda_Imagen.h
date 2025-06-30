#ifndef TIENDA_IMAGEN_H
#define TIENDA_IMAGEN_H

#include <windows.h>
#include <stdio.h>

// Variable global para mantener el proceso de la imagen
static PROCESS_INFORMATION proceso_imagen = {0};
static BOOL imagen_abierta = FALSE;

// Función para el thread que fuerza el foco
DWORD WINAPI ForzarFocoThread(LPVOID lpParam) {
    Sleep(300); // Esperar a que se abra la imagen
    
    HWND hwndConsole = GetConsoleWindow();
    if (hwndConsole) {
        // Estrategia súper agresiva
        for (int i = 0; i < 10; i++) {
            // Hacer la consola temporalmente siempre visible
            SetWindowPos(hwndConsole, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            Sleep(10);
            
            // Forzar activación
            ShowWindow(hwndConsole, SW_RESTORE);
            ShowWindow(hwndConsole, SW_SHOW);
            BringWindowToTop(hwndConsole);
            SetForegroundWindow(hwndConsole);
            SetActiveWindow(hwndConsole);
            SetFocus(hwndConsole);
            
            Sleep(50);
        }
        
        // Quitar el modo topmost
        SetWindowPos(hwndConsole, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    
    return 0;
}

void MostrarImagen(int id) {
    // Cerrar imagen anterior si existe
    if (imagen_abierta) {
        TerminateProcess(proceso_imagen.hProcess, 0);
        CloseHandle(proceso_imagen.hProcess);
        CloseHandle(proceso_imagen.hThread);
        imagen_abierta = FALSE;
    }

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOWNOACTIVATE; // Abrir sin activar la ventana
    ZeroMemory(&proceso_imagen, sizeof(proceso_imagen));

    // Comando para abrir la imagen con el visor por defecto de Windows
    char comando[512];
    sprintf(comando, "rundll32.exe \"C:\\Program Files\\Windows Photo Viewer\\PhotoViewer.dll\", ImageView_Fullscreen C:\\Proyecto\\recursos\\imagenes\\%d.jpg", id);

    // Crear proceso
    if (!CreateProcess(
            NULL, comando, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &proceso_imagen)) {
        // Si falla, intentar con el comando start de Windows
        sprintf(comando, "cmd /c start \"\" \"C:\\Proyecto\\recursos\\imagenes\\%d.jpg\"", id);
        if (!CreateProcess(
                NULL, comando, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &proceso_imagen)) {
            printf("No se pudo abrir la imagen.\n");
            return;
        }
    }

    imagen_abierta = TRUE;
    
    // Crear un thread que fuerce el foco después de un delay
    HANDLE hThread = CreateThread(NULL, 0, ForzarFocoThread, NULL, 0, NULL);
    if (hThread) {
        CloseHandle(hThread); // No necesitamos esperar al thread
    }
}

void CerrarImagen() {
    if (imagen_abierta) {
        TerminateProcess(proceso_imagen.hProcess, 0);
        CloseHandle(proceso_imagen.hProcess);
        CloseHandle(proceso_imagen.hThread);
        imagen_abierta = FALSE;
    }
}

#endif // TIENDA_IMAGEN_H
