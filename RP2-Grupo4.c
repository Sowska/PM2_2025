/*
ANALISIS DE LOS RESULTADOS:

RAL: El costo maximo elevado indica que varias claves distintas tienen el mismo hash, lo que empeora el rendimiento porque varias claves caen cerca entre sí.
 El alto costo de evocaciones no exitosas es otra desventaja. El peor escenario de un RAL es bastante costoso, especialmente para un factor de carga alto.

RAC: el costo medio y maximo de evocaciones exitosas es menor que RAL, lo que indica que la propiedad cuadratica del rebalse es mas eficiente para dispersar los elementos.
El costo maximo en el fracaso es mayor que el de RAL, ya que el RAC puede hacer saltos erroneos mas grandes, lo cual afecta negativamente el costo en busqueda fallida.

RS: El esfuerzo maximo de localizacion exitosa muestra el mejor resultado para esta estructura, teniendo en cuenta que este esfuerzo se obtiene al consultar el ultimo elemento
de la lista mas larga. El medio de evocacion exitosa y no exitosa indica una alta eficiencia del rebalse para distribuir los elementos manteniendo listas relativamente cortas,
localizando elementos rapidamente y minimizando el esfuerzo en consultas fallidas.

Conclusion:

El rebalse separado es la estructura mas eficiente tanto para evocaciones exitosas como no exitosas, permitiendo un acceso eficiente a los elementos.

----Costos del REBALSE ABIERTO LINEAL:----

----Evocacion exitosa----
Costo total de evocacion exitosa: 1432
Costo maximo de evocacion exitosa: 97
Costo medio de evocacion exitosa: 3.35

----Evocacion NO exitosa----
Costo total de evocacion NO exitosa: 36813
Costo maximo de evocacion NO exitosa: 119
Costo medio de evocacion NO exitosa: 107.01

----Costos del REBALSE ABIERTO CUADRATICO:----

----Evocacion exitosa----
Costo total de evocacion exitosa: 948
Costo maximo de evocacion exitosa: 34
Costo medio de evocacion exitosa: 2.20

----Evocacion NO exitosa----
Costo de total evocacion NO exitosa: 7246
Costo maximo de evocacion NO exitosa: 127
Costo medio de evocacion exitosa: 21.19

------------------------

----Costos del REBALSE SEPARADO:----

----Evocacion exitosa----
Costo total de evocacion exitosa: 766
Costo maximo de evocacion exitosa: 5
Costo medio de evocacion exitosa: 1.83

----Evocacion NO exitosa----
Costo total de evocacion NO exitosa: 533
Costo maximo de evocacion NO exitosa: 5
Costo medio de evocacion NO exitosa: 1.51*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cant_prestadores 110

typedef struct
{
    int dni;
    char nombre[80];
    char servicios[120];
    char domicilio[80];
    char email[50];
    char telefono[30];
} Prestador;

typedef struct
{
    int M_RAC;
    int cantRAC;
    int M_RAL;
    int cantRAL;
    int M_RS;
    int cantRS;
} Informacion;
typedef struct Nodo
{
    Prestador p;
    struct Nodo *siguiente;
} Nodo;
typedef struct
{
    Nodo lista[45];
    Nodo *pos;
    Nodo *anterior;
} TablaRS;
typedef struct
{
    int costoRS[2], costoRAL[2], costoRAC[2];
    int maxRS[2], maxRAL[2], maxRAC[2];
    int cantEvoRS[2], cantEvoRAL[2], cantEvoRAC[2];
} Costos;

int hashing(int dni, int M)
{
    char x[10];
    int longitud, i;
    int contador = 0;
    sprintf(x, "%d", dni);
    longitud = strlen(x);
    for (i = 0; i < longitud; i++)
    {
        contador += ((int)x[i]) * (i + 1);
    }
    return (contador % M);
}

void inicializar_RS(TablaRS *RS, int info)
{

    for (int i = 0; i < info; i++)
    {//dos cursores, el que queda como anterior va haciendo el free de todas las lista
        RS->lista[i].p.dni = -1;
        RS->lista[i].siguiente = NULL;
    }
    RS->pos = NULL;
    RS->anterior = NULL;
}

void limpiarLista(Nodo *inicio) {
    Nodo *actual = inicio;
    Nodo *sig;

    while (actual != NULL) {
        sig = actual->siguiente;
        free(actual);
        actual = sig;
    }
}

void reiniciar_RS(TablaRS *RS, int info) {
    for (int i = 0; i < info; i++) {

        if (RS->lista[i].siguiente != NULL) {
            limpiarLista(RS->lista[i].siguiente);
        }
        RS->lista[i].p.dni = -1;
        RS->lista[i].siguiente = NULL;
    }
    RS->pos = NULL;
    RS->anterior = NULL;
}


void inicializar_costos(Costos *costo)
{
    //---------------------RS----------------
    costo->cantEvoRS[0] = 0;
    costo->cantEvoRS[1] = 0;
    costo->costoRS[0] = 0;
    costo->costoRS[1] = 0;
    costo->maxRS[0] = 0;
    costo->maxRS[1] = 0;
    //---------------------RAL----------------
    costo->cantEvoRAL[0] = 0;
    costo->cantEvoRAL[1] = 0;
    costo->costoRAL[0] = 0;
    costo->costoRAL[1] = 0;
    costo->maxRAL[0] = 0;
    costo->maxRAL[1] = 0;
    //---------------------RAC----------------
    costo->cantEvoRAC[0] = 0;
    costo->cantEvoRAC[1] = 0;
    costo->costoRAC[0] = 0;
    costo->costoRAC[1] = 0;
    costo->maxRAC[0] = 0;
    costo->maxRAC[1] = 0;
}

void mostrarCostos(Informacion info, Costos *costo)
{
    printf("\n----Costos del REBALSE ABIERTO LINEAL:----\n");
    printf("\n----Evocacion exitosa----\n");
    if (costo->cantEvoRAL[0] > 0)
    {
        printf("Costo total de evocacion exitosa: %d\n", costo->costoRAL[0]);
        printf("Costo maximo de evocacion exitosa: %d\n", costo->maxRAL[0]);
        printf("Costo medio de evocacion exitosa: %.2f\n", ((float)(costo->costoRAL[0])) / costo->cantEvoRAL[0]);
    }
    else
    {
        printf("\nNo hubo evocaciones exitosas\n");
    }
    printf("\n----Evocacion NO exitosa----\n");
    if (costo->cantEvoRAL[1] > 0)
    {
        printf("Costo total de evocacion NO exitosa: %d\n", costo->costoRAL[1]);
        printf("Costo maximo de evocacion NO exitosa: %d\n", costo->maxRAL[1]);
        printf("Costo medio de evocacion NO exitosa: %.2f\n", ((float)(costo->costoRAL[1])) / costo->cantEvoRAL[1]);
    }
    else
    {
        printf("\nNo hubo evocaciones NO exitosas\n");
    }
    //-------------------------------------------------------------------------------------------------------------------
    printf("\n----Costos del REBALSE ABIERTO CUADRATICO:----\n");
    printf("\n----Evocacion exitosa----\n");
    if (costo->cantEvoRAC[0] > 0)
    {
        printf("Costo total de evocacion exitosa: %d\n", costo->costoRAC[0]);
        printf("Costo maximo de evocacion exitosa: %d\n", costo->maxRAC[0]);
        printf("Costo medio de evocacion exitosa: %.2f\n", ((float)(costo->costoRAC[0])) / costo->cantEvoRAC[0]);
    }
    else
    {
        printf("\nNo hubo evocaciones exitosas\n");
    }
    printf("\n----Evocacion NO exitosa----\n");
    if (costo->cantEvoRAC[1] > 0)
    {
        printf("Costo de total evocacion NO exitosa: %d\n", costo->costoRAC[1]);
        printf("Costo maximo de evocacion NO exitosa: %d\n", costo->maxRAC[1]);
        printf("Costo medio de evocacion exitosa: %.2f\n", ((float)(costo->costoRAC[1])) / costo->cantEvoRAC[1]);
    }
    else
    {
        printf("\nNo hubo evocaciones NO exitosas\n");
    }
    printf("\n------------------------\n");

    printf("\n----Costos del REBALSE SEPARADO:----\n");
    printf("\n----Evocacion exitosa----\n");
    if (costo->cantEvoRS[0] > 0)
    {
        printf("Costo total de evocacion exitosa: %d\n", costo->costoRS[0]);
        printf("Costo maximo de evocacion exitosa: %d\n", costo->maxRS[0]);
        printf("Costo medio de evocacion exitosa: %.2f\n", ((float)(costo->costoRS[0]) / costo->cantEvoRS[0]));
    }
    else
    {
        printf("No hubo evocaciones exitosas\n");
    }
    printf("\n----Evocacion NO exitosa----\n");
    if (costo->cantEvoRS[1] > 0)
    {
        printf("Costo total de evocacion NO exitosa: %d\n", costo->costoRS[1]);
        printf("Costo maximo de evocacion NO exitosa: %d\n", costo->maxRS[1]);
        printf("Costo medio de evocacion NO exitosa: %.2f\n", ((float)(costo->costoRS[1]) / costo->cantEvoRS[1]));
    }
    else
    {
        printf("\nNo hubo evocaciones NO exitosas\n");
    }
    //-------------------------------------------------------------------------------------------------------------------
}
int sonDiferentes(Prestador p1, Prestador p2)
{

    int nombre = strcmp(p1.nombre, p2.nombre);
    int servicios = strcmp(p1.servicios, p2.servicios);
    int domicilio = strcmp(p1.domicilio, p2.domicilio);
    int email = strcmp(p1.email, p2.email);
    int telefono = strcmp(p1.telefono, p2.telefono);

    if (nombre || servicios || domicilio || email || telefono)
    {

        return 1;
    }

    return 0;
}

/*OPERACIONES DEL RS*/
int localizarRS(TablaRS *RS, int dni, int info, int *i, int *costo)
{
    (*costo) = 0;
    *i = hashing(dni, info);
    RS->pos = &(RS->lista[*i]);
    RS->anterior = NULL;

    while (RS->pos != NULL)
    {
        (*costo)++;

        if (RS->pos->p.dni == dni)
        {
            return 1; // Prestador encontrado
        }

        RS->anterior = RS->pos;
        RS->pos = RS->pos->siguiente;
    }

    return 0; // No encontrado
}

int altaRS(TablaRS *RS, Prestador p, int inf)
{
    int costo = 0;
    int i = 0;
    if (localizarRS(RS, p.dni, inf, &i, &costo) == 1)
    {
        return 0; // Elemento encontrado
    }
    else
    {

        if (RS->lista[i].p.dni == -1)
        {
            RS->lista[i].p = p;
            return 1;
        }
        Nodo *nodo = (Nodo *)malloc(sizeof(Nodo));
        if (nodo != NULL)
        {
            nodo->p = p;
            nodo->siguiente = RS->lista[i].siguiente;
            RS->lista[i].siguiente = nodo;
            return 1; // Alta exitosa
        }
        else
        {
            return -1; // No hay espacio
        }
    }
}

int bajaRS(TablaRS *RS, Prestador p, int inf)
{
    int costo = 0;
    int i = 0;
    if (localizarRS(RS, p.dni, inf, &i, &costo) == 0)
    {
        return 0; // El elemento no esta en la tabla
    }
    else if (RS->pos->p.dni == -1)
    {
        return 0; // El elemento no esta en la tabla
    }
    else
    {
        //
        if (sonDiferentes(p, RS->pos->p) == 1)
        {
            return -1; // Los prestadores no son iguales
        }
        else
        {

            if (RS->pos != &(RS->lista[i]))
            {
                RS->anterior->siguiente = RS->pos->siguiente;
                free(RS->pos);
                RS->pos = NULL;
            }
            else // Caso donde estamos en la cabecera
            {
                if (RS->pos->siguiente != NULL)
                {
                    Nodo *temp = RS->pos->siguiente;
                    RS->pos->p = RS->pos->siguiente->p;
                    RS->pos->siguiente = RS->pos->siguiente->siguiente;
                    free(temp);
                }
                else
                {
                    RS->pos->p.dni = -1;
                    return 1;
                }
            }
            return 1; // Eliminacion exitosa
        }
    }
}

Prestador evocarRS(TablaRS *RS, int dni, int inf, int *exito, int *costo)
{
    (*costo) = 0;
    int i;
    Prestador aux;
    if (localizarRS(&*RS, dni, inf, &i, costo) == 1)
    {
        *exito = 1;
        aux = RS->pos->p;
        return aux;
    }
    else
    {
        *exito = 0;
        aux.dni = -1;
        return aux;
    }
}

void mostrarRS(TablaRS *RS, int info)
{
    int i = 0;
    printf("\n----Rebalse Separado----\n");
    for (i = 0; i < info; i++)
    {
        printf("\nLista %d:\n", i);
        RS->pos = &(RS->lista[i]);
        while (RS->pos != NULL)
        {

            switch (RS->pos->p.dni)
            {
            case 0:
                printf("Celda libre\n");
                printf("\n------------*------------\n");
                break;

            case -1:
                printf("Lista vacia\n");
                getchar();
                break;
            default:
                printf("\nDNI: %d", RS->pos->p.dni);
                printf("\nNombre: %s", RS->pos->p.nombre);
                printf("\nServicios: %s", RS->pos->p.servicios);
                printf("\nDomicilio: %s", RS->pos->p.domicilio);
                printf("\nCorreo electronico: %s", RS->pos->p.email);
                printf("\nNumero de telefono: %s \n", RS->pos->p.telefono);
                printf("\n------------*------------\n");
                break;
            }
            RS->pos = RS->pos->siguiente;
        }
    }
}

/*OPERACIONES DEL RAL*/

void localizarRAL(Prestador RAL[], int dni, int M, int *i, int *costo, int *exito)
{
    int intentos = 0;
    int primerLugarVacio = -2;
    *i = hashing(dni, M);
    (*costo) = 0;

    while (intentos < M && RAL[*i].dni != -1 && RAL[*i].dni != dni)
    {

        // guardar primer lugar vacio
        if (primerLugarVacio == -2 && RAL[*i].dni == 0)
        {
            primerLugarVacio = *i;
        }

        *i = (*i + 1) % M;
        intentos++;
        (*costo)++;
    }

    // caso realizo menos de M intentos
    if (intentos < M)
    {
        (*costo)++;
    }

    if (RAL[*i].dni == dni)
    {
        *exito = 1; // Prestador encontrado
    }
    else
    {
        if (primerLugarVacio != -2)
        {
            *i = primerLugarVacio;
            *exito = 2;
        }
        else if (RAL[*i].dni == -1)
        {
            *exito = 0;
        }
        else
        {
            *exito = -1;
        }
    }
}

int altaRAL(Prestador RAL[], Prestador p, int M)
{
    int i = 0, exito = 0, costo = 0;
    localizarRAL(RAL, p.dni, M, &i, &costo, &exito);
    if (exito == 1)
    {
        return 0; // El prestador ya existe
    }
    if (exito == -1)
    {
        return -1; // No hay lugar disponible
    }
    // Alta del prestador
    RAL[i] = p;
    return 1; // Alta exitosa
}

int bajaRAL(Prestador RAL[], Prestador p, int M)
{
    int i = 0, exito = 0, costo = 0;
    localizarRAL(RAL, p.dni, M, &i, &costo, &exito);
    if (exito == 1)
    {
        if (sonDiferentes(p, RAL[i]) == 1)
        {
            return -1; // Los prestadores no son iguales
        }
        else
        {
            RAL[i].dni = 0;
            return 1;
        } // baja exitosa
    }
    else
    {
        return 0; // baja fracasada
    }
}

Prestador evocarRAL(Prestador RAL[], int dni, int M, int *exito, int *costo)
{
    (*costo) = 0;
    int i = 0, exi;
    Prestador aux;
    localizarRAL(RAL, dni, M, &i, costo, &exi);
    if (exi == 1)
    {
        aux = RAL[i];
        *exito = 1;
        return aux;
    }
    else
    {
        *exito = 0;
        return aux;
    }
}

void mostrarRAL(Prestador RAL[], int M)
{
    int i, aux = 0;
    printf("\n----Rebalse Abierto Lineal----\n");
    for (i = 0; i < M; i++)
    {
        if ((aux % 5) == 0)
        {
            getchar();
        }
        if (RAL[i].dni == -1)
        {
            printf("\nDatos de la posicion %d:\n", i);
            printf(" -Posicion virgen\n");
        }
        else if (RAL[i].dni == 0)
        {
            printf("\nDatos de la posicion %d:\n", i);
            printf(" -Posicion libre\n");
        }
        else
        {
            printf("\n------------*------------\n");
            printf("\nDatos de la posicion %d:\n", i);
            printf("\nDNI: %d", RAL[i].dni);
            printf("\nNombre: %s", RAL[i].nombre);
            printf("\nServicios: %s", RAL[i].servicios);
            printf("\nDomicilio: %s", RAL[i].domicilio);
            printf("\nCorreo electronico: %s", RAL[i].email);
            printf("\nNumero de telefono: %s \n", RAL[i].telefono);
            printf("\n------------*------------\n");
        }
        aux++;
    }
}

/*OPERACIONES DEL RAC*/
void localizarRAC(Prestador RAC[], int dni, int M, int *i, int *costo, int *exito)
{
    int intentos = 0;
    int primerLugarVacio = -2;
    *i = hashing(dni, M);
    int k = 1;
    (*costo) = 0;

    while (intentos < M && RAC[*i].dni != -1 && RAC[*i].dni != dni)
    {
        (*costo)++;

        if (primerLugarVacio == -2 && RAC[*i].dni == 0)
        {
            primerLugarVacio = *i;
        }

        *i = (*i + k) % M;
        k++;
        intentos++;
    }

    if (intentos < M)
    {
        (*costo)++;
    }

    if (RAC[*i].dni == dni)
    {
        *exito = 1; // Prestador encontrado
    }
    else
    {
        if (primerLugarVacio != -2)
        {
            *i = primerLugarVacio;
            *exito = 2; // No encontrado, hay lugar libre para dar alta
        }
        else if (RAC[*i].dni == -1)
        {
            *exito = 0; // No encontrado, pero hay lugar
        }
        else
        {
            *exito = -1; // No encontrado y no hay espacio
        }
    }
}

int altaRAC(Prestador RAC[], Prestador p, int M)
{
    int i = 0, exito = 0, costo = 0;
    localizarRAC(RAC, p.dni, M, &i, &costo, &exito);
    if (exito == 1)
    {
        return 0; // El prestador ya existe
    }
    if (exito == -1)
    {
        return -1; // No hay lugar disponible
    }
    // Alta del prestador
    RAC[i] = p;
    return 1; // Alta exitosa
}

int bajaRAC(Prestador RAC[], Prestador p, int M)
{
    int i = 0, exito = 0, costo = 0;
    localizarRAC(RAC, p.dni, M, &i, &costo, &exito);
    if (exito == 1)
    {
        if (sonDiferentes(p, RAC[i]) == 1)
        {
            return -1; // Los prestadores no son iguales
        }
        else
        {
            RAC[i].dni = 0;
            return 1;
        } // baja exitosa
    }
    else
    {
        return 0; // baja fracasada
    }
}

Prestador evocarRAC(Prestador RAC[], int dni, int M, int *exito, int *costo)
{
    (*costo) = 0;
    int i = 0, exi;
    Prestador aux;
    localizarRAC(RAC, dni, M, &i, costo, &exi);
    if (exi == 1)
    {
        aux = RAC[i];
        *exito = 1;
        return aux;
    }
    else
    {
        *exito = 0;
        return aux;
    }
}

void mostrarRAC(Prestador RAC[], int M)
{
    printf("\n----Rebalse Abierto Cuadratico----\n");
    int i, aux = 0;
    for (i = 0; i < M; i++)
    {
        if ((aux % 5) == 0)
        {
            getchar();
        }
        if (RAC[i].dni == -1)
        {
            printf("\nDatos de la posicion %d:\n", i);
            printf(" -Posicion virgen\n");
        }
        else if (RAC[i].dni == 0)
        {
            printf("\nDatos de la posicion %d:\n", i);
            printf(" -Posicion libre\n");
        }
        else
        {
            printf("\n------------*------------\n");
            printf("\nDatos de la posicion %d:\n", i);
            printf("\nDNI: %d", RAC[i].dni);
            printf("\nNombre: %s", RAC[i].nombre);
            printf("\nServicios: %s", RAC[i].servicios);
            printf("\nDomicilio: %s", RAC[i].domicilio);
            printf("\nCorreo electronico: %s", RAC[i].email);
            printf("\nNumero de telefono: %s \n", RAC[i].telefono);
            printf("\n------------*------------\n");
        }
        aux++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------

void cargarArchivo(Prestador RAC[], Prestador RAL[], TablaRS *RS, Informacion *info, Costos *costo)
{

    int exito = 0, costoauxi = 0;
    int operacion = 0;
    Prestador aux, aux1;
    FILE *fp = fopen("Operaciones-Prestadores.txt", "r");
    if (fp == NULL)
    {
        printf("Error con el archivo");
        exit(3);
    }

    fseek(fp, 0, SEEK_END);

    if (ftell(fp) == 0)
    {
        fclose(fp); // Asegurarse de cerrar el archivo antes de salir
        exit(2);
    }

    rewind(fp);

    while (!feof(fp))
    {
        fscanf(fp, " %d", &operacion);
        fscanf(fp, "%d\n", &aux.dni);

        if (operacion == 1 || operacion == 2)
        {
            fscanf(fp, "%[^\n]\n", aux.nombre);
            fscanf(fp, "%[^\n]\n", aux.servicios);
            fscanf(fp, "%[^\n]\n", aux.domicilio);
            fscanf(fp, "%[^\n]\n", aux.email);
            fscanf(fp, "%[^\n]\n", aux.telefono);
        }

        switch (operacion)
        {
        case 1:
            if (aux.dni > 99999999 || 10000000 > aux.dni)
            {
                printf("El prestador ingresado tiene un dni invalido: %d\n", aux.dni);
                break;
            }
            if ( altaRS(RS, aux, info->M_RS) == 1)
            {
                info->cantRS++;
            }

            if (info->cantRAL <= info->M_RAL && altaRAL(RAL, aux, info->M_RAL) == 1)
            {
                info->cantRAL++;
            }

            if (info->cantRAC <= info->M_RAC && altaRAC(RAC, aux, info->M_RAC) == 1)
            {
                info->cantRAC++;
            }
            break;
        case 2:
            if (info->cantRS > 0)
            {
                if (bajaRS(RS, aux, info->M_RS) == 1)
                {
                    info->cantRS--;
                }
            }
            if (info->cantRAL > 0)
            {
                if (bajaRAL(RAL, aux, info->M_RAL) == 1)
                {
                    info->cantRAL--;
                }
            }
            if (info->cantRAC > 0)
            {
                if (bajaRAC(RAC, aux, info->M_RAC) == 1)
                {
                    info->cantRAC--;
                }
            }
            break;

        case 3:
            //----------------------------------------------------------RS
            if (info->cantRS > 0)
            {
                aux1 = evocarRS(RS, aux.dni, info->M_RS, &exito, &costoauxi);
                switch (exito)
                {
                case 1:
                    costo->cantEvoRS[0]++;
                    costo->costoRS[0] = costo->costoRS[0] + costoauxi;
                    if (costo->maxRS[0] < costoauxi)
                    {
                        costo->maxRS[0] = costoauxi;
                    }
                    break;
                case 0:
                    costo->cantEvoRS[1]++;
                    costo->costoRS[1] = costo->costoRS[1] + costoauxi;
                    if (costo->maxRS[1] < costoauxi)
                    {
                        costo->maxRS[1] = costoauxi;
                    }
                    break;
                }
            }

            //----------------------------------------------------------RAL
            if (info->cantRAL > 0)
            {
                aux1 = evocarRAL(RAL, aux.dni, info->M_RAL, &exito, &costoauxi);
                switch (exito)
                {
                case 1:
                    costo->cantEvoRAL[0]++;
                    costo->costoRAL[0] = costo->costoRAL[0] + costoauxi;
                    if (costo->maxRAL[0] < costoauxi)
                    {
                        costo->maxRAL[0] = costoauxi;
                    }
                    break;
                case 0:
                    costo->cantEvoRAL[1]++;
                    costo->costoRAL[1] = costo->costoRAL[1] + costoauxi;
                    if (costo->maxRAL[1] < costoauxi)
                    {
                        costo->maxRAL[1] = costoauxi;
                    }
                    break;
                }
            }
            /* else
             {
                 printf("Error de RAL esta intentando evocar con la lista vacia\n");
             }*/
            //----------------------------------------------------------RAC
            if (info->cantRAC > 0)
            {
                aux1 = evocarRAC(RAC, aux.dni, info->M_RAC, &exito, &costoauxi);
                switch (exito)
                {
                case 1:
                    costo->cantEvoRAC[0]++;
                    costo->costoRAC[0] = costo->costoRAC[0] + costoauxi;
                    if (costo->maxRAC[0] < costoauxi)
                    {
                        costo->maxRAC[0] = costoauxi;
                    }

                    break;
                case 0:
                    costo->cantEvoRAC[1]++;
                    costo->costoRAC[1] = costo->costoRAC[1] + costoauxi;
                    if (costo->maxRAC[1] < costoauxi)
                    {
                        costo->maxRAC[1] = costoauxi;
                    }
                    break;
                }
            }
            /*else
             {
                 printf("Error de RAC esta intentando evocar con la lista vacia\n");
             }*/
            break;

        default:
            printf("Operacion no valida\n");
        }
    }
    fclose(fp);
}

int main()
{
    Costos costo = {0};
    Informacion info;
    // ATENCION ninguna de las estructuras cuenta las evocaciones no existosas estando vacias

    info.M_RS = 45;
    info.M_RAL = 119;
    info.M_RAC = 127;
    info.cantRS = 0;
    info.cantRAL = 0;
    info.cantRAC = 0;

    TablaRS *RS;
    RS = (TablaRS *)malloc(sizeof(TablaRS));

    if (RS == NULL)
{
  return 2; // Error al reservar memoria para RS
}
    Prestador RAL[info.M_RAL];
    Prestador RAC[info.M_RAC];

    inicializar_RS(RS, info.M_RS);

    for (int i = 0; i < info.M_RAL; i++)
    {
        RAL[i].dni = -1;
    }

    for (int i = 0; i < info.M_RAC; i++)
    {
        RAC[i].dni = -1;
    }
    inicializar_costos(&costo);

    int des = -2, des2 = -2;
    while (des != 0)
    {
        printf("\n\nBIENVENIDO\n");
        printf("Ingrese la opcion deseada:\n\n1-Comparacion de estructuras\n2-Mostrar Estructura\n3-Salir\n");
        scanf("%d", &des);
        switch (des)
        {
        case 1:
            system("cls");
            printf("USTED SELECCIONO: COMPARACION DE ESTRUCTURAS\n");
            inicializar_costos(&costo);
            
            inicializar_RS(RS, info.M_RS);
            
            for (int i = 0; i < info.M_RAL; i++)
            {
                RAL[i].dni = -1;
            }
            
            for (int i = 0; i < info.M_RAC; i++)
            {
                RAC[i].dni = -1;
            }
            
            info.cantRS = 0;
            info.cantRAL = 0;
            info.cantRAC = 0;

            cargarArchivo(RAC, RAL, RS, &info, &costo);
            mostrarCostos(info, &costo);
            break;
        case 2:
            system("cls");

            printf("USTED SELECCIONO: MOSTRAR ESTRUCTURA\n");
            do
            {
                printf("\nElija que estructura desea mostrar");
                printf("\n1-Rebalse Separado\n2-Rebalse Abierto Lineal\n3-Rebalse Abierto Cuadratico\n4-Salir\n");
                scanf("%d", &des2);
                switch (des2)
                {
                case 1:
                    system("cls");
                    mostrarRS(&*RS, info.M_RS);
                    break;
                case 2:
                    system("cls");
                    mostrarRAL(RAL, info.M_RAL);
                    break;
                case 3:
                    system("cls");
                    mostrarRAC(RAC, info.M_RAC);
                    break;
                case 4:
                    system("cls");
                    break;
                default:
                    printf("\nIngrese una opcion correcta\n");
                }
            } while (des2 != 4);
            break;
        case 3:
            des = 0;
            system("cls");
            break;
        }
    }
}
