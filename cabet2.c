#include<stdio.h>
#include<string.h>
struct datos{
    char nom[20];
    int canti;
    float produ;
    float vent;
};
int main(){
    struct datos p[2];
    float ganan[2];
    char crc;
    char prodelegi[20], nombusq[20];
    int cantibusq;
    float prodbusq, ventbusq, gananbusq;
    long posci;
    char nomborr[20];
    int cantborr;
    float prodborr, ventborr, gananborr;  
    for(int i=0; i<2; i++){
        printf("Nombre del producto: ");
        gets(p[i].nom);
        fflush(stdin);
        printf("Cantidad de productos: ");
        scanf("%d",&p[i].canti);
        fflush(stdin);
        printf("Precio de produccion: ");
        scanf("%f",&p[i].produ);
        fflush(stdin);
        printf("Precio de venta: ");
        scanf("%f",&p[i].vent);
        fflush(stdin);
        ganan[i] = ((p[i].vent - p[i].produ)*p[i].canti);
    }
    FILE *archivo, *temp;
    archivo = fopen("datos.csv","w");
    for(int i=0; i<2; i++){
        fprintf(archivo,"%s; %d; %.2f; %.2f; %.2f\n", p[i].nom, p[i].canti, p[i].produ, p[i].vent, ganan[i]);
    }
    fclose(archivo);

    /*Leer archivo
    archivo = fopen("datos.csv","r");
    while (!feof(archivo))
    {
        fscanf(archivo,"%[^;];%d;%f;%f;%f\n",nombusq,&cantibusq,&prodbusq,&ventbusq,&gananbusq);
        printf("%s %d %.2f %.2f %.2f\n",nombusq,cantibusq,prodbusq,ventbusq,gananbusq);
    }
    fclose(archivo);*/

    //identificar filas:
    printf("Ingresa el producto que deseas editar:\n");
    scanf("%s",&prodelegi);
    fflush(stdin);
    archivo=fopen("datos.csv","r");
    while(!feof(archivo)){
        posci = ftell(archivo);
        fscanf(archivo,"%[^;];%d;%f;%f;%f\n",&nombusq,&cantibusq,&prodbusq,&ventbusq,&gananbusq);
        if(strcmp(nombusq,prodelegi)==0){
            printf("Los datos escogidos son: %s; %d; %.2f; %.2f; %.2f", nombusq,cantibusq,prodbusq,ventbusq,gananbusq);
            break;
        }
    }
    fclose(archivo);

    //Se puede implementar un switch para escoger la opcion de borrado o actualizado:

    //Borrado:
    archivo = fopen("datos.csv","r+");
    temp = fopen("temp.csv","w");
    fseek(archivo,posci,0);
    ;
    while(fscanf(archivo,"%[^;];%d;%f;%f;%f\n",&nomborr,&cantborr,&prodborr,&ventborr,&gananborr)){
        if(nomborr!=){
            
        }

    }
    return 0;
}