#include <stdio.h>
#include <stdlib.h>
#include "./logtree.h"

//Inicia-se a árvore binária, raiz aponta para nulo
void log_inicializar(Log **l) {
    *l = NULL;

}

//Insere um novo elemento na árvore 
void log_registrar(Log **l, int conta, int classe, int timer, int caixa) {
    Log *aux, *novo;

    if (*l == NULL) { //Inserindo numa árvore vazia
        novo = (Log*)malloc(sizeof(Log));
        *l = novo;
        novo->conta = conta;
        novo->classe = classe;
        novo->timer = timer;
        novo->caixa = caixa;
        novo->esquerdo = NULL;
        novo->direito = NULL;
    } else { //Inserindo numa árovre não vazia
        aux = *l;

        if (classe < aux->classe) //Comparando as classes, sabe-se se o novo elemento será filho esquerdo ou direito
            log_registrar(&((*l)->esquerdo),conta,classe,timer,caixa); 
        else
            log_registrar(&((*l)->direito),conta,classe,timer,caixa); //Convencionado que se a classe for a mesma, deve ser filho direito
    }
}

//Mostra o tempo total de uma classe solicitada
int log_obter_soma_por_classe(Log **l, int classe) {
    int soma = 0;
    Log *aux;

    if (*l == NULL) //Se a árvore estiver vazia, retorna 0
        return 0;

    aux = *l;

    //Enquanto o elemento for diferente de nulo, deve-se checar se a chave fornecida é maior, menor ou igual à classe do elemento
    while (aux != NULL) { 
        if (aux->classe < classe) //Se for maior, deve-se ir para o filho direito do elemento
            aux = aux->direito;
        else if (aux->classe > classe) //Se for menor, deve-se ir para o filho esquerdo do elemento
            aux = aux->esquerdo;
        else if (aux->classe == classe) { //Se for igual, a variável 'soma' é incrementada e deve-se ir para o filho direito
            soma += aux->timer;
            aux = aux->direito;
        }}

    return soma;
}

//Mostra a quantidade de elementos de uma classe solicitada
//Funciona da mesma forma que a função log_obter_soma_por_classe
int log_obter_contagem_por_classe(Log **l, int classe) {
    int qntd = 0;
    Log *aux;

    if (*l == NULL)
        return 0;

    aux = *l;

    while (aux != NULL) {
        if (aux->classe < classe)
            aux = aux->direito;
        else if (aux->classe > classe)
            aux = aux->esquerdo;
        else if (aux->classe == classe) {
            qntd++;
            aux = aux->direito;
        }}
    return qntd;
}
//Mostra a quantidade de elementos de um caixa solicitado
int log_obter_contagem_por_caixa(Log **l, int caixa) {
    int qntd = 0, esquerdo = 0, direito = 0;
    Log *aux;

    if (*l == NULL) //Se a árvore for vazia, retorna-se 0
        return 0;

    aux = *l;

    if (aux->caixa == caixa) //Se o caixa do elemento for igual ao caixa solicitado, a variável 'qntd' é incrementada
        qntd++;
    esquerdo += log_obter_contagem_por_caixa(&((*l)->esquerdo), caixa); //Repete-se o processo para os filhos esquerdos dos elementos
    direito += log_obter_contagem_por_caixa(&((*l)->direito), caixa); //Repete-se o processo para os filhos direitos dos elementos

    return qntd + esquerdo + direito; //retorna a soma total
}

//Mostra a média de tempo de uma classe solicitada
//A média é dada pela divisão do tempo total da classe pela quantidade de pessoas da classe
float log_media_por_classe(Log **l, int classe) {
    float media, qntd, soma;

    qntd = log_obter_contagem_por_classe(l, classe);
    soma = log_obter_soma_por_classe(l, classe);
    media = (soma / qntd);
    return media;
}
