/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * 				TRABALHO PRÁTICO 1 				   *
 * 				AEDS II - TURMA TN				   *
 * INGRID ROSSELIS SANT'ANA DA CUNHA - 2016430936  *
 * * * * * * * *  * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define direcao 4       //número de direções possíveis para o caminhamento

/* ESTRUTURA DO TRABALHO */

typedef struct
{
    int N;      			// Dimensão do labirinto
    int x;      			// Coordenada x da entrada
    int y;      			// Coordenada y da entrada
    int sx;     			// Coordenada x da espada
    int sy;     			// Coordenada y da espada
    unsigned char *mapa; 	// variável para armazenar o mapa (matriz)
} Labirinto;

/* PILHA */

typedef struct
{
    int x;                  //coordenada x
    int y;                  //coordenada y
    int dir[direcao];       //vetor de direções
}TipoItem;

typedef struct Celula* Apontador;   //ponteiro específico para a estrutura Celula

typedef struct Celula               //estrutura da Celula
{
    TipoItem Item;
    Apontador Prox;
} Celula;

typedef struct                      //estrutura da pilha
{
    Apontador Topo, Fundo;
    int Tamanho;
} TipoPilha;

/* FUNÇOES */

/* -----------------------------------------------------------------------------------------*
 *  Prototipo: Testes(char const* argv[])                                         	 	        *
 *  Funcao: Faz os testes iniciais com o inteiro do algoritmo e o nome do arquivo de saída, *
 *    para que não sejam feitos na main 	                                                *                                                                    *
 *  Entrada: vetor argv com as strings passadas por parametro na chamada do programa        *
 *  Saida: Parametro para seleção do tipo de algoritmo (recursivo ou iterativo)  	        *
 * -----------------------------------------------------------------------------------------*/
int Testes(const char* argv[]);

/* -------------------------------------------------------------------------------------*
 *  Prototipo: TestaExtensao (char* entrada)                                            *
 *  Funcao: Testa se a extensão do nome de um arquivo está correta, ou seja, se é "txt" *
 *  Entrada: nome do arquivo (char*)                                                    *
 *  Saida: 0 se há erro na extensão e 1 se estiver correta                              *
 * -------------------------------------------------------------------------------------*/
int TestaExtensao(const char* entrada);

/* --------------------------------------------------------------------------------*
 *  Prototipo: LeLabirinto(const char* entrada, Labirinto *lab)           		   *
 *  Funcao: A partir de um arquivo de entrada, lê e guarda as informações contidas *
 *    na estrutura Labirinto               					    		  		   *
 *  Entrada: Nome do arquivo do labirinto e ponteiro para estrutura labirinto 	   *
 *  Saida: Sem retorno           		    	                                   *
 * --------------------------------------------------------------------------------*/
void LeLabirinto(const char* entrada, Labirinto* lab);

/* -------------------------------------------------------------------------------*
 *  Prototipo: CaminhaLabirintoRecursivo(Labirinto* lab, int x, int y, int *sol)  *
 *  Funcao: Faz o algoritmo de caminhamento na matriz mapa recursivamente         *
 *  Entrada: Estrutura Labirinto*, inteiros x e y representando um ponto e        *
 *    a matriz solução                                                            *
 *  Saida: devolve TRUE (1) se encontrou um caminho até o ponto de saída          *
 *  ou FALSE (0) se não existe caminho entre os pontos                            *
 * -------------------------------------------------------------------------------*/
int CaminhaLabirintoRecursivo(Labirinto* lab, int x, int y, int *sol);

/* -------------------------------------------------------------------------------*
 *  Prototipo: CaminhaLabirintoIterativo(Labirinto* lab, int x, int y, int *sol)  *
 *  Funcao: Faz o algoritmo de caminhamento na matriz mapa simulando a pilha do   *
 *    sistema e, consequentemente, não usando recursão                            *
 *  Entrada: Estrutura Labirinto*, inteiros x e y representando um ponto e        *
 *    a matriz solução                                                            *
 *  Saida: devolve TRUE (1) se encontrou um caminho até o ponto de saída          *
 *    ou FALSE (0) se não existe caminho entre os pontos                          *
 * -------------------------------------------------------------------------------*/
int CaminhaLabirintoIterativo(Labirinto* lab, int x, int y, int *sol);

/* -------------------------------------------------------------------------------------*
 *  Prototipo: EscreveArquivo(char* saida, int n, int boolean, int *sol)  			    *
 *  Funcao: Dado um inteiro referente à solução do caminhamento iterativo/recursivo,    *
 *    escreve a matriz solução no arquivo de saída se o inteiro for TRUE (1) ou escreve *
 *    apenas um 0 se o inteiro for FALSE (0)                                            *
 *  Entrada: Nome do arquivo de saída, número de lados da matriz solução, int "boolean" *
 *    (0 ou 1) e a matriz solução                      			  		                *
 *  Saida: Retorna 1 se o arquivo foi gravado com sucesso                  			    *
 * -------------------------------------------------------------------------------------*/
int EscreveArquivo(const char* saida, int n, int boolean, int *sol);

/* -------------------------------------------------------------------------*
 *  Prototipo: Libera_Memoria(Labirinto *lab, int *sol)                     *
 *  Funcao: Libera toda a memoria alocada para o labirinto e para a solução *
 *  Entrada: Ponteiro para labirinto e ponteiro para solução				*
 *  Saida: Sem retorno                                      			  	*
 * -------------------------------------------------------------------------*/
void Libera_Memoria(Labirinto *lab, int *sol);

/* ---------------------------------------------------------*
 *  Prototipo: Libera_Pilha(TipoPilha *pilha)               *
 *  Funcao: Libera a pilha alocada                          *
 *  Entrada: Ponteiro para pilha                            *
 *  Saida: Sem retorno                                      *
 * ---------------------------------------------------------*/
void Libera_Pilha(TipoPilha *piha);

/* FUNÇÕES DA PILHA */

/* ------------------------------------------------*
 *  Prototipo: FPVazia (TipoPilha* Pilha)          *
 *  Funcao: Inicializa e torna a pilha vazia       *
 *  Entrada: Ponteiro para pilha                   *
 *  Saida: Sem retorno                             *
 * ------------------------------------------------*/
void FPVazia(TipoPilha* Pilha);

/* ------------------------------------------------*
 *  Prototipo: Vazia (TipoPilha* Pilha)            *
 *  Funcao: Confere se a pilha está vazia          *
 *  Entrada: Ponteiro para pilha                   *
 *  Saida: 1 se a pilha estiver vazia, 0 se não    *
 * ------------------------------------------------*/
int Vazia(TipoPilha* Pilha);

/* -------------------------------------------------------*
 *  Prototipo: Empilha(TipoItem x, TipoPilha* Pilha)      *
 *  Funcao: Adiciona um novo elemento x ao topo da pilha  *
 *  Entrada: Estrutura do tipo Item, ponteiro para pilha  *
 *  Saida: Sem retorno                                    *
 * -------------------------------------------------------*/
void Empilha(TipoItem x, TipoPilha* Pilha);

/* ------------------------------------------------*
 *  Prototipo: Desempilha(TipoPIlha* Pilha)        *
 *  Funcao: Retira o elemento do topo da pilha     *
 *  Entrada: Ponteiro para pilha                   *
 *  Saida: Devolve o Item retirado                 *
 * ------------------------------------------------*/
TipoItem Desempilha(TipoPilha* Pilha);
