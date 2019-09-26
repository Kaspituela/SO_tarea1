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
        printf("Error al abrir directorio\n");
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
        sprintf(nombre, "+2 %s2.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);

        //reversas (R)
        sprintf(nombre, "R %s.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);
        sprintf(nombre, "R %s2.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);

        //saltos (S)
        sprintf(nombre, "S %s.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);
        sprintf(nombre, "S %s2.txt", colores[i]);
        carta = fopen(nombre, "w");
        fclose(carta);

        //cambios de color (color)
        sprintf(nombre, "color%d.txt", i+1);
        carta = fopen(nombre, "w");
        fclose(carta);

        //+4
        sprintf(nombre, "+4, %d.txt", i+1);
        carta = fopen(nombre, "w");
        fclose(carta);

        //del 1 al 9
        for(j=1; j<10; j++)
        {
            sprintf(nombre, "%d %s.txt", j, colores[i]);
            carta = fopen(nombre, "w");
            fclose(carta);
            sprintf(nombre, "%d %s2.txt", j, colores[i]);
            carta = fopen(nombre, "w");
            fclose(carta);
        }
    }
    chdir("..");
}


int main() {
    srand(time(0));
    char *name = "cartas.txt";
    struct stat st = {0};
    //Se crea mazo
    if (stat("./mazo",&st) >= 0) 
        system("rm -rf ./mazo");
    if (mkdir("./mazo", 0777) == -1){
        printf("Error para crear directorio mazo\n");
        exit(0);
    }

    //Create cards
    char s[100];
    createCards(name,"mazo");
    printf(" %s\n", getcwd(s, 100));

    //Create players
    if (stat("./player1",&st) < 0) {
        if (mkdir("./player1", 0777) == -1){
            printf("Error para crear directorio player1\n");
            exit(0);
    }
    } else
        eliminarCartas("./player1");

    if (stat("./player2",&st) < 0) {
        if (mkdir("./player2", 0777) == -1){
            printf("Error para crear directorio player2\n");
            exit(0);
        }
    } else
      eliminarCartas("./player2");

    if (stat("./player3",&st) < 0) {
        if (mkdir("./player3", 0777) == -1){
            printf("Error para crear directorio player3\n");
            exit(0);
        }
    } else
      eliminarCartas("./player3");

    if (stat("./player4",&st) < 0) {
        if (mkdir("./player4", 0777) == -1){
            printf("Error para crear directorio player4\n");
            exit(0);
        }
    } else
      eliminarCartas("./player4");

    //Create lastCard
    if (stat("./lastCard",&st) < 0) {
        if (mkdir("./lastCard", 0777) == -1){
            printf("Error para crear directorio lastCard\n");
            exit(0);
        }
    } else
      eliminarCartas("./lastCard");

    //listFiles("./mazo");
    repartirCartas();
    return 0;
}
