#include "tp1.h"

int main(int argc, const char *argv[])
{
   if(argc != 4)        //testa número de parâmetros
    {
        puts("Numero incorreto de parametros!");
        exit(1);
    }
    else
    {
    	Labirinto *lab = malloc(sizeof(Labirinto));     //aloca espaço para estrutura Labirinto
    	int *sol;                               //declara matriz solução
    	int resultado;              //variável guarda resultado do caminhamento

        LeLabirinto(argv[1], lab);
        sol = calloc(lab->N*lab->N,sizeof(int));        //aloca espaço para solução

        switch(Testes(argv)) //recebe o inteiro do tipo de algoritmo e seleciona se caminhamento será recursivo ou iterativo
        {
            case 0:
                resultado = CaminhaLabirintoRecursivo(lab, lab->x, lab->y, sol);
                break;
            case 1:
                resultado = CaminhaLabirintoIterativo(lab, lab->x, lab->y, sol);
        }

        if(EscreveArquivo(argv[2], lab->N, resultado, sol) == 1)    //escreve o arquivo
            puts("Arquivo gravado com sucesso!");

        Libera_Memoria(lab, sol);       //libera memória alocada
    }
    return 0;
}
