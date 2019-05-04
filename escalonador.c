#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./fila_fifo.h"
#include "./logtree.h"
#include "./escalonador.h"

#define NUM_CAIXAS_MAXIMO 10

//Inicializa o Escalonador e atribui os parâmetros
void e_inicializar (Escalonador *e, int caixas, int delta_t, int n_1, int n_2, int n_3, int n_4, int n_5) {
    int i;


    for (i = 0; i < 5; i++) //Inicializando as 5 filas, cada posição do vetor é uma fila
        f_inicializar(&(e->f[i]));

    e->caixas = caixas; 
    e->delta_t = delta_t;
    e->disciplina[0] = n_1; //O vetor disciplina representa a disciplina de atendimento
    e->disciplina[1] = n_2;
    e->disciplina[2] = n_3;
    e->disciplina[3] = n_4;
    e->disciplina[4] = n_5;


    for (i = 0; i < 5; i++) //Vetor reserva que recebe os mesmos valores do vetor disciplina
        e->n[i] = e->disciplina[i];
}

//Insere o numero da conta e a quantidade de operações do cliente na fila que representa sua classe
int e_inserir_por_fila (Escalonador *e, int classe, int num_conta, int qtde_operacoes) {

    if ((classe < 1) || (classe > 5)) //Se a classe for diferente das 5, o programa retorna Erro
        return -1;
    return f_inserir(&(e->f[classe-1]),num_conta, qtde_operacoes); //Insere as informações na fila, de acordo com a função f_inserir

}

//Retira o próximo cliente a ser atendido da fila, e retorna seu número da conta
int e_obter_prox_num_conta(Escalonador *e) {
    int conta, i;

    //Primeira Rodada
    for (i = 0; i < 5; i++) 
		if ((e->disciplina[i] >= 1) && (e->f[i] != NULL)) { //Vai de acordo com a Disciplina de Atendimento
			conta = f_obter_proxima_chave(&(e->f[i]));
            e->disciplina[i]--;
			return conta;
		}

	//Após a primeira rodada de atendimento, o vetor disciplina recebe os valores do vetor reserva, para que assim possa haver ciclos de atendimento
    for (i = 0; i < 5; i++)
		if (f_num_elementos(&(e->f[i])) != 0)
    		e->disciplina[i] = e->n[i];

    //Ciclo de atendimento
    for (i = 0; i < 5; i++)
		if (f_num_elementos(&(e->f[i])) != 0)
    		return e_obter_prox_num_conta(e);
    
    return -1;
}

//Mostra o valor da conta do próximo cliente a ser atendido, sem retirá-lo da fila
//Funciona da mesma forma que a função e_obter_prox_conta
int e_consultar_prox_num_conta (Escalonador *e) {
    int conta,i;

    for (i = 0; i < 5; i++)
	if ((e->disciplina[i] >= 1) && (e->f[i] != NULL)) {
		conta = f_consultar_proxima_chave(&(e->f[i]));
		return conta;
		}

    for (i = 0; i < 5; i++)
		if (f_num_elementos(&(e->f[i])) != 0)
            e->disciplina[i] = e->n[i];

    for (i = 0; i < 5; i++)
		if (f_num_elementos(&(e->f[i])) != 0)
    		return e_consultar_prox_num_conta(e);
    return -1;
}

//Mostra a quantidade de operações do próximo cliente a ser atendido, sem retirá-lo da fila
//Funciona da mesma forma que a função e_obter_prox_conta
int e_consultar_prox_qtde_oper (Escalonador *e) {
    int operacoes,i;

    for (i = 0; i < 5; i++)
	if ((e->disciplina[i] >= 1) && (e->f[i] != NULL)) {
		operacoes = f_consultar_proximo_valor(&(e->f[i]));
			return operacoes;
		}

    for (i = 0; i < 5; i++)
	if (f_num_elementos(&(e->f[i])) != 0)
            e->disciplina[i] = e->n[i];
    for (i = 0; i < 5; i++)
	if (f_num_elementos(&(e->f[i])) != 0)
    	    return e_consultar_prox_qtde_oper(e);
    return -1;
}

//Mostra a classe do próximo cliente a ser atendido, sem retirá-lo da fila
//Funciona da mesma forma que a função e_obter_prox_conta
int e_consultar_prox_fila (Escalonador *e) {
    int i;

    for (i = 0; i < 5; i++)
	if ((e->disciplina[i] >= 1) && (e->f[i] != NULL))
            return i+1;
    
    for (i = 0; i < 5; i++)
	if (f_num_elementos(&(e->f[i])) != 0)
            	e->disciplina[i] = e->n[i];
    for (i = 0; i < 5; i++)
	if (f_num_elementos(&(e->f[i])) != 0)
    		return e_consultar_prox_fila(e);
    return -1;

}

//Mostra a quantidade de clientes a serem atendidos
int e_consultar_qtde_clientes (Escalonador *e) {
    int i, total = 0;
    for (i = 0; i < 5; i++)
        total += f_num_elementos(&(e->f[i])); //O total é a soma da quantidade de clientes restantes em cada fila
    return total;
}

//Mostra o tempo que o próximo cliente levará para realizar todas as operações que fará
int e_consultar_tempo_prox_cliente (Escalonador *e) {
    if ((e->f[0] == NULL)&&(e->f[1] == NULL)&&(e->f[2] == NULL)&&(e->f[3] == NULL)&&(e->f[4] == NULL)) //Se não há clientes em mais nenhuma fila, retorna -1
        return -1;

    return e_consultar_prox_qtde_oper(e)*(e->delta_t); //Quantidade de operações vezes o tempo de cada operãção
}

//Essa função serve para calcular qual caixa foi mais rápido
//Assim saberemos para qual caixa o próximo cliente deverá ser chamado
int e_menor_tempo (int *vetor, int tamanho) {
    int menor = 0, i;

    for (i = 0; i < tamanho; i++)
        if (vetor[i] < vetor[menor])
            menor = i;

    return menor;

}

//No fim, deveremos saber qual caixa demorou mais tempo para atender todos os clientes
//Como os caixas atendem simultaneamente, o maior tempo será o tempo total de atendimento
int e_maior_tempo (int *vetor, int tamanho) {
    int maior = 0, i;

    for (i = 0; i < tamanho; i++)
        if (vetor[i] > maior)
            maior = vetor[i];

    return maior;
}

//Função para ler o arquivo .txt e passar os valores para o programa
int e_conf_por_arquivo (Escalonador *e, char *nome_arq_conf) {
    char tipo_conta[8]; //Vetor para armazenar o nome das classes
    int conta, operacoes, n_1, n_2, n_3,n_4,n_5,caixas,delta_t;

    FILE *arquivo;

    arquivo = fopen(nome_arq_conf, "r");

    if (arquivo == NULL) //Se o arquivo não for achado, retorna -1
        return -1;

    //Lendo as 3 primeiras linhas do arquivo e atribuindo os valores
    fscanf(arquivo,"qtde de caixas = %d\n", &caixas);
    fscanf(arquivo,"delta t = %d\n", &delta_t);
    fscanf(arquivo,"disciplina de escalonamento = {%d,%d,%d,%d,%d}\n", &n_1, &n_2, &n_3, &n_4, &n_5);

    e_inicializar(e,caixas,delta_t,n_1,n_2,n_3,n_4,n_5);


    //O resto das linhas seguem um padrão, logo usamos While até o arquivo acabar
    while (fscanf(arquivo,"%s - conta %d - %d operacao(oes)\n", tipo_conta, &conta, &operacoes) != EOF) {
        //Compara o conteudo do vetor tipo_conta com as contas, e entao insere na fila adequada
        if (strcmp(tipo_conta, "Premium") == 0)
            e_inserir_por_fila(e,1,conta,operacoes);
        else if (strcmp(tipo_conta, "Ouro") == 0)
            e_inserir_por_fila(e,2,conta,operacoes);
        else if (strcmp(tipo_conta, "Prata") == 0)
            e_inserir_por_fila(e,3,conta,operacoes);
        else if (strcmp(tipo_conta, "Bronze") == 0)
            e_inserir_por_fila(e,4,conta,operacoes);
        else if (strcmp(tipo_conta, "Comum") == 0)
            e_inserir_por_fila(e,5,conta,operacoes);
}
    fclose(arquivo); //Fecha o arquivo
    return 1;
}

//Função para gerar o arquivo de saída
void e_rodar (Escalonador *e, char *nome_arq_in, char *nome_arq_out) {
    int i, n, num_caixa, tempo, operacoes, conta, fila, qntd_caixas[NUM_CAIXAS_MAXIMO], qntd_ops[5];
    char *tipo_conta[5] = {"Premium","Ouro","Prata","Bronze","Comum"};
    float media_op, op_total, qntd_classe;
    Log *contador_de_tempo;

    log_inicializar(&contador_de_tempo);

    FILE *arquivo;

    e_conf_por_arquivo(e,nome_arq_in);

    n = e_consultar_qtde_clientes(e);

    arquivo = fopen(nome_arq_out, "w");

    /* Sabe-se que o máximo de caixas é 10, logo criamos um vetor de tamanho 10, para armazenar o tempo
    de atendimento de cada caixa
    */
    for (i = 0; i < NUM_CAIXAS_MAXIMO; i++) 
        qntd_caixas[i] = 0; //No início, todos os caixas recebem tempo 0

    /*Criamos um vetor de 5 posições para armazenar o total de operações que os clientes de caixa uma
    das 5 classes fizeram 
    */

    for (i = 0; i < 5; i++)
        qntd_ops[i] = 0; //No início, todos recebem 0 


    while (n > 0) { //Enquanto há clientes nas filas
        num_caixa = e_menor_tempo(qntd_caixas, e->caixas); //O caixa para qual o cliente será chamado é o de menor tempo
        tempo = e_consultar_tempo_prox_cliente(e); 
        operacoes = e_consultar_prox_qtde_oper(e);

        fila = e_consultar_prox_fila(e);
        conta = e_obter_prox_num_conta(e);

        //Escreve no arquivo o tempo, o caixa, a classe, a conta e a quantidade de operãções do próximo cliente
        fprintf(arquivo,"T = %d min: Caixa %d chama da categoria %s cliente da conta %d para realizar %d operacao(oes).\n",qntd_caixas[num_caixa], num_caixa + 1, tipo_conta[fila-1], conta, operacoes);

        //Informações são registradas na árvore binária
        log_registrar(&contador_de_tempo, conta,fila,qntd_caixas[num_caixa], num_caixa + 1); 
        qntd_caixas[num_caixa] += tempo; //O caixa que atendeu o cliente recebe o tempo do cliente
        qntd_ops[fila - 1] += operacoes; //A classe do cliente recebe a quantidade de operações realizadas
        n--; //O cliente vai embora
    }

    //Escreve no arquivo o tempo total de atendimento, representado pelo caixa que demorou mais tempo
    fprintf(arquivo,"Tempo total de atendimento: %d minutos.\n", e_maior_tempo(qntd_caixas, e->caixas));

    //Escreve no arquivo o tempo médio de espera de cada tipo de classe, exceto Comum
    for (i = 0; i < 4; i++)
            fprintf(arquivo,"Tempo medio de espera dos %d clientes %s: %.2f\n",log_obter_contagem_por_classe(&contador_de_tempo, i + 1),tipo_conta[i],log_media_por_classe(&contador_de_tempo, i + 1));

    //Escreve no arquivo o tempo médio de espera dos clientes da classe Comum
    fprintf(arquivo,"Tempo medio de espera dos %d clientes Comuns: %.2f\n",log_obter_contagem_por_classe(&contador_de_tempo, 5),log_media_por_classe(&contador_de_tempo, 5));

    //Escreve no arquivo o total médio de operações feitas pelos clientes de cada classe
    for (i = 0; i < 5; i++) {
        op_total = qntd_ops[i];
        qntd_classe = log_obter_contagem_por_classe(&contador_de_tempo, i + 1);
        media_op = op_total/qntd_classe;
        fprintf(arquivo, "Quantidade media de operacoes por cliente %s = %.2f\n", tipo_conta[i], media_op);
    }

    //Escreve no arquivo a quantidade de clientes atendidos por cada caixa
    for (i = 0; i < e->caixas; i++)
        fprintf(arquivo,"O caixa de número %d atendeu %d clientes.\n", i+1, log_obter_contagem_por_caixa(&contador_de_tempo, i+1));

    fclose(arquivo); //Fecha o arquivo

}