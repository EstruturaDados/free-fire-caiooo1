#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// --- CONSTANTES E DEFINIÇÕES ---
#define MAX_ITENS 10
#define MAX_NOME 30

// Enumeração para os critérios de ordenação
typedef enum {
    POR_NOME = 1,
    POR_TIPO,
    POR_PRIORIDADE
} CriterioOrdenacao;

// Estrutura do Item
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_NOME]; // Ex: Cura, Municao, Arma
    int quantidade;
    int prioridade;      // 1 (Baixa) a 5 (Alta)
} Item;

// Variáveis Globais (Simulando o estado da mochila)
Item mochila[MAX_ITENS];
int numItens = 0;
bool ordenadaPorNome = false; // Flag para controlar a busca binária

// --- PROTÓTIPOS DAS FUNÇÕES ---
void limparTela();
void removerQuebraLinha(char *str);
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio, int *totalComparacoes);
void buscaBinariaPorNome();

// --- FUNÇÃO PRINCIPAL ---
int main() {
    int opcao;

    do {
        limparTela();
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer do teclado

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("\nSaindo da ilha... Boa sorte!\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                getchar();
        }
    } while (opcao != 0);

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

void limparTela() {
    // Simula limpeza de tela com quebras de linha
    for (int i = 0; i < 3; i++) printf("\n");
}

void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void exibirMenu() {
    printf("=========================================\n");
    printf("      MOCHILA FREE FIRE - NIVEL MESTRE   \n");
    printf("=========================================\n");
    printf("Itens na mochila: %d/%d\n", numItens, MAX_ITENS);
    printf("Status Ordenacao (Nome): %s\n", ordenadaPorNome ? "[OK]" : "[NAO]");
    printf("-----------------------------------------\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens\n");
    printf("4. Ordenar Mochila (Nome/Tipo/Prioridade)\n");
    printf("5. Busca Binaria (Requer ordenacao por Nome)\n");
    printf("0. Sair\n");
    printf("=========================================\n");
}

void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("\n[ERRO] Mochila cheia! Descarte algo primeiro.\n");
        getchar();
        return;
    }

    Item novo;
    
    printf("\n--- ADICIONAR NOVO ITEM ---\n");
    printf("Nome: ");
    fgets(novo.nome, MAX_NOME, stdin);
    removerQuebraLinha(novo.nome);

    printf("Tipo (ex: Municao, Cura): ");
    fgets(novo.tipo, MAX_NOME, stdin);
    removerQuebraLinha(novo.tipo);

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    do {
        printf("Prioridade (1-Baixa a 5-Alta): ");
        scanf("%d", &novo.prioridade);
    } while (novo.prioridade < 1 || novo.prioridade > 5);

    mochila[numItens] = novo;
    numItens++;
    
    // Ao inserir um novo item, a lista deixa de estar necessariamente ordenada
    ordenadaPorNome = false; 

    printf("\n[SUCESSO] Item '%s' adicionado!\n", novo.nome);
    getchar(); getchar(); // Pausa
}

void removerItem() {
    char nomeBusca[MAX_NOME];
    int encontrou = -1;

    printf("\n--- REMOVER ITEM ---\n");
    printf("Nome do item para remover: ");
    fgets(nomeBusca, MAX_NOME, stdin);
    removerQuebraLinha(nomeBusca);

    // Busca linear simples para remoção
    for (int i = 0; i < numItens; i++) {
        if (strcasecmp(mochila[i].nome, nomeBusca) == 0) { // strcasecmp ignora maiúsculas/minúsculas
            encontrou = i;
            break;
        }
    }

    if (encontrou != -1) {
        // Shift (deslocamento) para preencher o buraco
        for (int i = encontrou; i < numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        numItens--;
        printf("\n[SUCESSO] Item removido.\n");
    } else {
        printf("\n[ERRO] Item nao encontrado.\n");
    }
    getchar();
}

void listarItens() {
    printf("\n--- CONTEUDO DA MOCHILA ---\n");
    if (numItens == 0) {
        printf("Mochila vazia.\n");
    } else {
        printf("%-15s | %-10s | %-5s | %s\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
        printf("---------------------------------------------------\n");
        for (int i = 0; i < numItens; i++) {
            printf("%-15s | %-10s | %-5d | %d\n", 
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
        }
    }
    printf("\nPressione ENTER para voltar...");
    getchar();
}

// Algoritmo Insertion Sort modificado para aceitar critérios
void insertionSort(CriterioOrdenacao criterio, int *totalComparacoes) {
    int i, j;
    Item key;
    *totalComparacoes = 0;

    for (i = 1; i < numItens; i++) {
        key = mochila[i];
        j = i - 1;
        
        bool deveMover = false;

        // O loop verifica a condição de troca baseada no critério
        while (j >= 0) {
            (*totalComparacoes)++; // Contabiliza a comparação feita no if abaixo

            if (criterio == POR_NOME) {
                // Ordem alfabética (A-Z)
                if (strcasecmp(mochila[j].nome, key.nome) > 0) deveMover = true;
                else deveMover = false;
            } 
            else if (criterio == POR_TIPO) {
                // Ordem alfabética de tipo (A-Z)
                if (strcasecmp(mochila[j].tipo, key.tipo) > 0) deveMover = true;
                else deveMover = false;
            } 
            else if (criterio == POR_PRIORIDADE) {
                // Ordem decrescente (5 -> 1)
                // Se o item anterior tem prioridade MENOR que a chave, movemos ele para trás
                if (mochila[j].prioridade < key.prioridade) deveMover = true;
                else deveMover = false;
            }

            if (deveMover) {
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break; // Se não precisa mover, sai do while
            }
        }
        mochila[j + 1] = key;
    }
}

void menuDeOrdenacao() {
    if (numItens < 2) {
        printf("\nNao ha itens suficientes para ordenar.\n");
        getchar(); return;
    }

    int op;
    int comparacoes = 0;

    printf("\n--- CRITERIO DE ORDENACAO ---\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Tipo (A-Z)\n");
    printf("3. Por Prioridade (Maior para Menor)\n");
    printf("Escolha: ");
    scanf("%d", &op);

    // Atualiza a flag global baseada na escolha
    if (op == 1) {
        insertionSort(POR_NOME, &comparacoes);
        ordenadaPorNome = true;
    } else if (op == 2) {
        insertionSort(POR_TIPO, &comparacoes);
        ordenadaPorNome = false;
    } else if (op == 3) {
        insertionSort(POR_PRIORIDADE, &comparacoes);
        ordenadaPorNome = false;
    } else {
        printf("Opcao invalida.\n");
        return;
    }

    printf("\n[SUCESSO] Mochila reordenada!\n");
    printf("Performance: %d comparacoes realizadas.\n", comparacoes);
    getchar(); getchar();
}

void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        printf("\n[ALERTA] A mochila precisa estar ordenada por NOME antes de usar a Busca Binaria.\n");
        printf("Va ao menu de ordenacao e selecione a opcao 1.\n");
        getchar();
        return;
    }
    
    

    char alvo[MAX_NOME];
    printf("\n--- BUSCA BINARIA ---\n");
    printf("Digite o nome do item: ");
    fgets(alvo, MAX_NOME, stdin);
    removerQuebraLinha(alvo);

    int inicio = 0;
    int fim = numItens - 1;
    int meio;
    int encontrado = -1;
    int iteracoes = 0;

    while (inicio <= fim) {
        iteracoes++;
        meio = (inicio + fim) / 2;
        
        int comparacao = strcasecmp(mochila[meio].nome, alvo);

        if (comparacao == 0) {
            encontrado = meio;
            break;
        } else if (comparacao < 0) {
            inicio = meio + 1; // Está na metade direita
        } else {
            fim = meio - 1;    // Está na metade esquerda
        }
    }

    if (encontrado != -1) {
        printf("\n[ENCONTRADO] Item localizado na posicao %d (apos %d iteracoes):\n", encontrado + 1, iteracoes);
        printf("Nome: %s | Tipo: %s | Qtd: %d | Prioridade: %d\n", 
               mochila[encontrado].nome, mochila[encontrado].tipo, 
               mochila[encontrado].quantidade, mochila[encontrado].prioridade);
    } else {
        printf("\n[404] Item '%s' nao encontrado na mochila.\n", alvo);
    }
    getchar();
}