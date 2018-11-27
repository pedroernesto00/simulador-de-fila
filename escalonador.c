#include <stdlib.h>
#include <stdio.h>

typedef struct ponteiro {
    int chave, valor, tamanho;
    struct ponteiro *proximo;
} Fila_FIFO;

void f_inicializar(Fila_FIFO **f) {
    *f = NULL;

}

int f_inserir (Fila_FIFO **f, int chave, int valor) {
    Fila_FIFO *novo, *aux;

    novo = (Fila_FIFO*)malloc(sizeof(Fila_FIFO));

    if (novo == NULL)
        return 0;

    if (*f == NULL) {//Inserir no inicio da fila
        *f = novo;
        novo->proximo = NULL;
        novo->chave = chave;
        novo->valor = valor;
        novo->tamanho++;
        return 1;
    }

    aux = *f;

    while ((aux->chave != chave)&&(aux->proximo != NULL))
        aux = aux->proximo;

    if (aux->chave == chave)
        return 0;

    aux->proximo = novo;
    novo->proximo = NULL;
    novo->chave = chave;
    novo->valor = valor;
    novo->tamanho++;
    return 1;
}

int f_obter_proxima_chave (Fila_FIFO **f) {
    Fila_FIFO *aux;

    if (*f == NULL)
        return -1;

    aux = *f;
    *f = (*f)->proximo;
    (*f)->tamanho--;

    return aux->chave;

}

int f_consultar_proxima_chave (Fila_FIFO **f) {
    if (*f == NULL)
        return -1;
    return (*f)->chave;
}

int f_consultar_proximo_valor(Fila_FIFO **f) {
    if (*f == NULL)
        return -1;
    return (*f)->valor;
}

int f_num_elementos (Fila_FIFO **f) {
    if ((*f) == NULL)
        return 0;
    return (*f)->tamanho;

}

int f_consultar_chave_por_posicao (Fila_FIFO **f, int posicao) {
    int elementos, i;
    Fila_FIFO *aux;

    elementos = f_num_elementos (f);

    if ((*f == NULL) || (posicao < 1) || (posicao > elementos))
        return -1;

    aux = *f;

    for (i = 1; i < posicao; i++)
        aux = aux->proximo;

    return aux->chave;
}

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

typedef struct ponteiros {
    int conta, classe, timer, caixa;
    struct ponteiros *direito, *esquerdo;

}Log;

void log_inicializar(Log **l) {
    *l = NULL;

}

void log_registrar(Log **l, int conta, int classe, int timer, int caixa) {
    Log *aux, *novo;

    if (*l == NULL) {
        novo = (Log*)malloc(sizeof(Log));
        *l = novo;
        novo->conta = conta;
        novo->classe = classe;
        novo->timer = timer;
        novo->caixa = caixa;
        novo->esquerdo = NULL;
        novo->direito = NULL;
    } else {
        aux = *l;

        if (classe < aux->classe)
            log_registrar(&((*l)->esquerdo),conta,classe,timer,caixa);
        else
            log_registrar(&((*l)->direito),conta,classe,timer,caixa);
    }
}

float log_media_por_classe(Log **l, int classe) {
    float media, qntd, soma;

    qntd = log_obter_contagem_por_classe(l, classe);
    soma = log_obter_soma_por_classe(l, classe);
    media = (soma / qntd);
    return media;
}

int log_obter_soma_por_classe(Log **l, int classe) {
    int soma = 0;
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
            soma += aux->timer;
            aux = aux->direito;
        }}

    return soma;
}

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
            qntd += 1;
            aux = aux->direito;
        }}
    return qntd;
}

typedef struct {
    int caixas, delta_t, disciplina[10];
    Fila_FIFO *f[5];
}Escalonador;

void e_inicializar (Escalonador *e, int caixas, int delta_t, int n_1, int n_2, int n_3, int n_4, int n_5) {

    for (int i = 0; i < 5; i++)
        f_inicializar(&(e->f[i]));

    e->caixas = caixas;
    e->delta_t = delta_t;
    e->disciplina[5] = e->disciplina[0] = n_1;
    e->disciplina[6] = e->disciplina[1] = n_2;
    e->disciplina[7] = e->disciplina[2] = n_3;
    e->disciplina[8] = e->disciplina[3] = n_4;
    e->disciplina[9] = e->disciplina[4] = n_5;
}

int e_inserir_por_fila (Escalonador *e, int classe, int num_conta, int qtde_operacoes) {

    if ((classe < 1) || (classe > 5))
        return -1;
    return f_inserir(&(e->f[classe-1]),num_conta, qtde_operacoes);


}

int e_obter_prox_num_conta(Escalonador *e) {
    int conta, i;

    for (i = 0; i < 5; i++)
		if ((e->disciplina[i] >= 1) && (e->f[i] != NULL)) {
			conta = f_obter_proxima_chave(&(e->f[i]));
            e->disciplina[i] -= 1;
			return conta;
		}

    for (i = 0; i < 5; i++)
		if (f_num_elementos(&(e->f[i])) != 0) {
            e->disciplina[i] = e->disciplina[i+5];
			return e_obter_prox_num_conta(&e);
    }
    return -1;
}


int e_consultar_qtde_clientes (Escalonador *e) {
    int total = 0;
    for (int i = 0; i < 5; i++)
        total += f_num_elementos(&(e->f[i]));
    return total;
}

int main (void) {
    Escalonador e;
    int x,n;

    e_inicializar (&e, 10, 3, 1, 2, 1, 1, 1);

    e_inserir_por_fila(&e,1,418465,2);
    e_inserir_por_fila(&e,1,7,2);
    e_inserir_por_fila(&e,2,2,2);
    e_inserir_por_fila(&e,3,3,2);
    e_inserir_por_fila(&e,4,4,2);
    e_inserir_por_fila(&e,5,5,2);

    //x = e_obter_prox_num_conta(&e);

    //printf("%i\n", x);
    n = e_consultar_qtde_clientes(&e);

    for (int i = 0; i < n; i++) {
        x = e_obter_prox_num_conta(&e);

        printf("%i\n", x);
    }
    return 0;

}

