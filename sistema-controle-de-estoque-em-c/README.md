# Sistema de Controle de Estoque e Vendas

Este é um sistema simples de controle de estoque e vendas desenvolvido em **C**. Ele permite o cadastro de produtos, registro de vendas, listagem de produtos e vendas, além da exclusão de produtos do estoque.

## Funcionalidades

- **Cadastrar Produto**: Permite adicionar novos produtos ao estoque com descrição, preço e quantidade.
- **Listar Produtos**: Exibe os produtos cadastrados no sistema.
- **Registrar Venda**: Registra a venda de um produto, atualizando a quantidade em estoque.
- **Listar Vendas**: Exibe as vendas realizadas, com informações sobre o produto vendido e a data.
- **Excluir Produto**: Exclui um produto do estoque, marcando-o como inativo.

## Pré-requisitos

Para rodar este projeto em seu computador, você precisará de:

- Um compilador C (como **GCC**, **MinGW**, ou um IDE como **Code::Blocks**).
- Sistema operacional compatível: **Windows**, **Linux** ou **macOS**.


## Como Usar
Ao executar o programa, você verá um menu com as seguintes opções:

1 - Cadastrar Produto
2 - Listar Produtos
3 - Registrar Venda
4 - Listar Vendas
5 - Excluir Produto
6 - Sair

Escolha uma das opções digitando o número correspondente. O programa guiará você através do processo.

## Arquivos Gerados
O sistema irá gerar e manipular os seguintes arquivos binários:

contador_id.bin: Armazena o próximo ID disponível para produtos.
produtos.bin: Armazena os produtos cadastrados.
vendas.bin: Armazena as vendas realizadas.
Esses arquivos são utilizados para persistir os dados entre execuções do programa.


## Testes
Para testar o sistema:

Execute o programa.
Realize as operações conforme descrito no menu.
Verifique os arquivos binários (produtos.bin, vendas.bin, contador_id.bin) para garantir que os dados estão sendo gravados corretamente.

## Licença
Este projeto está licenciado sob a MIT License.