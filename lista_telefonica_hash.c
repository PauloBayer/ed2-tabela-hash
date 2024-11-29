// Estrutura programa Tabela Hash - Professor Lucas.
// Aluno: Paulo Eduardo Bayer Kresko GRR20220863.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO_TABELA 100

typedef struct Contato {
    char nome[50];
    char telefone[20];
    struct Contato* proximo;
} Contato;

Contato* tabelaHash[TAMANHO_TABELA];

unsigned int funcaoHash(char *nome) {
    unsigned int valorHash = 0;
    for (; *nome != '\0'; nome++)
        valorHash += *nome;
    return valorHash % TAMANHO_TABELA;
}

double tempoTotalInsercao = 0;
int contagemInsercao = 0;

double tempoTotalBusca = 0;
int contagemBusca = 0;

void adicionarContato() {
    char nome[50];
    char telefone[20];
    printf("Nome: ");
    scanf("%49s", nome);
    printf("Telefone: ");
    scanf("%19s", telefone);

    clock_t inicio, fim;
    double tempo_insercao;

    inicio = clock();

    unsigned int indice = funcaoHash(nome);

    // Criando um novo contato
    Contato* novoContato = (Contato*) malloc(sizeof(Contato));
    if (novoContato == NULL) {
        printf("Erro de alocacao de memoria!\n");
        return;
    }
    strcpy(novoContato->nome, nome);
    strcpy(novoContato->telefone, telefone);
    novoContato->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoContato;

    fim = clock();
    tempo_insercao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;  // em milissegundos

    tempoTotalInsercao += tempo_insercao;
    contagemInsercao++;

    printf("Contato adicionado com sucesso. (Tempo de insercao: %.2f ms)\n", tempo_insercao);
}

void buscarContato() {
    char nome[50];
    printf("Nome: ");
    scanf("%49s", nome);

    clock_t inicio, fim;
    double tempo_busca;

    inicio = clock();

    unsigned int indice = funcaoHash(nome);
    Contato* atual = tabelaHash[indice];
    int encontrado = 0;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            if (!encontrado) {
                fim = clock();
                tempo_busca = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
                printf("Contatos encontrados para %s (tempo de busca: %.2f ms):\n", nome, tempo_busca);
                tempoTotalBusca += tempo_busca;
                contagemBusca++;
                encontrado = 1;
            }
            printf("Telefone: %s\n", atual->telefone);
        }
        atual = atual->proximo;
    }
    if (!encontrado) {
        fim = clock();
        tempo_busca = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
        printf("Contato nao encontrado. (tempo de busca: %.2f ms)\n", tempo_busca);
        tempoTotalBusca += tempo_busca;
        contagemBusca++;
    }
}

void removerContato() {
    char nome[50];
    printf("Nome: ");
    scanf("%49s", nome);

    unsigned int indice = funcaoHash(nome);
    Contato* atual = tabelaHash[indice];
    Contato* anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            if (anterior == NULL) {
                tabelaHash[indice] = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Contato removido com sucesso.\n");
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Contato nao encontrado.\n");
}

void exibirContatos() {
    int vazio = 1;
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        Contato* atual = tabelaHash[i];
        while (atual != NULL) {
            printf("Nome: %s, Telefone: %s\n", atual->nome, atual->telefone);
            atual = atual->proximo;
            vazio = 0;
        }
    }
    if (vazio) {
        printf("A lista de contatos esta vazia.\n");
    }
}

int main() {
    int opcao;

    // Inicializa a tabela hash com NULL
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabelaHash[i] = NULL;
    }

    do {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");

        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarContato();
                break;
            case 2:
                buscarContato();
                break;
            case 3:
                removerContato();
                break;
            case 4:
                exibirContatos();
                break;
            case 0:
                printf("Saindo...\n");
                if (contagemInsercao > 0)
                    printf("Tempo medio de insercao: %.2f ms\n", tempoTotalInsercao / contagemInsercao);
                if (contagemBusca > 0)
                    printf("Tempo medio de busca: %.2f ms\n", tempoTotalBusca / contagemBusca);
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    // Libera a memória alocada
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        Contato* atual = tabelaHash[i];
        while (atual != NULL) {
            Contato* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }

    return 0;
}
