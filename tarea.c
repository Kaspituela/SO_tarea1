#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void listFiles(char *dir){
    DIR *dir_actual;
    struct dirent *file;
    dir_actual = opendir(dir);
    if (dir_actual) {
      while ((file = readdir(dir_actual)) != NULL) {
        printf("%s\n", file->d_name);
        printf("%ld\n", telldir(dir_actual));
      }
      closedir(dir_actual);
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
  fclose(fp);
}


int main() {
  char *name = "cartas.txt";
struct stat st = {0};
//Create cards
if (stat("./mazo",&st) < 0){
  //Create directory
  if (mkdir("./mazo", 0777) == -1){
    printf("Error para crear directorio mazo\n");
  } else{
    char s[100];
    createCards(name,"mazo");
    printf(" %s\n", getcwd(s, 100));
    chdir("..");
  }
}

//Create players
if (stat("./player1",&st) < 0) {
  if (mkdir("./player1", 0777) == -1)
    printf("Error para crear directorio player1\n");
}
if (stat("./player2",&st) < 0) {
  if (mkdir("./player2", 0777) == -1)
    printf("Error para crear directorio player2\n");
}
if (stat("./player3",&st) < 0) {
  if (mkdir("./player3", 0777) == -1)
    printf("Error para crear directorio player3\n");
}
if (stat("./player4",&st) < 0) {
  if (mkdir("./player4", 0777) == -1)
    printf("Error para crear directorio player4\n");
}

//Create lastCard
if (stat("./lastCard",&st) < 0) {
  if (mkdir("./lastCard", 0777) == -1)
    printf("Error para crear directorio lastCard\n");
}
listFiles("./mazo");
  return 0;
}
