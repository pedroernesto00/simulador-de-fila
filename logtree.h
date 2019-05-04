typedef struct ponteiros {
    int conta, classe, timer, caixa;
    struct ponteiros *direito, *esquerdo;

}Log;

void log_inicializar(Log **l);
void log_registrar(Log **l, int conta, int classe, int timer, int caixa);
int log_obter_soma_por_classe(Log **l, int classe);
int log_obter_contagem_por_classe(Log **l, int classe);
int log_obter_contagem_por_caixa(Log **l, int caixa);
float log_media_por_classe(Log **l, int classe);
