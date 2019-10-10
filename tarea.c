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
char carta[200];

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

int jugador_actual = 1;

void next()
{
    if(sentido == 1)
    {
        if(jugador_actual == 4) jugador_actual = 1;
        else ++jugador_actual;
    }
    else
    {
        if (jugador_actual == 1) jugador_actual = 4;
        else --jugador_actual;
    }
}

int isEmpty(int directorio)
{
/* revisa si algún directorio está vacío basado en un codigo para los directorios
0:  revisa el maso
n:  revisa la mano del jugador n
*/
    char dir_name[20];
    if(directorio == 0) strcpy(dir_name, "mazo");
    else sprintf(dir_name, "player%d", directorio);

    DIR *dir = opendir(dir_name);
    if(!dir)
    {
        printf("Se ha geneado un error isEmpty\n");
        exit(0);
    }
    struct dirent *cartas_mazo = readdir(dir);
    while(cartas_mazo)
    {
        if((cartas_mazo->d_name)[0] != '.')
        {
            closedir(dir);
            return 1;
        }
        cartas_mazo = readdir(dir);
    }
    closedir(dir);
    if(directorio == 0) printf("Se acabo el mazo F\n");
    else printf("Gano el jugador %d", directorio);
    exit(0);

}
int getVal(char* palabra)
{
    //obtiene la suma de los valores de cada caracter en la palabra
    int i, suma = 0;
    for(i=0; palabra[i]; i++)
        suma += (int)palabra[i];
    return suma;
}


void entregarCarta(char *destino){
    chdir(destino);
    FILE *fp;
    fp = fopen(carta,"w");
    fclose(fp);
    chdir("..");
}


char* obtenerCarta()
{
    isEmpty(0);
    DIR *mazo = opendir("mazo");
    if(!mazo)
    {
        printf("Error al abrir mazo");
        exit(0);
    }
    struct dirent *arc_mazo;
    arc_mazo = readdir(mazo);
    while(arc_mazo != NULL)
    {
        if((arc_mazo->d_name)[0] != '.')
        {
            strcpy(carta, arc_mazo->d_name);
            if(rand()%2 == 0 )
            {
                closedir(mazo);
                return carta;
            }
        }
        arc_mazo = readdir(mazo);
    }
    printf("control 1");
    closedir(mazo);
    return carta;
}

void repartirCartas()
{
    int i;
    for (i = 0; i < 7; i++)
    {
        obtenerCarta();
        entregarCarta("./player1");

        obtenerCarta();
        entregarCarta("./player2");

        obtenerCarta();
        entregarCarta("./player3");

        obtenerCarta();
        entregarCarta("./player4");
    }
    obtenerCarta();

    entregarCarta("./lastCard");

    char *token1,*token2;
    char aux[20];
    strcpy(aux, carta);
    token1 = strtok(aux, " ");
    token2 = strtok(NULL, " ");
    int v = getVal(token1);
    strcpy(ultima_jugada1,token1);
    strcpy(ultima_jugada2,token2);

    printf("ultima_jugada: %s\n",ultima_jugada1);
    printf("ultima_jugada2: %s\n",ultima_jugada2);
    printf("token 1: %s\n", token1);
    printf("token 2: %s\n", token2);

    if (v < 82) return;
    switch (v)
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

}

void createCards(char *name, char *dir){
    FILE *carta_;
    char nombre[20];
    chdir(dir);
    char colores[4][9] = {"azul", "rojo", "verde", "amarillo"};
    int i, j;
    for(i=0; i<4; i++)
    {
        //0s
        sprintf(nombre, "0 %s .txt", colores[i]);
        carta_ = fopen(nombre, "w");
        fclose(carta_);

        //+2s
        sprintf(nombre, "+2 %s .txt", colores[i]);
        carta_ = fopen(nombre, "w");
        fclose(carta_);
        sprintf(nombre, "+2 %s 2 .txt", colores[i]);
        carta_ = fopen(nombre, "w");
        fclose(carta_);

        //reversas (R)
        sprintf(nombre, "R %s .txt", colores[i]);
        carta_ = fopen(nombre, "w");
        fclose(carta_);
        sprintf(nombre, "R %s 2 .txt", colores[i]);
        carta_ = fopen(nombre, "w");
        fclose(carta_);

        //saltos (S)
        sprintf(nombre, "S %s .txt", colores[i]);
        carta_ = fopen(nombre, "w");
        fclose(carta_);
        sprintf(nombre, "S %s 2 .txt", colores[i]);
        carta_ = fopen(nombre, "w");
        fclose(carta_);

        //cambios de color (color)
        sprintf(nombre, "color %d .txt", i+1);
        carta_ = fopen(nombre, "w");
        fclose(carta_);

        //+4
        sprintf(nombre, "+4 %d .txt", i+1);
        carta_ = fopen(nombre, "w");
        fclose(carta_);

        //del 1 al 9
        for(j=1; j<10; j++)
        {
            sprintf(nombre, "%d %s .txt", j, colores[i]);
            carta_ = fopen(nombre, "w");
            fclose(carta_);
            sprintf(nombre, "%d %s 2 .txt", j, colores[i]);
            carta_ = fopen(nombre, "w");
            fclose(carta_);
        }
    }
    chdir("..");
}





void jugarCarta(char *nombre)
{
    //borra el contenido de lastCard y pone una carta especificada ahí
    chdir("lastCard");
    system("rm -rf ./*");
    FILE *arc = fopen(nombre, "w");
    fclose(arc);
    printf("Jugador %d, juega la carta : %s\n", jugador_actual, nombre);
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
        printf("control 1");
        obtenerCarta();
        entregarCarta(s);
        printf("control 2");
    }
    printf("%s\n", s);
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
            printf("jugador %d, saca 2\n", jugador);
            masCartas(jugador, 2);
            estado = 0;
            return;

        case 2:
            printf("jugador %d, saca 4\n", jugador);
            masCartas(jugador, 4);
            estado = 0;
            return;
        case 3:
            printf("jugador %d, es saltado\n", jugador);
            estado = 0;
            return;

        case 4:
            printf("Esto no debería pasar R\n");
            exit(0);

        case 5:
            printf("Esto no debería pasar COLOR\n");
            exit(0);

        default:
            printf("Wut O.o");
            exit(0);
    }
    char *token1, *token2, nombre_carta[20];

    char nombre_player_dir[20];//nombre del directorio del jugador
    sprintf(nombre_player_dir, "player%d", jugador);

    DIR *player_dir = opendir(nombre_player_dir);
    if(!player_dir)
    {
        printf("Error al abrir %s\n", nombre_player_dir);
        exit(0);
    }
    struct dirent *player_carta;

    player_carta = readdir(player_dir);
    while(player_carta != NULL)
    {
        if((player_carta->d_name)[0] != '.')
        {
            strcpy(nombre_carta, player_carta->d_name);
            token1 = strtok(nombre_carta, " ");
            token2 = strtok(NULL, " ");

            if(!strcmp(token1, ultima_jugada1) || !strcmp(token2, ultima_jugada2))
            {
                //closedir(player_dir);
                jugarCarta(player_carta->d_name);
                strcpy(ultima_jugada1, token1);
                strcpy(ultima_jugada2, token2);

                chdir(nombre_player_dir);
                remove(player_carta->d_name);
                chdir("..");

                isEmpty(jugador);

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
                        sentido = sentido == 1? 0: 1;
                        break;
                    default:
                        color = rand()%4+1;
                }
                closedir(player_dir);
                return;
            }
        }
        player_carta = readdir(player_dir);
    }
    closedir(player_dir);
    strcpy(nombre_carta, obtenerCarta());
    token1 = strtok(nombre_carta, " ");
    token2 = strtok(NULL, " ");
    if(!strcmp(token1, ultima_jugada1) || !strcmp(token2, ultima_jugada2))
        {
            jugarCarta(player_carta->d_name);
            strcpy(ultima_jugada1, token1);
            strcpy(ultima_jugada2, token2);

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
                    sentido = sentido == 1? 0: 1;
                    break;
                default:
                    color = rand()%4+1;
            }
            return;
        }

    printf("jugador %d, no tiene para jugar\n", jugador);
    entregarCarta(nombre_player_dir);
}

void mostrarCartas(int jugador)
{
    char comando[50];
    sprintf(comando, "ls ./player%d/", jugador);;
    system(comando);
}


int main(int argc, char const *argv[])
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



    repartirCartas();
    /*
    while(isEmpty(0))
    {
      //  printf("Cartas del jugador : %d\n", jugador_actual);
    //  mostrarCartas(jugador_actual);
        //printf("\n\n\n");
        turno(jugador_actual);
        next();
    }
    */
    

   //testing
   
   /*
   next rdy
   isEmpty rdy
   getVal rdy
   entregarCarta rdy
   obtenerCarta rdy
   epartirCartas rdy
   createCards rdy
   jugarCarta rdy
   */
    //printf("control 1");
    //masCartas(1, 4);
    
    return 0;
}
