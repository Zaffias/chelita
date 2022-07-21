#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/wait.h>
//Reemplazar esta funcion por mi querido ft_split :)
char **get_input(char *input){
    int words = 2;
    int i = 0;
    char *word;
    char *delim = " ";
    
    char **command = malloc(10 * sizeof(char*));
    if (command == NULL)
        exit(1);
    // Usamos el strtok MAN STRTOK, porque no tengo el code del split T.T
    word = strtok(input, delim);
    //Strtok devuelve NULL una vez ha extraido todas las palabras del string
    while(word != NULL){
        //Metemos la palabara en el array de strings
        command[i] = word;
        //Nos vemos a la siguiente posicion del array de strings
        i++;
        //Segun el MAN si llamamos a strtok con el mismo string con el que ha sido llamado
        //en la llamada anterior, debemos pasarle NULL
        word = strtok(NULL, delim);
    }
    // Añadimos NULL a la ultima posicion del Array para poder iterar sobre el
    command[i] = NULL;
    return command;
}


int main(){
    char **command;
    char *input;
    pid_t child_pid;
    
    int stat_loc;
    // Inicializamos el bucle en el que se va a ejecutar la shell
    while(1){
        // Pillamos input de la consola
        input = readline("🦍> ");
        // Pocesamos el input en un array de strings que sea leible por execvp
        command = get_input(input);
        // Hacemos un fork al proceso actual para crear un proceso hijo
        child_pid = fork();
        // Si el fork falla salimos del programa con un error
        if (child_pid < 0)
            return 1;
        // Si child_pid es 0 estamos en el proceso hijo (No tiene otro hijo por lo que su child_pid es 0)
        // En este proceso hijo ejecutaremos las llamadas al sistema para que una vez acabadas no se cierre el proceso padre 
        // (Solo muere el hijo) :((()))
        if (child_pid == 0){
            // Llamamos a execvp, el primer argumento es el nombre del programa, el segundo son las flags y opciones disponibles
            execvp(command[0], command);
        }
        else {
            //Esperamos a que el proceso hijo acabe
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }
        // Liberamos la memoria reservada para el commando y el input
        free(input);
        free(command);
    }

    return 0;
}