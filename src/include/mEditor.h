#ifndef MEDITOR_H
#define MEDITOR_H

typedef struct node{// aqui temos a struct que irá guardar os nós(linhas) do arquivo
  char data[256];
  struct node* prev;
  struct node* next;
}node;
typedef struct lista{// aqui tenho a lista para guardar começo, fim e tamanho(quantidade de linhas), caso seja necessario
  int tamanho;
  node* head;
  node* tail;
}lista;


typedef struct node2{ // pilha para guardar as açoes
  char data[256];
  int posicao;
  int func;
  struct node2* next;
}node2;
typedef struct undo{
  int tamanho;
  node2* front;
}undo;


typedef struct node3{ // pilha para guardar as coisas que talvez serao desfeitas 
  char data[256];
  int posicao;
  int func;
  struct node3* next;
}node3;
typedef struct redo{
  int tamanho;
  node3* front;
}redo;

// todas as funções usadas na mEditorFunc.c
void open(char* arquivo, lista* lista, undo* pilha);
void save(char* arquivo, lista* lista, undo* pilha);
void i(int posicao, char* texto, lista* lista, undo* pilha);
void d(int posicao, lista* lista, undo* pilha);
void r(int posicao, char* texto, lista* lista, undo* pilha);
void a(int posicao, char* sufixo, lista* lista, undo* pilha);
void split(int posicao, int indice, lista* lista, undo* pilha);
void join(int posicao, lista* lista, undo* pilha);
void undo_r(undo* pilha, lista* lista, redo* pilha_2);
void redo_r(undo* pilha, lista* lista, redo* pilha_2);
void print(lista* lista, node* linha, int i);
void quit(lista* lista);
void cria_no(int posicao, char* texto, int func, undo* pilha);
void quit_pilha(undo* undo, redo* redo);

#endif
