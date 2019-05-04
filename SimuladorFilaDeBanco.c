/*PROGRAMA FEITO POR: 
DAVID LAEL BARROSO TEIXEIRA - 415198
PEDRO ERNESTO DE OLIVEIRA PRIMO - 418465
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./fila_fifo.h"
#include "./logtree.h"
#include "./escalonador.h"

int main (int narg, char *argv[]) {
    Escalonador e;
   	char entrada[18], saida[15];

    

 	/*Pega-se o segundo elemento do vetor argv, e compara seu tamanho
 	Depois, faz concatenações a depender do tamanho para formar o nome do arquivo de entrada e de saída
 	*/
    if (strlen(argv[1]) == 1) {
        strcpy(entrada, "entrada-000");
        strcpy(saida, "saida-000");
	
    } else if (strlen(argv[1]) == 2) {
        strcpy(entrada, "entrada-00");
        strcpy(saida, "saida-00");
    } else if (strlen(argv[1]) == 3) {
        strcpy(entrada, "entrada-0");
        strcpy(saida, "saida-0");
    } else if (strlen(argv[1]) == 4) {
        strcpy(entrada, "entrada-");
        strcpy(saida, "saida-");
    }
    

    strcat(entrada, argv[1]);
    strcat(saida, argv[1]);
    strcat(entrada, ".txt");
    strcat(saida, ".txt");

    e_rodar(&e, entrada, saida);
    return 0;
}
