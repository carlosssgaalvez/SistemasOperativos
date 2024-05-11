#include <stdio.h>

int main() {
int var_entera;
int *p;
var_entera =10;
p = 0;
printf("\n");

printf("Direccion de memoria de la variable var_entera = %lu \n", 
&var_entera);
printf("Valor de la variable var_entera = %d \n\n", var_entera);
printf("Direccion de memoria que alberga al puntero p = %p \n", &p);
printf("Direccion de memoria ques es apuntada por el puntero p = %lu \n", 
&p);
printf("Direccion de memoria que es apuntada por el puntero p = %lu \n", 
p);
printf("contenido de la direccion apuntada por el puntero p = %d \n", *p);
printf("\n");
}
