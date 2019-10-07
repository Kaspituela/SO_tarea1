#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

/* variable de estado
El valor de esta variable representa un efecto aplicado por una carta especial

0:  no efecto
1:  +2
2:  +4
3:  salto
4:  reversa
5:  cambio color
*/
int estado = 0;

/* variable carta
Permite el paso de string entre funciones a través de memoria estática
*/
char carta[20];

/*variable color
Se revisa el valor de esta carta cuando se acumula un efecto de cambio de color
o +4

0:  no inicializada
1:  azul
2:  rojo
3:  verde
4:  amarillo
*/
int color=0;

/*variable sentido
indica el sentido de los turnos, cresciente o decresciente (1 2 3 o 1 4 3)
1 :  cresciente
-1:  decresciente
*/
int sentido = 1;

/*variable ultima_jugada 1 y 2
indica la primera y segunda parte del nombre de la carta en lastCard para facilitar
el acceso al nombre
*/
char ultima_jugada1[20], ultima_jugada2[20];

/*variable cant_cartas_sacadas
almacena lacantidad de cartas que han sacado del mazo
parte en 29 porque esa es la cantidad de cartas que quedan luego de repartir manos
y poner la primera carta
*/
int cant_cartas_sacadas = 29;


void entregarCarta(char *destino, char *carta){
    chdir(destino);
    FILE *fp;
    fp = fopen(carta,"w");
    fclose(fp);
    chdir("..");
}


char* obtenerCarta(int cant){
    char s[100];
    int i = 1;
    int num = rand()%(110 - cant) + 1;
    printf("%d\n", num);
    DIR *dir_actual;
    struct dirent *file;
    dir_actual = opendir("./mazo");
    if (dir_actual) {
        while ((file = readdir(dir_actual)) != NULL) {
            if (i==num)
            {
                if (strcmp(file->d_name,".")==0 || strcmp(file->d_name,"..")==0)
                {
                    closedir(dir_actual);
                    return obtenerCarta(cant);
                }
                printf("%s\n", file->d_name);
                strcpy(carta, file->d_name);
                printf("%s\n", carta);
                printf("%s\n", getcwd(s, 100));
                chdir("mazo");
                remove(carta);
                chdir("..");
                closedir(dir_actual);
                break;
            }
            i++;
        }

    } else {
        printf("Error al abrir directorio\n");
        exit(0);
    }
    return carta;
}

void repartirCartas()
{
    char *carta;
    int i;
    for (i = 0; i < 7; i++) {
        carta = obtenerCarta(i*4);
        entregarCarta("./player1",carta);

        carta = obtenerCarta(i*4+1);
        entregarCarta("./player2",carta);

        carta = obtenerCarta(i*4+2);
        entregarCarta("./player3",carta);

        carta = obtenerCarta(i*4+3);
        entregarCarta("./player4",carta);
    }
    carta = obtenerCarta(28);
    entregarCarta("./lastCard",carta);

}

void createCards(char *name, char *dir){
    FILE *carta;
    char nombre[20];
    chdir(dir);
    char colores[4][9] = {"azul", "rojo", "verde", "amarillo"};
    int i, j;
    for(i=0; i<4; i++)
    {
        //0s
        sprintf(nombre, "0 %s.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);

        //+2s
        sprintf(nombre, "+2 %s.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);
        sprintf(nombre, "+2 %s 2.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);

        //reversas (R)
        sprintf(nombre, "R %s.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);
        sprintf(nombre, "R %s 2.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);

        //saltos (S)
        sprintf(nombre, "S %s.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);
        sprintf(nombre, "S %s 2.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);

        //cambios de color (color)
        sprintf(nombre, "color%d.txt", i+1);
        carta = fopen(nombre, "w");
        fclose(carta);

        //+4
        sprintf(nombre, "+4 %d.txt", i+1);
        carta = fopen(nombre, "w");
        fclose(carta);

        //del 1 al 9
        for(j=1; j<10; j++)
        {
            sprintf(nombre, "%d %s.txt", j, colores[i]);
            carta = fopen(nombre, "w");
            fclose(carta);
            sprintf(nombre, "%d %s 2.txt", j, colores[i]);
            carta = fopen(nombre, "w");
            fclose(carta);
        }
    }
    chdir("..");
}


int getVal(char* palabra)
{
    //obtiene la suma de los valores de cada caracter en la palabra
    int i, suma = 0;
    for(i=0; palabra[i]; i++)
        suma += (int)palabra[i];
    return suma;
}


void jugarCarta(char *nombre)
{
    //borra el contenido de lastCard y pone una carta especificada ahí
    chdir("lastCard");
    system("rm -rf ./*");
    FILE *arc = fopen(nombre, "w");
    fclose(arc);
    chdir("..");
}


void masCartas(int jugador, int cantidad)
{
    //le agrega cierta 'cantidad' de cartas al 'jugador' especificado
    int i;
    char s[20];
    sprintf(s, "player%d", jugador);
    for(i=0; i<cantidad; i++)
    {
        obtenerCarta(cant_cartas_sacadas++);
        entregarCarta(s, carta);
    }
}


void turno(int jugador)
{
    /*
    ejecuta el turno del 'jugador' revisando si tiene efectos acumulados,
    revisa la última carta jugada y juega la primera carta que cumpla con las condiciones
    aplicando los efectos correspondientes
    */
    switch (estado)
    {
        case 0:
            break;
        
        case 1:
            masCartas(jugador, 2);
            estado = 0;
            return;

        case 2:
            masCartas(jugador, 4);
            estado = 0;
            return;
        case 3:
            estado = 0;
            return;

        case 4:
            printf("Esto no debería pasar R\n");
            exit(0);

        case 5:
            printf("Esto no debería pasar COLOR\n");
            exit(0);
    }
    char *token1, *token2, nombre_carta[20];

    char nombre_player_dir[20];//nombre del directorio del jugador
    sprintf(nombre_player_dir, "player%d", jugador);

    DIR *player_dir = opendir(nombre_player_dir);
    struct dirent *player_carta;

    player_carta = readdir(player_dir);
    while(player_carta)
    {
        if((player_carta->d_name)[0] != '.')
        {
            strcpy(nombre_carta, player_carta->d_name);
            token1 = strtok(nombre_carta, " ");
            token2 = strtok(NULL, " ");

            if(!strcmp(token1, ultima_jugada1) || !strcmp(token2, ultima_jugada2))
            {
                closedir(player_dir);
                jugarCarta(player_carta->d_name);
                strcpy(ultima_jugada1, token1);
                strcpy(ultima_jugada2, token2);
                chdir(nombre_player_dir);
                remove(player_carta->d_name);
                chdir("..");

                int val = getVal(token1);
                switch (val)
                {
                    case 93:
                        estado = 1;
                        break;
                    case 95:
                        estado = 2;
                        color = rand()%4+1;
                        break;
                    case 83:
                        estado = 3;
                        break;
                    case 82:
                        sentido = !sentido;
                        break;
                    default:
                        color = rand()%4+1;
                        break;
                }
                return;
            }
        }
        player_carta = readdir(player_dir);
    }
    closedir(player_dir);

    strcpy(nombre_carta, obtenerCarta(cant_cartas_sacadas++));
    token1 = strtok(nombre_carta, " ");
    token2 = strtok(NULL, " ");
    if(!strcmp(token1, ultima_jugada1) || !strcmp(token2, ultima_jugada2))
        {
            jugarCarta(player_carta->d_name);
            return;
        }

    entregarCarta(nombre_player_dir, carta);
}

void mostrarCartas(int jugador)
{
    char dir_jugador[20];
    sprintf("player%d", jugador);
    chdir(dir_jugador);
    system("ls");
    chdir("..");
}

int main()
{
    time_t t;
    srand((unsigned)time(&t));
    
    char *name = "cartas.txt";
    struct stat st = {0};
    //Se crea mazo
    if (stat("./mazo",&st) >= 0)
        if(system("rm -rf ./mazo")!=0)
        {
            printf("No se ha podido eliminar directorio mazo\n");
            exit(0);
        }
    if (mkdir("./mazo", 0777) == -1){
        printf("Error para crear directorio mazo\n");
        exit(0);
    }

    //Create cards
    char s[100];
    createCards(name,"mazo");
    printf(" %s\n", getcwd(s, 100));

    //Create players
    if (stat("./player1",&st) >= 0)
        if(system("rm -rf ./player1")!=0)
        {
            printf("No se ha podido eliminar directorio player1\n");
            exit(0);
        }
    if (mkdir("./player1", 0777) == -1){
        printf("Error para crear directorio player1\n");
        exit(0);
    }

    if (stat("./player2",&st) >= 0)
        if(system("rm -rf ./player2")!=0)
            {
                printf("No se ha podido eliminar directorio player2\n");
                exit(0);
            }
    if (mkdir("./player2", 0777) == -1){
        printf("Error para crear directorio player2\n");
        exit(0);
    }

    if (stat("./player3",&st) >= 0)
        if(system("rm -rf ./player3")!=0)
            {
                printf("No se ha podido eliminar directorio player3\n");
                exit(0);
            }
    if (mkdir("./player3", 0777) == -1){
        printf("Error para crear directorio player3\n");
        exit(0);
    }

    if (stat("./player4",&st) >= 0)
        if(system("rm -rf ./player4")!=0)
            {
                printf("No se ha podido eliminar directorio player4\n");
                exit(0);
            }
    if (mkdir("./player4", 0777) == -1){
        printf("Error para crear directorio player4\n");
        exit(0);
    }

    //Create lastCard
    if (stat("./lastCard",&st) >= 0)
        if(system("rm -rf ./lastCard")!=0)
            {
                printf("No se ha podido eliminar directorio lastCard\n");
                exit(0);
            }
    if (mkdir("./lastCard", 0777) == -1){
        printf("Error para crear directorio lastCard\n");
        exit(0);
    }


    //listFiles("./mazo");
    repartirCartas();


    return 0;
}
