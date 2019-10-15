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
void killProcess(){
  int pid;
  char command[50];
  if (jugador_actual==1)
    pid = getpid();
  else
    pid = getppid();
  sprintf(command,"kill -9 %d",pid);
  system(command);
  return;
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
    if(directorio == 0) {
    printf("Se acabo el mazo F\n");
    killProcess();
    }
    else{
    printf("Gano el jugador %d\n", directorio);
    killProcess();
    }
    return 0;
}
int getVal(char* palabra)
{
    //obtiene la suma de los valores de cada caracter en la palabra
    int i, suma = 0;
    for(i=0; palabra[i]; i++)
        suma += (int)palabra[i];
    return suma;
}


void entregarCarta(char *destino){  // destino formato "./player%d"
    chdir(destino);
    FILE *fp;
    fp = fopen(carta,"w");
    fclose(fp);
    chdir("..");
}


void obtenerCarta(char *dir)
{
    isEmpty(0);
    DIR *mazo = opendir(dir);
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
                chdir("mazo");
                remove(carta);
                chdir("..");
                closedir(mazo);
                return;
            }
        }
        arc_mazo = readdir(mazo);
    }
    chdir("mazo");
    remove(carta);
    chdir("..");
    closedir(mazo);
}

void repartirCartas()
{
    int i;
    for (i = 0; i < 7; i++)
    {
        obtenerCarta("mazo");
        entregarCarta("./player1");

        obtenerCarta("mazo");
        entregarCarta("./player2");

        obtenerCarta("mazo");
        entregarCarta("./player3");

        obtenerCarta("mazo");
        entregarCarta("./player4");
    }
    obtenerCarta("mazo");

    entregarCarta("./lastCard");

    char *token1,*token2;
    char aux[20];
    strcpy(aux, carta);
    token1 = strtok(aux, " ");
    token2 = strtok(NULL, " ");
    int v = getVal(token1);
    strcpy(ultima_jugada1,token1);
    strcpy(ultima_jugada2,token2);

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

void printColor(){
  switch (color) {
    case 1:
      printf("Carta de color  Azul\n");
      break;
    case 2:
      printf("Carta de color  Rojo\n");
      break;
    case 3:
      printf("Carta de color  Verde\n");
      break;
    case 4:
      printf("Carta de color  Amarillo\n");
      break;
    default:
      printf("Error printColor\n");
      exit(0);
  }
}

int mostrarCartas(int jugador)
{
  char nombre_player_dir[50],nombre_carta[50], *token1;//nombre del directorio del jugador
  int i = 0;
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
          token1 = strtok(nombre_carta, ".txt");
          printf("[%d] %s\n", ++i,token1);
      }
      player_carta = readdir(player_dir);
  }
  closedir(player_dir);
  return i;
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
        obtenerCarta("mazo");
        entregarCarta(s);
    }
}

int verificarCarta(){
  char nombre_carta[50],*token1,*token2;
  strcpy(nombre_carta,carta);
  token1 = strtok(nombre_carta," ");
  token2 = strtok(NULL," ");
  if (getVal(token1)==95){
    estado = 2;
    if (jugador_actual!=1) {
      color = rand()%4+1;
      printColor();
    }
    return 1;
  }
  if (getVal(token1)== 93 && getVal(ultima_jugada1) == 93){
    estado = 1;
    return 1;
  }
  if (getVal(token1)== 83 && getVal(ultima_jugada1) == 83){
    estado = 3;
    return 1;
  }
  if (getVal(token1)== 82 && getVal(ultima_jugada1) == 82){
    return 1;
  }
  if (!strcmp("color",token1)){
    if (jugador_actual!=1){
      color = rand()%4+1;
      printColor();
    }
    return 1;
  }
  if (color!=0) {
    switch (color) {
      case 1:
        if (!strcmp("azul",token2)){
          color = 0;
          return 1;
        }
        else
          return 0;
      case 2:
        if (!strcmp("rojo",token2)){
          color = 0;
          return 1;
        }
        else
          return 0;
      case 3:
        if (!strcmp("verde",token2)){
          color = 0;
          return 1;
        }
        else
          return 0;
      case 4:
        if (!strcmp("amarillo",token2)){
          color = 0;
          return 1;
        }
        else
          return 0;
      default:
        printf("Error de color\n");
    }
  }
  if (!strcmp(token2,ultima_jugada2)) {
    estado = 0;
    return 1;
  }
  if (!strcmp(token1,ultima_jugada1)) {
    estado = 0;
    return 1;
  }
  return 0;
}

void setVal(char *token1){
  if (getVal(token1)== 82){
    sentido = sentido == 1? 0: 1;
    return;
  }
  if (getVal(token1)== 93){
    estado = 1;
    return;
  }
  if (getVal(token1)== 83){
    estado = 3;
    return;
  }
  if (getVal(token1)==95) {
    estado = 2;
    return;
  }
  estado = 0;
  return;
}

void jugarSolo(int jugador){
  char *token1, *token2, nombre_carta[50];
  int flag;
  char nombre_player_dir[50];//nombre del directorio del jugador
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
          strcpy(carta, nombre_carta);
          flag = verificarCarta();
          if (flag) {
            jugarCarta(carta);
            token1 = strtok(nombre_carta, " ");
            setVal(token1);
            token2 = strtok(NULL, " ");
            strcpy(ultima_jugada1,token1);
            strcpy(ultima_jugada2,token2);
            chdir(nombre_player_dir);
            remove(carta);
            chdir("..");
            closedir(player_dir);
            isEmpty(jugador);
            return;
          }
      }
      player_carta = readdir(player_dir);
  }
  closedir(player_dir);
  obtenerCarta("mazo");
  printf("Jugador%d roba una carta porque no puede jugar alguna\n", jugador_actual);
  strcpy(nombre_carta, carta);
  flag = verificarCarta();
  if (flag) {
    jugarCarta(carta);
  } else {
    printf("jugador%d, no puede jugar una carta\n", jugador);
    entregarCarta(nombre_player_dir);
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
    if (jugador!=1) {
      jugarSolo(jugador);
    } else {
      int eleccion,cantidad;
      printf("Cartas: \n");
      printf("[0] Robar\n");
      cantidad = mostrarCartas(jugador);
      printf("Elija una carta\n");
      while (1) {
        scanf("%d", &eleccion);
        if (eleccion >=0 && eleccion <=cantidad)
          break;
        else
          printf("Eleccion incorrecta\n");
      }

      if (eleccion != 0) {
        char nombre_player_dir[50], nombre_carta[50], *token1,*token2;//nombre del directorio del jugador
        int i = 0;
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
                i++;
                if (i == eleccion) {
                  strcpy(carta, player_carta->d_name);
                  break;
                }
            }
            player_carta = readdir(player_dir);
        }
        closedir(player_dir);
        printf("carta: %s\n", carta);
        i = verificarCarta();
        switch (i) {
          case 0:
            printf("La carta no cumple las reglas, robas una carta\n");
            obtenerCarta("mazo");
            entregarCarta("./player1");
            token1 = strtok(carta,".txt");
            printf("El jugador%d roba la carta %s\n", jugador, token1);
            break;
          case 1:
            strcpy(nombre_carta,carta);
            token1 = strtok(nombre_carta, " ");
            setVal(token1);
            if (!strcmp("color",token1) || getVal(token1) == 95) {
              printf("[1] Azul\n");
              printf("[2] Rojo\n");
              printf("[3] Verde\n");
              printf("[4] Amarillo\n");
              printf("Elija un color:\n");
              scanf("%d", &color);
            }
            token2 = strtok(NULL, " ");
            jugarCarta(carta);
            strcpy(ultima_jugada1, token1);
            strcpy(ultima_jugada2, token2);

            chdir(nombre_player_dir);
            remove(carta);
            chdir("..");

            isEmpty(jugador);
            break;
          default:
            printf("Error de validar\n");
            exit(0);
      }
    } else{
      int sel,flag;
      char nombre_carta[50],*token1,*token2;
      obtenerCarta("mazo");
      strcpy(nombre_carta,carta);
      entregarCarta("./player1");
      token1 = strtok(nombre_carta,".txt");
      printf("El jugador%d roba la carta %s\n", jugador, token1);
      printf("¿Quieres jugarlo?\n[1]Si\n[2]No\n");
      scanf("%d", &sel);
      if (sel) {
        flag = verificarCarta();
        if (flag) {
            strcpy(nombre_carta,carta);
            token1 = strtok(nombre_carta, " ");
            setVal(token1);
            if (!strcmp("color",token1) || getVal(token1) == 95) {
              printf("[1] Azul\n");
              printf("[2] Rojo\n");
              printf("[3] Verde\n");
              printf("[4] Amarillo\n");
              printf("Elija un color:\n");
              scanf("%d", &color);
            }
            token2 = strtok(NULL, " ");
            jugarCarta(carta);
            strcpy(ultima_jugada1, token1);
            strcpy(ultima_jugada2, token2);

            chdir("player1");
            remove(carta);
            chdir("..");

            isEmpty(jugador);
        } else {
          printf("La carta no cumple las reglas, robas una carta\n");
          obtenerCarta("mazo");
          entregarCarta("./player1");
          token1 = strtok(carta,".txt");
          printf("El jugador%d roba la carta %s\n", jugador, token1);
        }
      }
    }
  }
}

void updateLast(){
  char nombre[50];
  obtenerCarta("lastCard");
  strcpy(nombre,carta);
  strcpy(ultima_jugada1,strtok(carta," "));
  strcpy(ultima_jugada2,strtok(NULL," "));
  printf("Ultima carta jugada : %s\n", strtok(nombre,".txt"));
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

    pid_t pid,pid2,pid3;
    pid_t pids[4];
    pids[0] = getpid();
    pids[1] = -1;
    pids[2] = -1;
    pids[3] = -1;

    int p0[2]; //pipes
    int p1[2];
    int p2[2];
    int p3[2];

    pipe(p0);
    pipe(p1);
    pipe(p2);
    pipe(p3);
    pid = fork();
    if (pid > 0) {
      pid2 = fork();
      if (pid2==0) {
        pids[2] = getpid();
      } else if(pid2<0){
        printf("Error al crear hijo2\n");
        exit(0);
      }
      if (pid2>0 && pid>0) {
        pid3 = fork();
        if (pid3 == 0) {
          pids[3] = getpid();
        } else if(pid3<0){
          printf("Error al crear hijo3\n");
          exit(0);
        }
      }
    } else if(pid<0){
      printf("Error al crear hijo1\n");
      exit(0);
    }
    if (pid==0) {
      pids[1] = getpid();
    }
    //pipe p*[0] lectura, p*[1] escritura.
    /*int p2[2];  //pipes para comunicar 4 procesos
    int p3[2];  //pipes para comunicar 4 procesos*/
    /*if (pipe(p2) == -1) {
      printf("Error pipe3\n");
    }
    if (pipe(p3) == -1) {
      printf("Error pipe4\n");
    }*/
  while(isEmpty(0))
  {
      //  printf("Cartas del jugador : %d\n", jugador_actual);
    //  mostrarCartas(jugador_actual);
        //printf("\n\n\n");
        if (getpid()==pids[0]) {
          if (jugador_actual==1) {
            printf("\n\n\n");
            printf("Turno del jugador%d\n",jugador_actual);
            updateLast();
            if (color!=0)
              printColor();
            turno(jugador_actual);
            next();
            close(p1[0]);
            write(p1[1], &jugador_actual, 1);
            close(p2[0]);
            write(p2[1], &jugador_actual, 1);
            close(p3[0]);
            write(p3[1], &jugador_actual, 1);

            write(p1[1], &estado, 1);
            write(p2[1], &estado, 1);
            write(p3[1], &estado, 1);

            write(p1[1], &sentido, 1);
            write(p2[1], &sentido, 1);
            write(p3[1], &sentido, 1);

            write(p1[1], &color, 1);
            write(p2[1], &color, 1);
            write(p3[1], &color, 1);
          } else if(jugador_actual!=1){
            close(p0[1]);
            read(p0[0], &jugador_actual, 1);

            read(p0[0], &estado, 1);

            read(p0[0], &sentido, 1);

            read(p0[0], &color, 1);

          }
        }

        if (getpid()==pids[1]) {
          if (jugador_actual==2) {
            printf("\n\n\n");
            printf(" Turno del jugador%d\n",jugador_actual);
            updateLast();
            if (color!=0)
              printColor();
            mostrarCartas(jugador_actual);
            turno(jugador_actual);
            next();
            printf("[Press Enter]");
            getchar();
            close(p0[0]);
            write(p0[1], &jugador_actual, 1);
            close(p2[0]);
            write(p2[1], &jugador_actual, 1);
            close(p3[0]);
            write(p3[1], &jugador_actual, 1);

            write(p0[1], &estado, 1);
            write(p2[1], &estado, 1);
            write(p3[1], &estado, 1);

            write(p0[1], &sentido, 1);
            write(p2[1], &sentido, 1);
            write(p3[1], &sentido, 1);

            write(p0[1], &color, 1);
            write(p2[1], &color, 1);
            write(p3[1], &color, 1);
          } else if(jugador_actual!=2) {
            close(p1[1]);
            read(p1[0], &jugador_actual, 1);

            read(p1[0], &estado, 1);

            read(p1[0], &sentido, 1);

            read(p1[0], &color, 1);

          }
        }

        if (getpid()==pids[2]) {
          if (jugador_actual==3) {
            printf("\n\n\n");
            printf("Turno del jugador%d\n",jugador_actual);
            updateLast();
            if (color!=0)
              printColor();
            mostrarCartas(jugador_actual);
            turno(jugador_actual);
            next();
            printf("[Press Enter]");
            getchar();
            close(p0[0]);
            write(p0[1], &jugador_actual, 1);
            close(p1[0]);
            write(p1[1], &jugador_actual, 1);
            close(p3[0]);
            write(p3[1], &jugador_actual, 1);

            write(p0[1], &estado, 1);
            write(p1[1], &estado, 1);
            write(p3[1], &estado, 1);

            write(p0[1], &sentido, 1);
            write(p1[1], &sentido, 1);
            write(p3[1], &sentido, 1);

            write(p0[1], &color, 1);
            write(p1[1], &color, 1);
            write(p3[1], &color, 1);
          } else if(jugador_actual!=3) {
            close(p2[1]);
            read(p2[0], &jugador_actual, 1);

            read(p2[0], &estado, 1);

            read(p2[0], &sentido, 1);

            read(p2[0], &color, 1);

          }
        }

        if (getpid()==pids[3]) {
          if (jugador_actual==4) {
            printf("\n\n\n");
            printf("Turno del jugador%d\n",jugador_actual);
            updateLast();
            if (color!=0)
              printColor();
            mostrarCartas(jugador_actual);
            turno(jugador_actual);
            next();
            printf("[Press Enter]");
            getchar();
            close(p0[0]);
            write(p0[1], &jugador_actual, 1);
            close(p1[0]);
            write(p1[1], &jugador_actual, 1);
            close(p2[0]);
            write(p2[1], &jugador_actual, 1);

            write(p0[1], &estado, 1);
            write(p1[1], &estado, 1);
            write(p2[1], &estado, 1);

            write(p0[1], &sentido, 1);
            write(p1[1], &sentido, 1);
            write(p2[1], &sentido, 1);

            write(p0[1], &color, 1);
            write(p1[1], &color, 1);
            write(p2[1], &color, 1);
          } else if(jugador_actual!=4) {
            close(p3[1]);
            read(p3[0], &jugador_actual, 1);

            read(p3[0], &estado, 1);

            read(p3[0], &sentido, 1);

            read(p3[0], &color, 1);

          }
        }
   }
  exit(0);
}
