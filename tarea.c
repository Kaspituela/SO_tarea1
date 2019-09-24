#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

void eliminarCartas(char *jugador){
  char s[100];
  printf("%s\n",jugador);
  printf("%s\n", getcwd(s, 100));
  DIR *dir;
  struct dirent *file;
  dir = opendir(jugador);
  chdir(jugador);
  if (dir) {
    while ((file = readdir(dir)) != NULL) {
      if ( strcmp(file->d_name,".") != 0 && strcmp(file->d_name,"..") !=0 ){
        remove(file->d_name);
        printf("%s\n", file->d_name);
      }
    }
  }
  chdir("..");
  closedir(dir);
}
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
  char *carta = (char *)malloc(sizeof(char)*15);
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
          return obtenerCarta(cant);
        printf("%s\n", file->d_name);
        strcpy(carta,file->d_name);
        printf("%s\n", carta);
        printf("%s\n", getcwd(s, 100));
        chdir("mazo");
        remove(carta);
        chdir("..");
        break;
      }
      i++;
    }
    closedir(dir_actual);
  } else {
      printf("Error al abrir directorioo\n");
      exit(0);
  }
  return carta;
}

void repartirCartas(){
  char *carta;
  for (int i = 0; i < 7; i++) {
    carta = obtenerCarta((i*4));
    entregarCarta("./player1",carta);
    free(carta);
    carta = obtenerCarta((i*4)+1);
    entregarCarta("./player2",carta);
    free(carta);
    carta = obtenerCarta((i*4)+2);
    entregarCarta("./player3",carta);
    free(carta);
    carta = obtenerCarta((i*4)+3);
    entregarCarta("./player4",carta);
    free(carta);
  }
  carta = obtenerCarta(28);
  entregarCarta("./lastCard",carta);
  free(carta);
}

void listFiles(char *dir){
    DIR *dir_actual;
    struct dirent *file;
    dir_actual = opendir(dir);
    if (dir_actual) {
      while ((file = readdir(dir_actual)) != NULL) {
        printf("%s\n", file->d_name);
      }
      closedir(dir_actual);
    } else {
        printf("Error al abrir directorio\n");
        exit(0);
    }
}

void createCards(char *name, char *dir){
  FILE *fp;
  FILE *carta;
  char s[100];
  fp = fopen(name,"r");
  char nombre[15];
  chdir(dir);
  printf("%s\n", getcwd(s, 100));
  while (fgets(nombre, 15, fp)) {
        strtok(nombre, "\n");
        strcat(nombre, ".txt");
        carta = fopen(nombre,"w");
        fclose(carta);
    }
  chdir("..");
  fclose(fp);
}


int main() {
srand(time(0));
char *name = "cartas.txt";
struct stat st = {0};
//Create directory
if (stat("./mazo",&st) < 0) {
  if (mkdir("./mazo", 0777) == -1)
    printf("Error para crear directorio mazo\n");
}
//Create cards
if (stat("./mazo",&st) >= 0){
    char s[100];
    createCards(name,"mazo");
    printf(" %s\n", getcwd(s  , 100));
}
//Create players
if (stat("./player1",&st) < 0) {
  if (mkdir("./player1", 0777) == -1)
    printf("Error para crear directorio player1\n");
} else {
  eliminarCartas("./player1");
}
if (stat("./player2",&st) < 0) {
  if (mkdir("./player2", 0777) == -1)
    printf("Error para crear directorio player2\n");
} else {
  eliminarCartas("./player2");
}
if (stat("./player3",&st) < 0) {
  if (mkdir("./player3", 0777) == -1)
    printf("Error para crear directorio player3\n");
} else {
  eliminarCartas("./player3");
}
if (stat("./player4",&st) < 0) {
  if (mkdir("./player4", 0777) == -1)
    printf("Error para crear directorio player4\n");
} else {
  eliminarCartas("./player4");
}

//Create lastCard
if (stat("./lastCard",&st) < 0) {
  if (mkdir("./lastCard", 0777) == -1)
    printf("Error para crear directorio lastCard\n");
} else {
  eliminarCartas("./lastCard");
}
//listFiles("./mazo");
repartirCartas();
return 0;
}
