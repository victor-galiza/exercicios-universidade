#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_DESCRICAO 100

// Defini��o das structs
struct Produto {
    int id;              // Identificador �nico
    char descricao[100]; // Nome ou descri��o do produto
    float preco;         // Pre�o do produto
    int quantidade;      // Quantidade dispon�vel no estoque
    int ativo;           // Verifica se o produto est� ativo ou n�o. 1 para ativo e 0 para exclu�do
};

struct Venda {
    int idProduto;  // Refer�ncia ao ID do produto vendido
    int quantidade; // Quantidade do produto vendida
    char data[11];  // Data da venda (ex: "DD/MM/AAAA")
};

// Fun��es

int obterProximoId() {
    FILE *arquivo = fopen("contador_id.bin", "rb");
    int id = 1; // Come�a com ID 1, caso n�o exista no arquivo

    if (arquivo != NULL) {
        fread(&id, sizeof(int), 1, arquivo); // L� o �ltimo ID salvo
        fclose(arquivo);
    }

    return id;
}

void atualizarContadorId(int novoId) {
    FILE *arquivo = fopen("contador_id.bin", "wb");
    if (arquivo != NULL) {
        fwrite(&novoId, sizeof(int), 1, arquivo); // Atualiza o contador com o pr�ximo ID
        fclose(arquivo);
    } else {
        printf("Erro ao atualizar o contador de IDs.\n");
    }
}

void salvarProduto(struct Produto produto) {
    FILE *arquivo = fopen("produtos.bin", "ab");
    if (arquivo != NULL) {
        fwrite(&produto, sizeof(struct Produto), 1, arquivo); // Escreve o produto no arquivo
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para salvar o produto.\n");
    }
}

void listarProdutos() {
    FILE *arquivo = fopen("produtos.bin", "rb");
    if (arquivo != NULL) {
        struct Produto produto;
        int encontrou = 0;

        while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
            if (produto.ativo) {
                printf("ID: %d\n", produto.id);
                printf("Descri��o: %s\n", produto.descricao);
                printf("Pre�o: %.2f\n", produto.preco);
                printf("Quantidade em estoque: %d\n", produto.quantidade);
                printf("----------------------------\n");
                encontrou = 1;
            }
        }
        if (!encontrou) {
            printf("Nenhum produto cadastrado ou ativo no momento.\n");
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para leitura.\n");
    }
}

void registrarVenda() {
    int idProduto, quantidade;
    char data[11];

    printf("Digite o ID do produto: ");
    scanf("%d", &idProduto);
    printf("Digite a quantidade vendida: ");
    scanf("%d", &quantidade);
    printf("Digite a data da venda (DD/MM/AAAA): ");
    scanf("%s", data); // No caso de n�o haver espa�os, scanf funciona bem

    // Atualiza a quantidade do produto no estoque
    FILE *arquivo = fopen("produtos.bin", "rb+");
    if (arquivo != NULL) {
        struct Produto produto;
        while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
            if (produto.id == idProduto) {
                if (produto.quantidade >= quantidade) {
                    produto.quantidade -= quantidade; // Decrementa a quantidade do estoque
                    fseek(arquivo, -sizeof(struct Produto), SEEK_CUR); // Vai para a posi��o do produto
                    fwrite(&produto, sizeof(struct Produto), 1, arquivo); // Atualiza o produto no arquivo
                    printf("Venda registrada com sucesso!\n");
                    break;
                } else {
                    printf("Estoque insuficiente!\n");
                    break;
                }
            }
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para registrar a venda.\n");
    }

    // Registra a venda no arquivo de vendas
    FILE *vendaArquivo = fopen("vendas.bin", "ab");
    if (vendaArquivo != NULL) {
        struct Venda venda = {idProduto, quantidade};
        strcpy(venda.data, data); // Copia a data para a venda
        fwrite(&venda, sizeof(struct Venda), 1, vendaArquivo); // Salva a venda
        fclose(vendaArquivo);
    } else {
        printf("Erro ao registrar a venda.\n");
    }
}

void listarVendas() {
    FILE *arquivo = fopen("vendas.bin", "rb");
    if (arquivo != NULL) {
        struct Venda venda;
        while (fread(&venda, sizeof(struct Venda), 1, arquivo)) {
            printf("ID Produto: %d\n", venda.idProduto);
            printf("Quantidade Vendida: %d\n", venda.quantidade);
            printf("Data da Venda: %s\n", venda.data);
            printf("----------------------------\n");
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para listar as vendas.\n");
    }
}

void cadastrarProduto() {
    struct Produto novoProduto;
    novoProduto.ativo = 1;
    novoProduto.id = obterProximoId(); // Obt�m o pr�ximo ID dispon�vel
    printf("Digite a descri��o do produto: ");
    scanf(" %[^\n]s", novoProduto.descricao); // Leitura da descri��o, incluindo espa�os
    printf("Digite o pre�o do produto: ");
    scanf("%f", &novoProduto.preco);
    printf("Digite a quantidade do produto: ");
    scanf("%d", &novoProduto.quantidade);

    salvarProduto(novoProduto); // Salva o produto no arquivo
    atualizarContadorId(novoProduto.id + 1); // Atualiza o contador de ID para o pr�ximo valor
    printf("Produto cadastrado com sucesso!\n");
}

void excluirProduto() {
    int id;
    printf("Digite o ID do produto a ser exclu�do: ");
    scanf("%d", &id);

    FILE *arquivo = fopen("produtos.bin", "rb+");
    if (arquivo != NULL) {
        struct Produto produto;
        int encontrado = 0;

        while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
            if (produto.id == id && produto.ativo) {
                produto.ativo = 0; // Marca como exclu�do
                fseek(arquivo, -sizeof(struct Produto), SEEK_CUR);
                fwrite(&produto, sizeof(struct Produto), 1, arquivo);
                printf("Produto exclu�do com sucesso!\n");
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            printf("Produto com ID %d n�o encontrado ou j� exclu�do.\n", id);
        }

        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para exclus�o.\n");
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    // Inicializa o contador de ID se o arquivo n�o existir
    if (fopen("contador_id.bin", "r") == NULL) { // Verifica se o arquivo n�o existe
        atualizarContadorId(1); // Come�a o contador de ID com 1
    }

    // INTERFACE DO USU�RIO
    do {
        printf("\n========================BEM VINDO=========================\n");
        printf("\n\tMenu:\n\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Registrar Venda\n");
        printf("4 - Listar Vendas\n");
        printf("5 - Excluir Produto\n");
        printf("6 - Sair\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            cadastrarProduto();
            break;
        case 2:
            listarProdutos();
            break;
        case 3:
            registrarVenda();
            break;
        case 4:
            listarVendas();
            break;
        case 5:
            excluirProduto();
            break;
        case 6:
            printf("Saindo...\n");
            break;
        default:
            printf("Op��o inv�lida!\n");
        }
    } while (opcao != 6);

    return 0;
}
