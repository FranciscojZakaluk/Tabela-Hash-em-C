#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Livro {
    char titulo[100];
    char autor[100];
    char isbn[20];
    struct Livro* prox; // Para lista de colisão na tabela hash
} Livro;

typedef struct Autor {
    char nome[100];
    struct Autor* prox;
} Autor;

Livro* tabelaISBN[TABLE_SIZE];
Livro* tabelaTitulo[TABLE_SIZE];
Autor* listaAutores = NULL;

// Função hash para ISBN (números)
int hashISBN(char* isbn) {
    int soma = 0;
    for (int i = 0; isbn[i] != '\0'; i++) {
        soma += isbn[i];
    }
    return soma % TABLE_SIZE;
}

// Função hash para título (strings)
int hashTitulo(char* titulo) {
    int hash = 0;
    for (int i = 0; titulo[i] != '\0'; i++) {
        hash = (hash * 31 + titulo[i]) % TABLE_SIZE;
    }
    return hash;
}

// Função para cadastrar autor na lista encadeada
void cadastrarAutor(char* nome) {
    Autor* atual = listaAutores;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) return; // Já existe
        atual = atual->prox;
    }
    Autor* novo = (Autor*)malloc(sizeof(Autor));
    strcpy(novo->nome, nome);
    novo->prox = listaAutores;
    listaAutores = novo;
}

// Função para cadastrar livro
void cadastrarLivro() {
    Livro* novo = (Livro*)malloc(sizeof(Livro));
    printf("Digite o titulo do livro: ");
    fgets(novo->titulo, 100, stdin);
    novo->titulo[strcspn(novo->titulo, "\n")] = '\0'; // Remove newline

    printf("Digite o autor do livro: ");
    fgets(novo->autor, 100, stdin);
    novo->autor[strcspn(novo->autor, "\n")] = '\0';

    printf("Digite o ISBN do livro: ");
    fgets(novo->isbn, 20, stdin);
    novo->isbn[strcspn(novo->isbn, "\n")] = '\0';

    novo->prox = NULL;

    // Inserir na tabela hash de ISBN
    int idxISBN = hashISBN(novo->isbn);
    novo->prox = tabelaISBN[idxISBN];
    tabelaISBN[idxISBN] = novo;

    // Inserir na tabela hash de Título
    int idxTitulo = hashTitulo(novo->titulo);
    Livro* novoTitulo = (Livro*)malloc(sizeof(Livro));
    *novoTitulo = *novo; // Cópia dos dados
    novoTitulo->prox = tabelaTitulo[idxTitulo];
    tabelaTitulo[idxTitulo] = novoTitulo;

    // Cadastrar autor
    cadastrarAutor(novo->autor);

    printf("Livro cadastrado com sucesso!\n");
}

// Buscar livro por ISBN
void buscarPorISBN() {
    char isbn[20];
    printf("Digite o ISBN para buscar: ");
    fgets(isbn, 20, stdin);
    isbn[strcspn(isbn, "\n")] = '\0';

    int idx = hashISBN(isbn);
    Livro* atual = tabelaISBN[idx];

    while (atual != NULL) {
        if (strcmp(atual->isbn, isbn) == 0) {
            printf("Livro encontrado: %s - Autor: %s\n", atual->titulo, atual->autor);
            return;
        }
        atual = atual->prox;
    }
    printf("Livro nao encontrado.\n");
}

// Buscar livro por Título
void buscarPorTitulo() {
    char titulo[100];
    printf("Digite o titulo para buscar: ");
    fgets(titulo, 100, stdin);
    titulo[strcspn(titulo, "\n")] = '\0';

    int idx = hashTitulo(titulo);
    Livro* atual = tabelaTitulo[idx];

    while (atual != NULL) {
        if (strcmp(atual->titulo, titulo) == 0) {
            printf("Livro encontrado: ISBN: %s - Autor: %s\n", atual->isbn, atual->autor);
            return;
        }
        atual = atual->prox;
    }
    printf("Livro nao encontrado.\n");
}

// Listar todos os autores
void listarAutores() {
    Autor* atual = listaAutores;
    printf("Lista de autores:\n");
    while (atual != NULL) {
        printf("- %s\n", atual->nome);
        atual = atual->prox;
    }
}

void menu() {
    int opcao;
    do {
        printf("\n==== MENU ====\n");
        printf("1. Cadastrar Livro\n");
        printf("2. Buscar Livro por ISBN\n");
        printf("3. Buscar Livro por Titulo\n");
        printf("4. Listar Autores\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // Consumir o \n que sobra

        switch (opcao) {
            case 1: cadastrarLivro(); break;
            case 2: buscarPorISBN(); break;
            case 3: buscarPorTitulo(); break;
            case 4: listarAutores(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção invalida.\n");
        }
    } while (opcao != 0);
}

int main() {
    menu();
    return 0;
}
