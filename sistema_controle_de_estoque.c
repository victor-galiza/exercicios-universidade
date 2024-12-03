#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_DESCRICAO 100

// Definição das structs
struct Produto {
    int id;              // Identificador único
    char descricao[100]; // Nome ou descrição do produto
    float preco;         // Preço do produto
    int quantidade;      // Quantidade disponível no estoque
    int ativo;           // Verifica se o produto está ativo ou não. 1 para ativo e 0 para excluído
};

struct Venda {
    int idProduto;  // Referência ao ID do produto vendido
    int quantidade; // Quantidade do produto vendida
    char data[11];  // Data da venda (ex: "DD/MM/AAAA")
};

// Funções

int obterProximoId() {
    FILE *arquivo = fopen("contador_id.bin", "rb");
    int id = 1; // Começa com ID 1, caso não exista no arquivo

    if (arquivo != NULL) {
        fread(&id, sizeof(int), 1, arquivo); // Lê o último ID salvo
        fclose(arquivo);
    }

    return id;
}

void atualizarContadorId(int novoId) {
    FILE *arquivo = fopen("contador_id.bin", "wb");
    if (arquivo != NULL) {
        fwrite(&novoId, sizeof(int), 1, arquivo); // Atualiza o contador com o próximo ID
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
                printf("Descrição: %s\n", produto.descricao);
                printf("Preço: %.2f\n", produto.preco);
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
    scanf("%s", data); // No caso de não haver espaços, scanf funciona bem

    // Atualiza a quantidade do produto no estoque
    FILE *arquivo = fopen("produtos.bin", "rb+");
    if (arquivo != NULL) {
        struct Produto produto;
        while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
            if (produto.id == idProduto) {
                if (produto.quantidade >= quantidade) {
                    produto.quantidade -= quantidade; // Decrementa a quantidade do estoque
                    fseek(arquivo, -sizeof(struct Produto), SEEK_CUR); // Vai para a posição do produto
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
    novoProduto.id = obterProximoId(); // Obtém o próximo ID disponível
    printf("Digite a descrição do produto: ");
    scanf(" %[^\n]s", novoProduto.descricao); // Leitura da descrição, incluindo espaços
    printf("Digite o preço do produto: ");
    scanf("%f", &novoProduto.preco);
    printf("Digite a quantidade do produto: ");
    scanf("%d", &novoProduto.quantidade);

    salvarProduto(novoProduto); // Salva o produto no arquivo
    atualizarContadorId(novoProduto.id + 1); // Atualiza o contador de ID para o próximo valor
    printf("Produto cadastrado com sucesso!\n");
}

void excluirProduto() {
    int id;
    printf("Digite o ID do produto a ser excluído: ");
    scanf("%d", &id);

    FILE *arquivo = fopen("produtos.bin", "rb+");
    if (arquivo != NULL) {
        struct Produto produto;
        int encontrado = 0;

        while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
            if (produto.id == id && produto.ativo) {
                produto.ativo = 0; // Marca como excluído
                fseek(arquivo, -sizeof(struct Produto), SEEK_CUR);
                fwrite(&produto, sizeof(struct Produto), 1, arquivo);
                printf("Produto excluído com sucesso!\n");
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            printf("Produto com ID %d não encontrado ou já excluído.\n", id);
        }

        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo para exclusão.\n");
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    // Inicializa o contador de ID se o arquivo não existir
    if (fopen("contador_id.bin", "r") == NULL) { // Verifica se o arquivo não existe
        atualizarContadorId(1); // Começa o contador de ID com 1
    }

    // INTERFACE DO USUÁRIO
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
            printf("Opção inválida!\n");
        }
    } while (opcao != 6);

    return 0;
}
