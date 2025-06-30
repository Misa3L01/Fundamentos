#ifndef TIENDA_ESTRUCTURAS_H
#define TIENDA_ESTRUCTURAS_H

#define N 50

typedef struct Producto {
    char Nombre[N];
    char Categoria[N];
    char Subcategoria[N];
    int stock;
    int ID;
    float Precio;
    struct Producto *left;
    struct Producto *right;
} Producto;

typedef struct Cliente {
    char nombre[50];
    char usuario[30];
    char contrasena[30];
    struct Cliente *sig;
} Cliente;

typedef struct Cajero {
    char nombre[50];
    char contrasena[30];
    struct Cajero *sig;
} Cajero;

typedef struct Customer{
    char Nombre[50];
    struct Customer *next;
} Customer;

#endif // TIENDA_ESTRUCTURAS_H
