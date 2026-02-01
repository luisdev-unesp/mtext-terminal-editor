mEditor - Editor de Texto em C

Este projeto consiste em um editor de texto via terminal desenvolvido como parte da disciplina de Algoritmos e Técnicas de Programação (ATP) na UNESP (IBILCE). O foco principal é a aplicação prática de estruturas de dados dinâmicas para manipulação eficiente de texto e gerenciamento de estado.
🛠️ Arquitetura do Projeto

O sistema foi modularizado para garantir organização e facilidade de manutenção:

    mEditor.h: Contém as definições das estruturas de dados (node, lista, undo, redo) e os protótipos de todas as funções.

    mEditor.c: Arquivo principal que gerencia o fluxo de execução, alocação de memória inicial e o laço da interface com o usuário.

    mEditorFunc.c: Implementação de todas as funções que tornam o programa funcional, incluindo a lógica das pilhas.

    Makefile: Automatização do processo de compilação do executável mtext.

🚀 Funcionalidades Principais
Gerenciamento de Arquivos e Texto

    Abertura e Salvamento: As funções open e save permitem carregar e persistir dados em arquivos externos.

    Edição Dinâmica: Suporte para inserção (i), remoção (d), substituição (r) e concatenação de sufixos (a).

    Manipulação Avançada: Divisão de linhas (split) e junção de linhas adjacentes (join) com tratamento rigoroso de índices e terminadores de string (\n, \0).

Sistema de Histórico (Undo/Redo)

O editor implementa um sistema de histórico robusto utilizando duas pilhas dinâmicas:

    undo: Armazena as operações realizadas para permitir a reversão de estados.

    redo: Permite refazer ações que foram previamente desfeitas.

    Controle de Fluxo: Utiliza flags técnicas (como as variáveis acao e especial) para garantir que as operações de reversão não gerem nós de histórico duplicados ou errados.

Recursos Adicionais

    Impressão Recursiva: A função print percorre a lista duplamente encadeada utilizando recursão para exibir o conteúdo linha por linha.

    Gerenciamento de Memória: Funções dedicadas (quit e quit_pilha) garantem que toda a memória alocada dinamicamente para listas e pilhas seja liberada ao encerrar o programa.

💻 Especificações Técnicas

    Linguagem: C.

    Estruturas: Lista Duplamente Encadeada e Pilhas Dinâmicas.

    Limite por Linha: 255 caracteres.

    Interface: CLI (Command Line Interface).
