//Importación de librerías
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//Importación de la función volver
void volver(char *tecla);

//Estructura de cada Producto
typedef struct Productos Productos;
struct Productos
{
    char nombre[30];
    int cantidad;
    float precio1;
    float precio2;
    float ganan;
    int hora;
    int minuto;
};


int main()
{
    //Declaración de variables
    int opcion_usuario;
    int cantidad_productos;
    char nombre_proveedor[30];
    int ancho_nombre;
    int cantidad;
    float precio_unitario;
    float precio_venta;
    int borrar_producto;
    FILE *archivo;
    char linea[100];
    int contador;
    time_t tiempo_actual = time(NULL);
    struct tm *tiempo_descompuesto = localtime(&tiempo_actual);
    char tecla;

    do
    {
        //Impresión del menú
        printf("\n-----------------MENU-------------\n");
        printf("1. Ver productos\n");
        printf("2. Registrar productos\n");
        printf("3. Borrar productos\n");
        printf("4. Salir del inventario\n");
        printf("------------------------------------\n");
        printf("Por favor seleccione una opcion: ");
        scanf("%d", &opcion_usuario);

        switch (opcion_usuario)
        {
        case 1:
            archivo = fopen("productos.csv", "r");
            if (archivo)
            {
                printf("\n-------------- BODEGA --------------------------\n");
                printf("La informacion se dispondra de la sigueinte manera: \n");
                printf("NOMBRE;CANTIDAD;PRECIO UNITARIO;PRECIO VENTA;GANANCIA;HORA  \n");
                printf("--------------------------------------------------\n");
                //Se obtiene cada línea de archivo de texto
                while (fgets(linea, sizeof(linea), archivo) != NULL)
                {
                    //En caso de que haya un salto de línea al final de cada línea del código...
                    if (linea[strlen(linea) - 1] == '\n')
                    {
                        /*Esto es un poco raro, en teoría debería ser solo un =, pero si no pongo el doble igual
                        no se imprime correctamente las líneas del archivo*/
                        linea[strlen(linea) - 1] == '\0';
                    }
                    printf("%s", linea);
                    contador++;
                }
            }
            printf("----------------------------------------------------------\n");

            fclose(archivo);

            volver(&tecla);
            break;

        case 2:
            printf("Por favor ingrese la cantidad de prudctos a registrar: ");
            scanf("%d", &cantidad_productos);

            /*Se asigna de forma dinámica en memoria los productos que ingrese el usuario, esto debido a que no
            se sabe cuantos productos ingresará al final (no hay límite)*/
            Productos *lista = malloc(cantidad_productos * sizeof(Productos));

            for(int i = 0; i < cantidad_productos; i++)
            {
                printf("Ingrese el nombre del consumidor: ");
                scanf("%s", nombre_proveedor);

                printf("Ingrese la cantidad de elementos del producto: ");
                scanf("%d", &cantidad);

                printf("Ingrese el precio unitario de compra/produccion del prodcuto: ");
                scanf("%f", &precio_unitario);

                printf("Ingrese el precio de venta del producto: ");
                scanf("%f", &precio_venta);
                printf("\n");

                int hora_actual = tiempo_descompuesto->tm_hour;
                int minuto_actual = tiempo_descompuesto->tm_min;
                
                //Se guardan los productos en el struct
                strcpy(lista[i].nombre, nombre_proveedor);
                lista[i].cantidad = cantidad;
                lista[i].precio1 = precio_unitario;
                lista[i].precio2 = precio_venta;
                lista[i].hora = hora_actual;
                lista[i].minuto = minuto_actual;
            }
            
            /*Esto se hace para que a la hora de la impresión, los nombres se guarden de una manera que no haga
            que se vea desfazado el resto de la información del producto (cantidad, precio, etc)*/
            int ancho_nombre = 0;
            for (int i = 0; i < cantidad_productos; i++)
            {
                int longitud = strlen(lista[i].nombre);
                if (longitud > ancho_nombre)
                {
                    ancho_nombre = longitud;
                }
            }

            /*Se guarda la información del struct en el archivo productos.csv, estos están guardados de una forma
            en que, a la hora de imprimirlos, se vean correctamente alineados*/
            archivo = fopen("productos.csv", "a");
            for(int i = 0; i < cantidad_productos; i++)
            {
                fprintf(archivo, "%s;",lista[i].nombre);
                fprintf(archivo, "%d;", lista[i].cantidad);
                fprintf(archivo, "%.2f;", lista[i].precio1);
                fprintf(archivo, "%.2f;", lista[i].precio2);
                fprintf(archivo, "%.2f;", lista[i].precio2 - lista[i].precio1);
                fprintf(archivo, "%d:", lista[i].hora);
                fprintf(archivo, "%d", lista[i].minuto);
                fprintf(archivo, "\n");
            }
            fclose(archivo);
            free(lista);
            volver(&tecla);
            break;
        
        case 3:
            printf("Por favor ingrese el número de línea que desea borrar: ");
            scanf("%d", &borrar_producto);

            archivo = fopen("productos.csv", "r");
            if (archivo)
            {
                //Se crea un archivo temporal para almacenar ahí el archivo original, pero con la línea que se
                //quiere borrar
                FILE *temporal = fopen("temporal.csv", "w");
                if (temporal)
                {
                    int numero_linea = 1;

                    //Se lee cada línea del archivo original
                    while (fgets(linea, sizeof(linea), archivo) != NULL)
                    {
                        //En caso de que no sea la línea que se desea borrar, esta se copia al archivo temporal
                        if (numero_linea != borrar_producto)
                        {
                            fprintf(temporal, "%s", linea);
                        }
                        numero_linea++;
                    }

                    fclose(temporal);
                    fclose(archivo);

                    //Se reemplaza el archivo orginal con el temporal
                    if (remove("productos.csv") == 0)   //Se elimina el archivo original de productos.csv
                    {
                        //Se cambia el nombre de "temporal.csv" por "productos.csv"
                        if (rename("temporal.csv", "productos.csv") == 0)
                        {
                            printf("Línea borrada exitosamente.\n");
                        }
                        else
                        {
                            printf("Error al renombrar el archivo temporal.\n");
                        }
                    }
                    else
                    {
                        printf("Error al eliminar el archivo original.\n");
                    }
                }
                else
                {
                    printf("Error al abrir el archivo temporal.\n");
                }
            }
            else
            {
                printf("Error al abrir el archivo original.\n");
            }
            volver(&tecla);
            break;

        case 4:
            printf("\nVuelve pronto!\n");
            break;

        default:
            printf("Opcion no valida, intenta otra vez\n\n");
            volver(&tecla);
            break;
        }
    } while (opcion_usuario != 4);

    return 0;
}


void volver(char *tecla)
{
    do
    {
        // Método para esperar la respuesta del usuario
        printf("Presiona la tecla 'v' para volver: ");
        scanf(" %c", tecla);

        // Comprobación de si la tecla es distinta a una letra del alfabeto
        if (!(isalpha(*tecla)))
        {
            printf("No es una letra. Intenta otra vez\n\n");
        }
        // Comprobación de si la tecla es distinta a la letra 'v'
        else if (!(*tecla == 'v' || *tecla == 'V'))
        {
            printf("No es la letra 'v'. Intenta otra vez\n\n");
        }
        // Se vuelve al menú principal y se limpia la consola
        else
        {
            system("cls");
            break;
        }
    } while (*tecla != 'v' || *tecla != 'V');
}