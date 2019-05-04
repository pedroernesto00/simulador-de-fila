#include <stdlib.h>
#include <stdio.h>
#include "./fila_fifo.h"

//Iniciando a fila, ponteiro head aponta pra nulo
void f_inicializar(Fila_FIFO **f) {
    *f = NULL;

}

//Insere elementos na fila, sempre no fim
int f_inserir (Fila_FIFO **f, int chave, int valor) {
    Fila_FIFO *novo, *aux;

    novo = (Fila_FIFO*)malloc(sizeof(Fila_FIFO));

    if (novo == NULL) //Se não tiver espaço na memoria, retorna 0
        return 0;

    if (*f == NULL) {//Inserir na fila vazia
        *f = novo;
        novo->proximo = NULL;
        novo->chave = chave;
        novo->valor = valor;
        return 1;
    }

    aux = *f;

    while ((aux->chave != chave)&&(aux->proximo != NULL)) //Percorrendo a fila até encontrar uma chave igual ou até chegar ao último elemento
        aux = aux->proximo;

    if (aux->chave == chave) //Se a chave já estiver na fila, retorna 0 (erro)
        return 0;

    //Se não estiver, insere o novo elemento no fim e retorna 1 (êxito)
    aux->proximo = novo;
    novo->proximo = NULL;
    novo->chave = chave;
    novo->valor = valor;
    return 1;
}

//Retira-se o primeiro termo da fila e retorna sua chave
int f_obter_proxima_chave (Fila_FIFO **f) {
    Fila_FIFO *aux;
    int n;

    if (*f == NULL) //Se a fila estiver vazia, retorna -1
        return -1;

    //Se não estiver, armazena-se o valor da chave do primeiro elemento, e o ponteiro cabeça aponta para o próximo do elemento retirado
    aux = *f;
    *f = (*f)->proximo;
    n = aux->chave;
    free(aux);
    return n;

}

//Mostra a chave do primeiro elemento, sem retirá-lo da fila
int f_consultar_proxima_chave (Fila_FIFO **f) {
    if (*f == NULL)
        return -1;
    return (*f)->chave;
}

//Mostra o valor do próximo elemento, sem retirá-lo da fila
int f_consultar_proximo_valor(Fila_FIFO **f) {
    if (*f == NULL)
        return -1;
    return (*f)->valor;
}

//Mostra a quantidade de elementos na fila
int f_num_elementos (Fila_FIFO **f) {
    int contador = 0;

    Fila_FIFO *aux;

    aux = *f;

    while (aux != NULL) { //Enquanto o elemento não for nulo, o contador é incrementado
        contador++;
        aux = aux->proximo;
    }
    return contador;
}

//Dada uma posição, mostra a chave do elemento correspondente à posição
int f_consultar_chave_por_posicao (Fila_FIFO **f, int posicao) {
    int elementos, i;
    Fila_FIFO *aux;

    elementos = f_num_elementos (f);

    if ((*f == NULL) || (posicao < 1) || (posicao > elementos)) //Se a fila estiver vazia ou a posição solicitada for menor que 1 
        return -1;                                              //ou maior que a quantidade de elementos, retorna -1

    aux = *f;

    //Se não, percorre a fila até a posição determinada
    for (i = 1; i < posicao; i++)
        aux = aux->proximo;

    return aux->chave;
}

//Dada uma posição, mostra o valor do elemento correspondente à posição
//Funciona da mesma forma que f_consultar_chave_por_posição
int f_consultar_valor_por_posicao (Fila_FIFO **f, int posicao){
    int elementos, i;
    Fila_FIFO *aux;

    elementos = f_num_elementos (f);

    if ((*f == NULL) || (posicao < 1) || (posicao > elementos))
        return -1;

    aux = *f;

    for (i = 1; i < posicao; i++)
        aux = aux->proximo;

    return aux->valor;
}
