#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOMBRE 20 // Definimos una constante que tenga el maximo de caracteres permitidos en el nombre

// Definicion de la estructura Producto
struct Producto
{ 
    /*Se colocan como campos de la estrcutura toda la informacion que queremos se registre por cada producto, como: */
    char nombre[MAX_NOMBRE];   //El nombre del producto
    int cantidad;              //La cantidad unitaria del prodcuto ingresado   
    float produccion;          //El precio unitario de produccion 
    float venta;               //El precio unitario de venta
    float ganancia;            //La ganancia global     
    char fechaHora[20]; // Cambiamos el tipo a char[] para almacenar la fecha y hora como un string
};

// Funcion para obtener la fecha y hora actual y guardarla como un string
void obtenerFechaHora(char *fechaHora)   /*La funcion recibe como parametro por referencia el puntero hacia el campo que guardara fecha y hora de registro. Esto para darle la
informacion que queremos se guarde en este campo.*/
{
    time_t hora = time(NULL);  /*La declaracion de un tipo de dato como 'time_h' se la realiza para indicar que se guardaran valores de tiempo dentro de la variable. Ademas
    se le asigna a esta variable la funcion 'time(NULL)' para decirle que guardara el numero de segundos que han transcurrido desde el 'Epoch' (0 horas, 0 minutos, 0 segundos,
    del 1 de enero de 1970) hasta la hora y fecha en la que se invoca esta funcion 'time(NULL)' en el codigo.*/
    struct tm *tiempo_completo = localtime(&hora);  /*Ahora declaramos un puntero como un dato tipo 'struct tm' que lo hace esta estrcutura, es guardar valores de tiempo en
    distintos campos desglosados y ordenados, es decir en 'anio', 'mes', 'dia', 'hora', 'minuots', 'segundos'. Y le asignamos el valor que se obtendra con la funcion
    'localtime(&hora)', ya que esta ultima lo que hace es calcular el valor de tiempo asignado a una variable tipo 'time_t' y calcularla como una fecha y hora propia de la zona
    horaria registrada en nuestro sistema.*/
    strftime(fechaHora, 20, "%Y-%m-%d %H:%M:%S", tiempo_completo);  /*Finalmente la funcion 'strftime' que sirve para formatear el 'strcut tm' en una cadena de caracteres con
    formato que se le sea asignado. En este caso formateamos la estrcutura 'tiempo_completo' y la guardaremos en el campo de 'fechaHora' de nuestra estrcutura principal que
    contiene la informacion de prodcutos. El formato en el que es guardado tambien es propio del manejo de fechas y horas, por ejemplo cuando guardamos en '%Y' estamos diciendo
    que guardaremos el año completo con el siglo, ya que no ser asi el año se imprimiria de manera diferente, de igual forma con el resto de componentes horarios.*/
}

// Funcion para imprimir la fecha y hora desde un string
void imprimirFechaHora(const char *fechaHora)
{
    printf("Fecha y Hora: %s\n", fechaHora);
}

// Funcion para ingresar los datos de un producto
void ingresarProducto(struct Producto *producto)
{
    printf("Ingresa el nombre del producto: ");
    scanf("%s", producto->nombre);
    fflush(stdin);

    printf("Ingresa la cantidad de productos: ");
    scanf("%d", &producto->cantidad);
    fflush(stdin);

    printf("Ingresa el precio unitario de produccion: ");
    scanf("%f", &producto->produccion);
    fflush(stdin);

    printf("Ingresa el precio unitario de venta: ");
    scanf("%f", &producto->venta);
    fflush(stdin);

    producto->ganancia = (producto->venta - producto->produccion) * producto->cantidad;

    obtenerFechaHora(producto->fechaHora);
}

// Funcion para agregar un producto al archivo
void agregarProducto(FILE *archivo, const struct Producto *producto)
{
    fprintf(archivo, "%s;%d;%.2f;%.2f;%.2f;%s\n",
            producto->nombre, producto->cantidad, producto->produccion,
            producto->venta, producto->ganancia, producto->fechaHora);
}

// Funcion para buscar un producto por nombre en el archivo
void buscarProducto(const char *nombre)
{
    FILE *archivo = fopen("prod.csv", "r");
    char linea[100];

    if (archivo)
    {

        while (fgets(linea, sizeof(linea), archivo))
        {
            char *token = strtok(linea, ";");
            if (strcmp(nombre, token) == 0)
            {
                printf("Producto encontrado:\n");
                printf("%s;", token);

                token = strtok(NULL, ";");
                printf("%s;", token);

                token = strtok(NULL, ";");
                printf("%s;", token);

                token = strtok(NULL, ";");
                printf("%s;", token);

                token = strtok(NULL, ";");
                printf("%s;", token);

                token = strtok(NULL, "\n");
                imprimirFechaHora(token);
                fclose(archivo);
                return;
            }
        }

        printf("No se encontro el producto.\n");
        fclose(archivo);
    }
    else
    {
        printf("Error al abrir el archivo.\n");
    }
}

// Funcion para eliminar un producto del archivo por linea
void eliminarProducto(int linea)
{
    FILE *archivo = fopen("prod.csv", "r");
    FILE *temporal = fopen("temporal.csv", "w");
    char lineaActual[100];
    int numeroLinea = 1;

    if (archivo && temporal)
    {
        while (fgets(lineaActual, sizeof(lineaActual), archivo))
        {
            if (numeroLinea != linea)
            {
                fprintf(temporal, "%s", lineaActual);
            }
            else
            {
                printf("La linea numero %d: '%s' ha sido eliminada\n", linea, lineaActual);
            }
            numeroLinea++;
        }

        fclose(archivo);
        fclose(temporal);
        remove("prod.csv");
        rename("temporal.csv", "prod.csv");
    }
    else
    {
        printf("Error al abrir los archivos.\n");
    }
}

// Funcion para modificar un producto del archivo por linea
void modificarProducto(int linea)
{
    FILE *archivo = fopen("prod.csv", "r");
    FILE *temporal = fopen("temporal.csv", "w");
    char lineaActual[100];
    char nuevoNombre[MAX_NOMBRE];
    int nuevaCantidad;
    float nuevaProduccion, nuevaVenta;
    int numeroLinea = 1;

    if (archivo && temporal)
    {
        while (fgets(lineaActual, sizeof(lineaActual), archivo))
        {
            if (numeroLinea == linea)
            {
                printf("A continuacion, ingrese los nuevos datos para la modificacion:\n");

                printf("Ingresa el nombre del nuevo producto: ");
                scanf("%s", nuevoNombre);
                fflush(stdin);

                printf("Ingresa la cantidad de elementos del nuevo producto: ");
                scanf("%d", &nuevaCantidad);
                fflush(stdin);

                printf("Ingresa el precio unitario del nuevo producto: ");
                scanf("%f", &nuevaProduccion);
                fflush(stdin);

                printf("Ingresa el precio unitario de venta del nuevo producto: ");
                scanf("%f", &nuevaVenta);
                fflush(stdin);

                struct Producto producto;
                strcpy(producto.nombre, nuevoNombre);
                producto.cantidad = nuevaCantidad;
                producto.produccion = nuevaProduccion;
                producto.venta = nuevaVenta;
                producto.ganancia = (producto.venta - producto.produccion) * producto.cantidad;
                obtenerFechaHora(producto.fechaHora);

                agregarProducto(temporal, &producto);
            }
            else
            {
                fprintf(temporal, "%s", lineaActual);
            }
            numeroLinea++;
        }

        fclose(archivo);
        fclose(temporal);
        remove("prod.csv");
        rename("temporal.csv", "prod.csv");
    }
    else
    {
        printf("Error al abrir los archivos.\n");
    }
}

// Funcion principal
int main()
{
    int opcion;
    int cantimax;
    printf("Antes de iniciar, coloque cuantos productos se ingresaran hoy:\n ");
    scanf("%d", &cantimax);
    struct Producto productos[cantimax];  /*Declaracion de un array tipo strcut donde se guardara informacion de cada uno de los prodcutos ingresados*/
    // Creacion del menu con Switch case
    do
    {
        printf("\n========== Menu Principal ==========\n");
        printf("La informacion que usted busque o añada se dispondra de la sigueinte manera: \n");
        printf("NOMBRE; CANTIDAD; PRECIO UNITARIO; PRECIO VENTA; GANANCIA; HORA  \n");
        printf("--------------------------------------------------\n");
        printf("1. Ingresar productos\n");
        printf("2. Buscar producto\n");
        printf("3. Eliminar producto\n");
        printf("4. Modificar producto\n");
        printf("0. Salir\n");
        printf("=====================================\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            for (int i = 0; i < cantimax; i++)  /*Este for permitira añadir la informacion de cada uno de los productos, la cual sera adjuntada en cada uno de los campos
            definidos en el strcut. Este proceso se repetira, obviamente, considerando la cantidad de productos que se desean registrar*/
            {
                printf("=== Ingrese los datos del Producto %d ===\n", i + 1);
                ingresarProducto(&productos[i]);  /*Se envia como parametro por referencia la direccion de memoria de cada uno de los elementos del array tipo strcut para
                ser rellenados en esta funcion 'IngresarProducto()'*/
                printf("\n");  //Salto de linea para que se imprima el menu separadito xd
            }

            FILE *archivo = fopen("prod.csv", "a");  /*Abrimos el archivo en formato 'a' para añadir informacion*/
            if (archivo) /*Se usa este condicional para garantizar la correcta apertura del archivo, pues puede darse la situacion de que el archivo no se abra de manera
            adecuada. Y de darse esta ultima situacion, y sin plantear esta condicion, estariamos intentando trabajar con un archivo que ni siquiera se abrio. Por esta
            razon, aunque estemos seguros de que el archivo se va a abrir de manera correcta, es una buena practica colocar este tipo de condicionales cuando se trabaja
            con archivos*/
            {
                for (int i = 0; i < cantimax; i++)
                {
                    agregarProducto(archivo, &productos[i]);  /*Se envia como parametro el puntero hacia el archivo y la direccion de memoria de cada uno de los elementos
                    del array tipo strcut ya con la informacion que se le fue asignada en la funcion 'ingresarProdcuto()'. Esta funcion, 'agregarProdcuto()', sirve para
                    colocar la informacion guardada en el array tipo strcut en el archivo al que se esta apuntando, es decir 'prod.csv'*/
                }
                fclose(archivo);  //Una vez la funcion guarde la informacion del array tipo strcut en el archivo, se cierra en esta parte del main principal.
            }
            else
            {
                printf("Error al abrir el archivo.\n");  /*De darse el caso en el que el archivo no pueda ser abierta, se devolvera como respuesta un 'NULL' a la condicion
                de apertura, pues no se esta apuntando de manera valida al archivo*/
            }
            break;

        case 2:
        {
            char nombreBusqueda[MAX_NOMBRE];  //Se declara un string que servira para guardar el nombre del prodcuto que se desea buscar
            printf("Ingrese el nombre del producto que desea buscar: "); 
            scanf("%s", nombreBusqueda);  /*Se leera el dato de entrada tipo string que sera guardado en el string previamente declarado. No se coloca el operador '&' ya que
            al colocar arrays que guardaran un string como argumento de la funcion 'scanf()', automaticamente estos arrays se convierten en punteros que almacenaran el string
            leido.*/
            buscarProducto(nombreBusqueda);  /*Se envia como parametro por referencia por referencia al puntero al string recientemento leido, para que sea empleado en esta
            funcion 'buscarProdcuto()'*/
        }
        break;

        case 3:
        {
            int lineaEliminar;  //Se declara una variable que contendra un tipo de dato entero
            printf("Ingrese el numero de linea quedesea eliminar: ");  
            scanf("%d", &lineaEliminar);  /*Se le pide al usuario ingresar un numero que sera guardado en la variable recien declarada, este numero ingresado hace referencia
            al numero de fila contenida en el archivo, la cual se desea eliminar*/
            eliminarProducto(lineaEliminar);  /*Se envia este numero como parametro por valor, el cual sera empleado para dar lugar a la funcion 'eliminarProducto()'*/
        }
        break;

        case 4:
        {
            int lineaModificar;  //Se declara una nueva variable que contendra un tipo de dato entero
            printf("Ingrese el numero de linea que desea modificar: ");  
            scanf("%d", &lineaModificar);  /*Se leera un numero entero que sera guardado en la variable recien declarada, este numero a ser ingresado hace referencia al numero
            de fila en el archivo que se desea modificar*/
            modificarProducto(lineaModificar);  //Se envia este numero como parametro por valor para ser empleado en esta funcion 'modificarProdcuto()'
        }
        break;

        case 0:  //Case que da fin al bucle del menu del programa 
            printf("Saliendo del programa...\n");  
            break;

        default:   //Case 'default' que se dara lugar en caso de no ingresar ninguno de los anteriores case validos. 
            printf("Opcion invalida. Intente de nuevo.\n");
            break;
        }
    } while (opcion != 0);  //El bucle que da lugar al menu pues se repetira hasta ingresar el 0 como opcion del switch case.

    return 0;
}
