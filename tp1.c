#include "tp1.h"

int Testes(const char* argv[])
{
	int parametro = atoi(argv[3]);      //transforma string em inteiro

    if(parametro != 0 && parametro != 1)    //testa se inteiro do tipo de algoritmo é apenas 0 ou 1
    {
        puts("Opcao de algoritmo invalida!");
        exit(0);
    }
    if(!TestaExtensao(argv[2]))             //testa se extensão do arquivo de saída está correta
    {
        puts("Extensao do arquivo saida invalida!");
        exit(0);
    }
    return parametro;                       //retorna inteiro do tipo de algoritmo
}/* Testes */

int TestaExtensao(const char* entrada)
{
    char *ext;              //string para conter apenas a extensão do arquivo

    if((ext = strrchr(entrada,'.')) != NULL )       //confere se há algo depois do . na string entrada
    {
        if(strcmp(ext,".txt") == 0)                 //confere se o final da string é ".txt"
        {
            return 1;                               //retorna verdadeiro
        }
    }
    return 0;                                       //retorna falso
} /* TestaExtensao */

void LeLabirinto(const char* entrada, Labirinto* lab)
{
    FILE *f;                    //ponteiro para o arquivo
    int i, j;

    if((f = fopen(entrada, "r")) == NULL)       //abre e testa se obteve sucesso
    {
        perror(entrada);
        exit(0);
    }
    else
    {
        fscanf(f, "%d %d %d %d %d", &lab->N, &lab->x, &lab->y, &lab->sx, &lab->sy);     //guarda dados referentes ao mapa
        lab->mapa = malloc((lab->N*lab->N)*sizeof(unsigned char));                      //aloca mapa

        for(i = 0; i < lab->N; i++)
            for(j = 0; j < lab->N; j++)
                fscanf(f, "%hhi", &lab->mapa[i*lab->N + j]); //transcreve mapa do arquivo para a matriz dentro da estrutura
    }
    fclose(f);          //fecha arquivo
} /* LeLabirinto */

int CaminhaLabirintoRecursivo(Labirinto* lab, int x, int y, int *sol)
{
    if(x == lab->sx && y == lab->sy)        //testa se encontrou a espada
    {
        sol[x*lab->N + y] = 1;
        return TRUE;                //encontrou caminho até a espada
    }
    else
    {
        if(lab->mapa[x*lab->N + y] == 0 && sol[x*lab->N + y] == 0)      //testa se há passagem e segue para vizinhos
        {
            sol[x*lab->N + y] = 1;
            if(y+1 < lab->N)
                if(CaminhaLabirintoRecursivo(lab, x, y + 1, sol) == TRUE)
                    return TRUE;
            if(x+1 < lab->N)
                if(CaminhaLabirintoRecursivo(lab, x + 1, y, sol) == TRUE)
                    return TRUE;
            if(y-1 >= 0)
                if(CaminhaLabirintoRecursivo(lab, x, y - 1, sol) == TRUE)
                    return TRUE;
            if(x-1 >= 0)
                if(CaminhaLabirintoRecursivo(lab, x - 1, y, sol) == TRUE)
                    return TRUE;
            sol[x*lab->N + y] = 0;
        }
    }
    return FALSE;       //não encontrou caminho possível no ponto
} /* CaminhaLabirintoRecursivo */

int CaminhaLabirintoIterativo(Labirinto *lab, int x, int y, int *sol)
{
    TipoPilha *pilha = malloc(sizeof(TipoPilha));       //cria pilha auxiliar
    TipoItem item;
    int boolean = TRUE, i;          //flag e contador
    FPVazia(pilha);             //inicializa pilha

    do
    {
        if(x == lab->sx && y == lab->sy)        //testa se encontrou a espada
        {
            sol[x*lab->N + y] = 1;
            Libera_Pilha(pilha);
            return TRUE;                    //encontrou caminho
        }
        else
        {
            if(lab->mapa[x*lab->N + y] == 0)        //testa se há passagem
            {
                item.x = x;
                item.y = y;
                Empilha(item,pilha);                //empilha ponto atual
                sol[x*lab->N + y] = 1;
                boolean = TRUE;                     //coloca flag como true para possível caminho e testa vizinhos

                if(y+1 < lab->N)
                    if(lab->mapa[x*lab->N + (y+1)] == 0 && sol[x*lab->N + (y+1)] == 0)
                    {
                        item.dir[0] = 1;
                        y++;
                        continue;
                    }
                item.dir[0] = 1;
                if(x+1 < lab->N)
                    if(lab->mapa[(x+1)*lab->N + y] == 0 && sol[(x+1)*lab->N + y] == 0)
                    {
                        item.dir[1] = 1;
                        x++;
                        continue;
                    }
                item.dir[1] = 1;
                if(y-1 >= 0)
                    if(lab->mapa[x*lab->N + (y-1)] == 0 && sol[x*lab->N + (y-1)] == 0)
                    {
                        item.dir[2] = 1;
                        y--;
                        continue;
                    }
                item.dir[2] = 1;
                if(x-1 >= 0)
                    if(lab->mapa[(x-1)*lab->N + y] == 0 && sol[(x-1)*lab->N + y] == 0)
                    {
                        item.dir[3] = 1;
                        x--;
                        continue;
                    }
                item.dir[3] = 1;
                boolean = FALSE;        //não "encontrou" caminho
            }
            if(boolean == FALSE)
            {
                for(i = 0; i < direcao; i++)    //testa se passou pelas 4 direções
                    if(item.dir[i] != 1)
                        continue;

                sol[x*lab->N + y] = 0;
                lab->mapa[x*lab->N + y] = 1;        //cria uma parede no local
                item = Desempilha(pilha);           //retira ponto anterior
                x = item.x;
                y = item.y;

            }
        }
    }while(!Vazia(pilha));

    Libera_Pilha(pilha);
    return FALSE;           //não encontrou caminho
} /* CaminhaLabirintoIterativo */

int EscreveArquivo(const char* saida, int n, int boolean, int *sol)
{
    FILE *f = fopen(saida, "w");        //abre arquivo
    int i, j;

    if(f == NULL)       //testa se foi aberto com sucesso
    {
        perror(saida);
        exit(0);
    }
    else
    {
        if(boolean == TRUE)         //escreve solução
        {
            for(i = 0; i < n; i++)
            {
                for(j = 0; j < n; j++)
                    fprintf(f, "%d ", sol[i*n + j]);
                fprintf(f, "\n");
            }
        }
        else fprintf(f, "0");

    }
    fclose(f);      //fecha arquivo
    return 1;       //obteve sucesso na gravação
} /* EscreveArquivoTrue */

void Libera_Memoria(Labirinto *lab, int *sol)
{
	free(lab->mapa);    //libera mapa
	free(lab);          //libera toda estrutura
	free(sol);          //libera matriz solução
}/* Libera_Memoria */

void Libera_Pilha(TipoPilha *pilha)
{
	Apontador aux;
	while(!Vazia(pilha))        //libera cada celula empilhada
	{
		aux = pilha->Topo;
		pilha->Topo = aux->Prox;
		free(aux);
    }
    free(pilha->Fundo);         //libera ponteiro do fim da pilha
    free(pilha);                //libera ponteiro da pilha
}/* Libera_Pilha */

/* PILHA */

void FPVazia(TipoPilha* Pilha)
{
    Pilha->Topo = malloc(sizeof(Celula));       //cria cédula do topo
    Pilha->Fundo = Pilha->Topo;
    Pilha->Topo->Prox= NULL;
    Pilha->Tamanho = 0;
} /* FPVazia*/

int Vazia(TipoPilha* Pilha)
{
    return(Pilha->Topo == Pilha->Fundo); //compara topo com fundo para saber se pilha está vazia
} /* Vazia */

void Empilha(TipoItem x, TipoPilha* Pilha)
{
    Apontador Aux;
    Aux= malloc(sizeof(Celula));
    Pilha->Topo->Item = x;      //coloca novo item na cédula cabeça
    Aux->Prox= Pilha->Topo;     //cria nova cédula cabeça
    Pilha->Topo = Aux;
    Pilha->Tamanho++;           //aumenta tamanho da pilha
} /* Empilha */

TipoItem Desempilha(TipoPilha* Pilha)
{
    Apontador q;
    if(Vazia(Pilha))        //antes de remover testa se a pilha está vazia
    {
        printf("Erro: pilha vazia\n"); exit(0);
    }
    q = Pilha->Topo;                //retira cédula cabeça e substitui pelo item que seria retirado
    Pilha->Topo = q->Prox;
    free(q);
    Pilha->Tamanho--;
    return Pilha->Topo->Item;       //retorna item "retirado"
} /* Desempilha */

