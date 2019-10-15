# SO

Integrantes:
><p>-Harold Caballero, rol: 201773602-k</p>
><p>-Guillermo Vasquez, rol: 201773552-k</p>

Notas:

El código fuente está en 'tarea.c', para compilar y ejecutar se debe escribir el comando 'make tarea' dentro de la carpeta del repositorio 'SO_tarea1'. Esto compilará, luego al ejecutar el comando 'make run' se ejecutará el compilado 'main'.
Para eliminar todos los directorios creados por el programa, se puede ejecutar el comando 'make rm', que también eliminará el ejecutable.

Al ejecutar el programa, se crearán los directorios:
><p>-lastCard</p>
><p>-mazo</p>
><p>-player1</p>
><p>-player2</p>
><p>-player3</p>
><p>-player4</p>
cada uno contendrá archivos que representan cartas (108 en total).

Además se imprimirá por pantalla las cartas que se van escogiendo para cada jugador y el directorio base.

Consideraciones:
-Los inputs que se soliciten mediante la terminal deben ser enteros positivos, porque al entregar un caracter el programa deja de funcionar.
-Al ejecutar el programa se juega como jugador1 y los otros 3 jugadores son procesos que juegan por sí solos, estos jugadores revisan su mazo y juegan la primera carta que cumpla todas las restricciones. 
