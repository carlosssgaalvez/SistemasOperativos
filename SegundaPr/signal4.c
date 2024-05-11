#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void hijo(int sig) {
  printf("Hijo: Mi padre tiene malas intenciones conmigo... ME QUIERE MATAR!!\n");
  sleep(2);
  printf("Hijo: Contraatacando!!\n");
  kill(getppid(), SIGINT);
}

int main(int argc, char *argv[]) {
  pid_t pidfork;
  int status;

  pidfork = fork(); 
  if (pidfork == 0) { 
    int i=0;
    signal(SIGINT, hijo); 
    printf("Hijo: pid %d: ejecutando...\n",getpid());
    while(i<3) {sleep(1); printf("Hijo: %d seg\n",++i); }
  } else { 
    sleep(2);
    printf("\nPadre: pid %d: mandando señal SIGINT\n",getpid());
    kill(pidfork,SIGINT);
    while (pidfork != wait(&status));
    if (WIFEXITED(status)) { 
      printf("El proceso terminó con estado %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) { 
      printf("El proceso terminó al recibir la señal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) { 
      printf("El proceso se ha parado al recibir la señal %d\n", WSTOPSIG(status));
    }
  }
  exit(10); 
}
