/*------------------------------------------------------------------------------
Proyecto Shell de UNIX. Sistemas Operativos
Grados I. Inform�tica, Computadores & Software
Dept. Arquitectura de Computadores - UMA

Algunas secciones est�n inspiradas en ejercicios publicados en el libro
"Fundamentos de Sistemas Operativos", Silberschatz et al.

Para compilar este programa: gcc ProyectoShell.c ApoyoTareas.c -o MiShell
Para ejecutar este programa: ./MiShell
Para salir del programa en ejecuci�n, pulsar Control+D
------------------------------------------------------------------------------*/

#include "ApoyoTareas.h" // Cabecera del m�dulo de apoyo ApoyoTareas.c
 
#define MAX_LINE 256 // 256 caracteres por l�nea para cada comando es suficiente
#include <string.h>  // Para comparar cadenas de cars. (a partir de la tarea 2)

job *tareas;

void manejador(int senal){
  block_SIGCHLD();
  job *item;
  int status;
  int info;
  int pid_wait = 0;
  enum status status_res;

  for(int i = 1; i <= list_size(tareas); i++){ //recorremos toda la lista
    item = get_item_bypos(tareas, i);
    pid_wait = waitpid(item->pgid, &status, WUNTRACED | WNOHANG); //si cambia de valor 0, ha recogido un hijo
    if(pid_wait == item->pgid){
      status_res = analyze_status(status, &info);
      if(status_res == SUSPENDIDO){
        printf("\nComando %s ejecutado en segundo plano con PID %d ha suspendido su ejecucion\n",
         item->command, item->pgid);
        item->ground = DETENIDO; 
      } else if(status_res == FINALIZADO){
        printf("\nComando %s ejecutado en segundo plano con PID %d ha concluido\n",
         item->command, item->pgid);
        delete_job(tareas, item);
      }
    }
  }
  unblock_SIGCHLD();
}

// --------------------------------------------
//                     MAIN          
// --------------------------------------------

int main(void)
{
  char inputBuffer[MAX_LINE]; // B�fer que alberga el comando introducido
  int background;         // Vale 1 si el comando introducido finaliza con '&'
  char *args[MAX_LINE/2]; // La l�nea de comandos (de 256 cars.) tiene 128 argumentos como m�x
                          // Variables de utilidad:
  int pid_fork, pid_wait; // pid para el proceso creado y esperado
  int status;             // Estado que devuelve la funci�n wait
  enum status status_res; // Estado procesado por analyze_status()
  int info;		      // Informaci�n procesada por analyze_status()

  job *item;
  int primerplano = 0;

  ignore_terminal_signals(); //ignoramos señales del terminal
  signal(SIGCHLD, manejador); //manejador de los hijos (cuando se suspende y segundos planos)
  tareas = new_list("Tareas");

  while (1) // El programa termina cuando se pulsa Control+D dentro de get_command()
  {   		
    printf("COMANDO->");
    fflush(stdout);
    get_command(inputBuffer, MAX_LINE, args, &background); // Obtener el pr�ximo comando
    if (args[0]==NULL) continue; // Si se introduce un comando vac�o, no hacemos nada

    if(!strcmp(args[0], "cd")){
      chdir(args[1]); //args 1 es el proximo directorio que quiero ir
      continue;
    }
    if (!strcmp(args[0], "logout")){ //salida de la shell
      exit(0);
    }
    if(!strcmp(args[0], "jobs")){
      block_SIGCHLD();
      print_job_list(tareas); //imprime la lista de ejecuciones suspendidas y de sgundo plano
      unblock_SIGCHLD();
      continue;
    }
    if(!strcmp(args[0], "bg")){ // pasar a segundo plano un comando suspendido
      block_SIGCHLD();
      int pos = 1;
      if(args[1] != NULL){
        pos = atoi(args[1]);
      }
      item = get_item_bypos(tareas, pos);
      if((item != NULL) && (item->ground == DETENIDO)){
        item->ground = SEGUNDOPLANO;
        killpg(item->pgid, SIGCONT);
      }
      unblock_SIGCHLD();
      continue;
    }
    if(!strcmp(args[0], "fg")){ // pasar comando de primer plano a suspendido o segundo plano
      block_SIGCHLD();
      int pos = 1;
      primerplano = 1;
      if(args[1] != NULL){
        pos = atoi(args[1]);
      }
      item = get_item_bypos(tareas, pos);
      if(item != NULL){
        set_terminal(item->pgid);
        if(item->ground == DETENIDO){
          killpg(item->pgid, SIGCONT);
        }
        pid_fork = item->pgid;
        delete_job(tareas, item);
      }
      unblock_SIGCHLD();
    }

   if(!primerplano) { //controlar si hay algun comando en primer plano
    pid_fork = fork();
   }

   if(pid_fork > 0) { //Padre (el valor es el pid del hijo)
    if(background == 0){ // Primer plano
      waitpid(pid_fork, &status, WUNTRACED); //esperamos al hijo (Status: si finaliza bien, con error...)
      set_terminal(getpid()); //restauramos señal del terminal
      status_res = analyze_status(status, &info);
      if(status_res == SUSPENDIDO){
        block_SIGCHLD();
        item = new_job(pid_fork, args[0], DETENIDO);
        add_job(tareas, item);
        printf("\nComando %s ejecutado en primer plano con pid %d. Estado %s. Info %d\n", 
        args[0], pid_fork, status_strings[status_res], info);
        unblock_SIGCHLD();
      } else if(status_res == FINALIZADO){
        if(info != 255){ // Si existe fallo no se imprime
          printf("\nComando %s ejecutado en primer plano con pid %d. Estado %s. Info %d\n", 
          args[0], pid_fork, status_strings[status_res], info);
        }
      }
      primerplano = 0;  // hemos terminado este proceso de primer plano asi puede entrar otro comando
    } else { //Segundo plano
      block_SIGCHLD();
      item = new_job(pid_fork, args[0], SEGUNDOPLANO);
      add_job(tareas, item);
      printf("\nComando %s ejecutado en segundo plano con pid %d.\n",args[0], pid_fork);
      unblock_SIGCHLD();
    }


   } else { //Hijo (ejecuta siempre el comando)
    new_process_group(getpid());
    if(background == 0){ //si es primer plano el hijo toma el termina
      set_terminal(getpid());
    }
    restore_terminal_signals();
    execvp(args[0], args); //libreria para los comandos de linux
    printf("\nError. Comando %s no encontrado\n", args[0]);
    exit(-1); // salimos con error si  no se ejecuta el execvp
   }
  } // end while
}


