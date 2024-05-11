#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void main(int argc, char *argv[]) {

  pid_t pidfork;
  int status;
  pidfork = fork();   // creamos un proceso hijo
  if (pidfork == 0) { /* proceso hijo */
    execlp("ls","ls","-la",NULL);  // sustituimos el código del proceso hijo por el del "ls" con argumento "-la"
    printf("No se ha podido ejecutar el comando.\n");
    exit(-1); 
  } else {/* proceso padre */ 
    while (pidfork != wait(&status));
    if (WIFEXITED(status)) { // el proceso ha terminado con un exit()
      printf("El proceso terminó con estado %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) { // el proceso ha terminado por la recepción de una señal
      printf("El proceso terminó al recibir la señal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) { // el proceso se ha parado por la recepción de una señal
      printf("El proceso se ha parado al recibir la señal %d\n", WSTOPSIG(status));
    }
  }
  exit(0); 
}