# ==============================================================================
# Variáveis de Configuração
# ==============================================================================

# Compilador C
CC = gcc

# Flags de Compilação:
# -Wall -Wextra: Habilita avisos importantes
# -g: Inclui informações de debug
CFLAGS = -Wall -Wextra -g -I include

# Nome do Executável Final
EXEC = mtext

# Arquivos Fonte (.c) conforme sua pasta:
SRC = src/mEditor.c src/mEditorFunc.c

# Arquivos Objeto (.o) - gerados automaticamente a partir dos arquivos fonte
OBJ = $(SRC:.c=.o)

# ==============================================================================
# Regra Principal (Default)
# ==============================================================================

# Alvo 'all' (chamado ao digitar 'make'): Cria o executável
all: $(EXEC)

# Regra para criar o executável: Depende de todos os arquivos objeto.
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)
	@echo "Executável '$(EXEC)' criado com sucesso!"

# ==============================================================================
# Regras para Arquivos Objeto
# ==============================================================================

# Regra para mEditor.o (Assumindo que este contém a função 'main')
mEditor.o: mEditor.c mEditor.h
	$(CC) $(CFLAGS) -c mEditor.c -o mEditor.o

# Regra para mEditorFun.o (Assumindo que este contém o restante das funções)
mEditorFun.o: mEditorFunc.c mEditor.h
	$(CC) $(CFLAGS) -c mEditorFunc.c -o mEditorFunc.o

# ==============================================================================
# Regra de Limpeza
# ==============================================================================

# Regra 'clean': Remove os arquivos objeto e o executável.
clean:
	@echo "Limpando arquivos objeto (*.o) e executável..."
	rm -f $(OBJ) $(EXEC)

# Sinaliza ao make que 'all' e 'clean' não são arquivos.
.PHONY: all clean

