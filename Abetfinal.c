#include<stdio.h>
#include<string.h>
struct datos{
    char nombre[20];
    int cantidad;
    float produccion;
    float venta;
    float ganancia;
};
int main(){
    struct datos p[2];
    char nom[20];
    int cant;
    float prod, vent, gana[2];
    FILE *archivo, *temporal;
    for(int i=0; i<2; i++){
        printf("Ingresa el nombre del produccto: ");
        scanf("%s",&nom);
        printf("Ingresa la cantidad de productos: ");
        scanf("%d",&cant);
        printf("Ingresa el precio unitario de produccion: ");
        scanf("%f",&prod);
        printf("Ingresa el precio unitario de venta: ");
        scanf("%f",&vent);
        gana[i] = ((vent - prod)*cant);
        strcpy(p[i].nombre, nom);
        p[i].cantidad = cant;
        p[i].produccion = prod;
        p[i].venta = vent;
        p[i].ganancia = gana[i];
    }
    archivo = fopen("productos.csv", "r");
    for(int i = 0; i < 2; i++){
        fprintf(archivo, "%s;",p[i].nombre);
        fprintf(archivo, "%d;", p[i].cantidad);
        fprintf(archivo, "%.2f;", p[i].produccion);
        fprintf(archivo, "%.2f;", p[i].venta);
        fprintf(archivo, "%.2f;", p[i].ganancia);
        fprintf(archivo, "\n");
    }
    return 0;
}