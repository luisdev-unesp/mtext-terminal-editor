#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mEditor.h"



int main(){
  lista *lista = malloc(sizeof(lista)); //crio um ponteiro para a lista 
  lista->head = NULL;
  lista->tail = NULL;
  lista->tamanho = 0;
  
  
  undo *undo = malloc(sizeof(undo));
  undo->tamanho = 0;
  undo->front = NULL;


  redo *redo = malloc(sizeof(redo));
  redo->tamanho = 0;
  redo->front = NULL;


  char op[20];
  int flag = 0;
  while(flag != 1){ // colocar condiçao de parada para o while
    printf("mtext> ");
    scanf(" %s",op);
        if(strcmp(op,"open") == 0){ // uso o strcmp para comparar as strings e se for 0 quer dizer que elas sao iguais
          char arquivo[256];//open <arquivo> – abre um arquivo de texto e carrega o conteúdo no editor;
          scanf(" %s",arquivo);
          open(arquivo,lista,undo);
        }else if(strcmp(op,"save") == 0){ //save <arquivo> – salva o documento atual em um arquivo texto;
            char arquivo[256];
            scanf(" %s",arquivo);
            save(arquivo,lista,undo);
          }else if(strcmp(op,"i") == 0){
              int posicao; //i <pos> <texto> – insere uma nova linha antes da posição <pos>;
              char texto[256];
              scanf(" %d",&posicao);
              scanf(" %[^\n]",texto);
              int quan = strlen(texto); // sempre coloco o \n e \0 para depois passar a função
              texto[quan] = '\n';
              texto[quan + 1] = '\0';
              i(posicao,texto,lista,undo);
            }else if(strcmp(op,"d") == 0){
                int posicao; //d <pos> – remove a linha na posição <pos>.
                scanf(" %d",&posicao);
                d(posicao,lista,undo);
              }else if(strcmp(op,"r") == 0){
                  int posicao; //r <pos> <novo texto> – substitui a linha da posição indicada;
                  char texto[256];
                  scanf(" %d",&posicao);
                  scanf(" %[^\n]",texto);
                  int quan = strlen(texto);
                  texto[quan] = '\n';
                  texto[quan + 1] = '\0';
                  r(posicao,texto,lista,undo);
                }else if(strcmp(op,"a") == 0){ //a <pos> <sufixo> – concatena um sufixo ao final da linha da posição indicada.
                    int posicao;
                    char sufixo[256];
                    scanf(" %d",&posicao);
                    scanf(" %[^\n]",sufixo);
                    int quan = strlen(sufixo);
                    sufixo[quan] = '\n';
                    sufixo[quan + 1] = '\0';
                    a(posicao,sufixo,lista,undo);
                  }else if(strcmp(op,"split") == 0){ //split <pos> <idx> – divide a linha em duas, no índice indicado;
                      int posicao;
                      int indice;
                      scanf(" %d",&posicao);
                      scanf(" %d",&indice);
                      split(posicao,indice,lista,undo);
                    }else if(strcmp(op,"join") == 0){ //join <pos> – une a linha <pos> com a seguinte.
                        int posicao;
                        scanf(" %d",&posicao);
                        join(posicao,lista,undo);
                      }else if(strcmp(op,"undo") == 0){ // undo – desfaz a última operação de edição; 
                          undo_r(undo,lista,redo);
                        }else if(strcmp(op,"redo") == 0){ // redo – refaz a última operação desfeita.
                              redo_r(undo,lista,redo);
                              
                          }else if(strcmp(op,"print") == 0){ //print – imprime todas as linhas numeradas, usando recursão.sss
                              print(lista, lista->head, 1);
                            }else if(strcmp(op,"quit") == 0){ //quit – encerra o programa, liberando toda a memória alocada dinamicamente.
                                quit(lista);
                                printf("programa encerrado\n");
                                quit_pilha(undo,redo);
                                flag = 1; // uso essa flag para literalmente encerrar o programa
                              }
  }
  free(lista);
  free(undo);
  free(redo);
  return 0;
}
