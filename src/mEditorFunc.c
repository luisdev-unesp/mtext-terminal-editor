#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mEditor.h"


int acao = 0; // essa vai ser a flag para garantir que quando eu for fazer undo eu não empilhe a mesma funçao novamente
int especial = 0; // essa flag é para garantir o funcionamento da redo quando "r" e "a" é chamado
char *esp = NULL; // esse será usado no mesmo caso da flag acima, porem para guardar a string que estava no nó

void open(char* arquivo, lista* lista, undo* pilha){  //open <arquivo> – abre um arquivo de texto e carrega o conteúdo no editor;
  FILE *arq;
  arq = fopen(arquivo,"r");
  if(arq == NULL){
    printf("o arquivo não foi aberto\n");
    return;
  }
  char linha[256];
  //vou "baixar" todo conteudo do arquivo, usando um while que só para quando o arq chega ao final
  while(fgets(linha,sizeof(linha),arq) != NULL){
      node* aux = malloc(sizeof(node)); //crio um novo nó(linha) e o adiciono no final da lista
      strcpy(aux->data, linha); //copio a info da variavel 
      aux->next = NULL; // aponta para null pois sera o ultimo nó
      if(lista->tamanho == 0){ // caso especial
        aux->prev = NULL; 
        lista->head = aux;
      }else{
        aux->prev = lista->tail; // atualiza o prev para o tail
        lista->tail->next = aux; // faz o tail apontar para o novo ultimo nó
      }
      lista->tail = aux; // atualiza o tail para o novo tail
      lista->tamanho++; // adiciona um ao tamanho da lista
  } 
  cria_no(0,arquivo,1,pilha); // todas essas chamadas vão ser para criar o "hisorico" na pilha undo
  fclose(arq); // fecho arquivo
}

void save(char* arquivo, lista* lista, undo* pilha){ //save <arquivo> – salva o documento atual em um arquivo texto;
  FILE *arq2; 
  arq2 = fopen(arquivo,"w");
  if(arq2 == NULL){
    printf("o arquivo não foi aberto\n");
    return;
  }
  node* aux = lista->head;
  while(aux != NULL){ // uso esse while para printar toda informação da lista em um segundo arquivo
    fprintf(arq2,"%s",aux->data);
    aux = aux->next;
  }
  fclose(arq2);
  cria_no(0,arquivo,2,pilha);
}

void i(int posicao, char* texto, lista* lista, undo* pilha){  //i <pos> <texto> – insere uma nova linha antes da posição <pos>;
  if(lista->tamanho == 0){ // sempre faço todas as verificações possíveis para não ocorrer erros
    printf("lista vazia\n");
    return;
  }
  if(posicao <= 0 || posicao > lista->tamanho){
    printf("posicao invalida\n");
    return;
  }
  if(strlen(texto) > 255){
    printf("texto muito grande\n");
    return;
  }
  node* aux = malloc(sizeof(node)); // crio um novo nó que sera colocado antes da posição
  strcpy(aux->data,texto); // já copia a informação passada
  if(posicao > 1){ // e  partir disso vou até a linha anterior para adicionar o novo nó
    node* ajuda = lista->head; // crio essa variável para chegar até o nó que vai apontar para aux
    for(int i=1;i<posicao-1;i++){
      ajuda = ajuda->next;
    } 
    aux->next = ajuda->next; // atualizo todos os nós, fazendo o ajuda apontar para o novo e tudo mais...
    aux->prev = ajuda;
    ajuda->next->prev = aux;
    ajuda->next = aux;
  }else{
    aux->prev = NULL;
    aux->next = lista->head;
    lista->head->prev = aux;
    lista->head = aux;
  }
  lista->tamanho++; // aumento o tamanho da lista pois adicionei um novo nó
  cria_no(posicao,texto,3,pilha);
}

void d(int posicao, lista* lista, undo* pilha){ //d <pos> – remove a linha na posição <pos>.
  if(lista->tamanho == 0){
    printf("lista vazia\n");
    return;
  }else if(posicao < 0 || posicao > lista->tamanho){
    printf("posiçao invalida\n");
    return;
  }else if(lista->tamanho == 1){ // aqui temos mais casos e por isso mais verificações, essa é para quando a lista tem um nó só, sendo necessário deixar o head e tail da lista nula
    node* aux = lista->head;
    cria_no(posicao,aux->data,4,pilha);
    lista->head = NULL;
    lista->tail = NULL;
    free(aux);  
  }else if(posicao == 1){ // quando a posição que será removida é a primeira, porém a lista não tem só um nó
    node* aux = lista->head;
    cria_no(posicao,aux->data,4,pilha);
    lista->head = aux->next;
    lista->head->prev = NULL;
    free(aux);  
  }else if(posicao == lista->tamanho){ // quando a posição passada é a do último nó precisando atualizar o tail 
    node* aux = lista->tail;
    cria_no(posicao,aux->data,4,pilha);
    lista->tail = aux->prev;
    lista->tail->next = NULL;
    free(aux); 
  }else{ // quando o nó não está nas pontas
    node* aux = lista->head;
    for(int i=1;i<posicao;i++){
      aux = aux->next;
    }
    cria_no(posicao,aux->data,4,pilha);
    aux->prev->next = aux->next;
    aux->next->prev = aux->prev;
    free(aux);  
  }
  lista->tamanho--; // tiro 1 do tamanho pois estou tirando um nó
}

void r(int posicao, char* texto, lista* lista, undo* pilha){ //r <pos> <novo texto> – substitui a linha da posição indicada;
  if(lista->tamanho == 0){
    printf("lista vazia\n");
    return;
  }
  if(posicao < 1 || posicao > lista->tamanho){
    printf("posição invalida\n");
    return;
  }else{ 
    node* aux = lista->head;
    if(posicao > 1){ // para não precisar entrar no loop de for, caso esteja na primeira linha (head)
      for(int i=1;i<posicao;i++){
        aux = aux->next;
      }
    }
    if(especial == 1){ // caso especial do redo, só vai entrar nesse if quando undo for chamado
      esp = malloc(sizeof(char)*256); // preciso desse caso para quando o nó de undo for para redo, modificando a informação do nó que foi para o redo, não o deixando errada
      strcpy(esp,aux->data);
    }
    cria_no(posicao,aux->data,5,pilha);
    strcpy(aux->data,texto); // copio o texto passado para o nó 
    return;
  }
}

void a(int posicao, char* sufixo, lista* lista, undo* pilha){ //a <pos> <sufixo> – concatena um sufixo ao final da linha da posição indicada.
// mesma ideia da funçao anterior porem agora eu uso um strcat, para juntar somente o sufixo e nao substituir a linha
  if(lista->tamanho == 0){
    printf("lista vazia\n");
    return;
  }
  if(posicao < 1 || posicao > lista->tamanho){
    printf("posição invalida\n");
    return;
  }else{
    node* aux = lista->head;
    if(posicao > 1){ // para não precisar entrar no loop de for, pois ja esta na primeira linha (head), caso for igual a 1
      for(int i=1;i<posicao;i++){
        aux = aux->next;
      } 
    }
    if(strlen(aux->data) + strlen(sufixo) < sizeof(aux->data)-2){ // uma coisa legal desse é que preciso verificar se o tamanho da informação que já está no nó mais o tamanho  do sufixo que foi passado é inferior a 254 (o tamanho de data menos 2) , preciso dessa garantia para colocar o \0 
      if(especial == 1){
        esp = malloc(sizeof(char)*256);
        strcpy(esp,aux->data);
      }
      cria_no(posicao,aux->data,6,pilha);
      aux->data[strlen(aux->data)-1] = '\0'; // atualizo o \0 no lugar do \n para quando concatenar nao passae para a proxima linha
      strcat(aux->data,sufixo);
      return;
    }else{
      printf("o sistema não suporta essa concatenação");
    }
  }
}

void split(int posicao, int indice, lista* lista, undo* pilha){ //split <pos> <idx> – divide a linha em duas, no índice indicado;
  if(lista->tamanho == 0){
    printf("lista vazia\n");
    return;
  }
  if(posicao < 1 || posicao > lista->tamanho){
    printf("posição invalida\n");
    return;
  }else{
    char str[3]; // faço isso para para passar o indice em forma de string e não precisar colocar outra varivel em cada nó
    sprintf(str, "%d", indice);
    cria_no(posicao,str,7,pilha);
    node* aux = lista->head;
    node* novo = malloc(sizeof(node));
    if(posicao > 1){
      for(int i=1;i<posicao;i++){
        aux = aux->next;
      }
    }
    int k = strlen(aux->data);
    if(indice < 0 || indice > k){ // verificação do indice
      printf("indice invalido\n");
      return;
    } 
    novo->prev = aux;
    novo->next = aux->next;
    aux->next = novo;
    if(novo->next != NULL){
      novo->next->prev = novo;
    }else{
      lista->tail = novo;
    }
    strcpy(novo->data,aux->data+indice); // nisso daqui eu copio somente os caracteres que estao apos o indice informado, pois o ponteiro foi deslocado "indice" posiçoes do inicio
    aux->data[indice] = '\n'; // vou na posiçao do indice e coloco o \n para quando printar não fica na mesma linha, ja q o \n da string foi para a proxima 
    aux->data[indice+1] = '\0'; // agora coloco o terminador de cadeia um espaço apos o \n
    lista->tamanho++;
    return;
  }
}

void join(int posicao, lista* lista, undo* pilha){ //join <pos> – une a linha <pos> com a seguinte.
  if(lista->tamanho == 0){
    printf("lista vazia\n");
    return;
  }
  if(posicao < 1 || posicao > lista->tamanho-1){
    printf("posição invalida\n");
    return;
  }else{
    node* aux = lista->head;
    if(posicao > 1){
      for(int i=1;i<posicao;i++){
        aux = aux->next;
      }
    }
    int num = strlen(aux->data); // tranformei o numero em string para nao ter q criar outra variavel nos nós
    char str[3];
    sprintf(str, "%d", num);
    cria_no(posicao,str,8,pilha);
    node* ajuda = aux->next;
    if(strlen(aux->data) + strlen(ajuda->data) < sizeof(aux->data)-2){
       aux->data[strlen(aux->data)-1] = '\0';
       strcat(aux->data,ajuda->data);
       aux->next = ajuda->next;
       if(ajuda->next != NULL){ 
        aux->next->prev = aux;
       }else{
        lista->tail = aux;
       }
       free(ajuda);
       lista->tamanho--;
       return;
     }else{
       printf("o sistema não suporta essa concatenação\n");
     }
  }
}


void undo_r(undo* pilha, lista* lista, redo* pilha_2){ //undo – desfaz a última operação de edição;
	if(pilha->tamanho == 0 || pilha == NULL){
		printf("nenhuma açao foi feita\n");
		return;
	}
	acao = 1; // flag para não criar os nós nas pilhas
	switch(pilha->front->func){ // funções inversas das ações feitas
		case 1: // inversa da open
		quit(lista);
		break;
		case 2: // inversa da save
		remove(pilha->front->data); // estou passando o nome do programa que eu tinha dado save isso vai estar em data, aqui temos uma função da stdlib para dar remove
		break;
		case 3: // inversa da insere
		d(pilha->front->posicao,lista,pilha);
		break;
		case 4: // inversa da remove
		i(pilha->front->posicao,pilha->front->data,lista,pilha);
		break;
		case 5: // inversa da substitui
		especial = 1;
		r(pilha->front->posicao,pilha->front->data,lista,pilha);
		strcpy(pilha->front->data,esp);
		especial = 0;
		break;
		case 6: // inversa da concatena
		especial = 1;
		r(pilha->front->posicao,pilha->front->data,lista,pilha);
		strcpy(pilha->front->data,esp);
		especial = 0;
		break;
		case 7: // inversa da divide
		join(pilha->front->posicao,lista,pilha);
		break;
		case 8: // inversa da junta
		int k = atoi(pilha->front->data); // transformo oq estava em string para numero e passo para funçao
		split(pilha->front->posicao,k-1,lista,pilha);
		break;
		default: 
		printf("algo deu errado");
		break;
	}
	printf("ação disfeita\n");
	if(pilha->front == NULL){
	  return;
	}

	node2* aux = pilha->front; // atualizo o topo da pilha undo
	pilha->front = aux->next;
  	aux->next = pilha_2->front; // e passo o nó que estava no antigo topo, para o topo da pilha redo, fiz isso para nao ter q criar outro nó
  	pilha_2->front = aux;
	pilha_2->tamanho++;
	pilha->tamanho--;
}


void redo_r(undo* pilha, lista* lista, redo* pilha_2){ //redo – refaz a última operação desfeita.
  // fazer a original, e depois passar o nó daqui para a undo de novo
  if(pilha_2->tamanho == 0 || pilha_2 == NULL){
		printf("nenhuma açao para ser feita\n");
		return;
	}
  acao = 1; // flag para não criar os nós nas pilhas
	switch(pilha_2->front->func){ // aqui temos todas as ações com suas funções originais
		case 1: // original open
		open(pilha_2->front->data,lista,pilha);
		break;
		case 2: // original save
		save(pilha_2->front->data,lista,pilha);  
		break;
		case 3: // original insere
		i(pilha_2->front->posicao,pilha_2->front->data,lista,pilha); 
		break;
		case 4: // original remove
		d(pilha_2->front->posicao,lista,pilha); 
		break;
		case 5: // original substitui
		r(pilha_2->front->posicao,pilha_2->front->data,lista,pilha); 
		break;
		case 6: // original concatena
		r(pilha_2->front->posicao,pilha_2->front->data,lista,pilha); 
		break;
		case 7: // original divide
    		int k = atoi(pilha_2->front->data);
		split(pilha_2->front->posicao,k,lista,pilha);
		break;
		case 8: // original junta
		join(pilha_2->front->posicao,lista,pilha);
		break;
		default: 
		printf("algo deu errado");
		break;
	}
	printf("ação refeita\n");
	if(pilha_2->front == NULL){
	  return;
	}
  	node3* aux = pilha_2->front; // atualizo o topo da pilha redo
	pilha_2->front = aux->next;
  	aux->next = pilha->front; // e passo o nó que estava no antigo topo, para o topo da pilha undo, fiz isso para nao ter q criar outro nó
  	pilha->front = aux;
	pilha_2->tamanho--;
	pilha->tamanho++;
}


void print(lista* lista, node* linha, int i){ //print – imprime todas as linhas numeradas, usando recursão.
  if(lista->tamanho == 0 || lista == NULL){
    printf("lista vazia\n");
    return;
  }
  if(linha == lista->tail){
    printf("%d | %s", i, linha->data);
    return;
  }
  printf("%d | %s", i, linha->data);
  print(lista,linha->next, i+1); // uso a recursão para ir do primeiro nó ate o ultimo, e para não guardar nenhuma informação nos nós eu uso uma variável para informar a linha
}

void quit(lista* lista){ //quit – encerra o programa, liberando toda a memória alocada dinamicamente.
  if(lista == NULL){ // quando a lista não existe
    return;
  }
  while(lista->head != NULL){ // só vou dando free nos nós e depois dou NULL no tail, head
    node* aux = lista->head;
    lista->head = aux->next;
    free(aux);
  }
  lista->head = NULL;
  lista->tail = NULL;
  lista->tamanho = 0;
}



void cria_no(int posicao, char* texto, int func, undo* pilha){ // usado para criar nó da undo
    if(acao == 0){ 
      node2* aux = malloc(sizeof(node2));
      strcpy(aux->data,texto);
      aux->posicao = posicao;
      aux->func = func;
	  if(pilha->tamanho == 0){
		  aux->next = NULL;
		  pilha->front = aux;
		  pilha->tamanho++;
	  }else{
		  aux->next = pilha->front;
		  pilha->front = aux;
		  pilha->tamanho++;
	  }
	}else{ // esse else é usado para quando a undo é chamada garantir que não vai criar nenhum nó errado na pilha 
	  acao = 0;
	  return;
	}
}

void quit_pilha(undo* undo, redo* redo){ // só uso quando o programa vai ser literalmente encerrado, para liberar memória das pilhas undo e redo
  if(undo == NULL && redo == NULL){
    return;
  }
  while(undo->front != NULL){
    node2* aux = undo->front;
    undo->front = aux->next;
    free(aux);
  }
  while(redo->front != NULL){
    node3* aux = redo->front;
    redo->front = aux->next;
    free(aux);
  }
  undo->front = NULL;
  undo->tamanho = 0;
  redo->front = NULL;
  redo->tamanho = 0;
}
