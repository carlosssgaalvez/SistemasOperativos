#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int varglobal;

void main(int argc, char *argv[]) {
  pid_t pidfork;
  int status;

  varglobal = 1;

  pidfork = fork();
  if (pidfork == 0) { /* proceso hijo */
    printf("Hijo: valor de la variable antes de modificación %d\n", varglobal);
    varglobal = 500;
    printf("Hijo: valor de la variable despues de modificación %d\n", varglobal);
  } else {/* proceso padre */ 
    printf("Padre: valor de la variable antes de wait %d\n", varglobal);
    while (pidfork != wait(&status));
    printf("Padre: valor de la variable despues de wait %d\n", varglobal);
    if (WIFEXITED(status)) { // el proceso ha terminado con un exit()
      printf("El proceso terminó con estado %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) { // el proceso ha terminado por la recepción de una señal
      printf("El proceso terminó al recibir la señal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) { // el proceso se ha parado por la recepción de una señal
      printf("El proceso se ha parado al recibir la señal %d\n", WSTOPSIG(status));
    }
  }
  sleep(5);
  exit(0); 
}