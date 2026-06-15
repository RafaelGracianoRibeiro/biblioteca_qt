# Gerenciador de Biblioteca em C++ com Qt

Este é um projeto de um sistema de gerenciamento de biblioteca desenvolvido em C++ com o framework Qt. A aplicação permite realizar operações básicas de um acervo, como cadastrar, buscar, deletar e emprestar livros, além de demonstrar a aplicação prática de diversas estruturas de dados e algoritmos de ordenação e busca.

## ✨ Funcionalidades

*   **Cadastro de Livros**: Adicione novos livros ao acervo com título, autor, ano, quantidade e ISBN.
*   **Atualização de Estoque**: Se um livro com um ISBN já existente for adicionado, o sistema atualiza sua quantidade em estoque.
*   **Visualização do Acervo**: Todos os livros são exibidos em uma tabela clara e organizada.
*   **Ordenação Flexível**: Ordene a lista de livros por ano de lançamento (crescente e decrescente).
*   **Busca Rápida**: Encontre livros rapidamente pelo seu código ISBN utilizando um algoritmo de **Busca Binária**.
*   **Remoção de Livros**: Delete livros do acervo de forma segura.
*   **Sistema de Empréstimos**: Gerencie uma fila de empréstimos, registrando o nome do cliente e o livro emprestado, com controle de estoque.
*   **Histórico de Adições**: Uma pilha mostra os últimos livros que foram adicionados ao sistema.
*   **Persistência de Dados**: Todas as informações (livros, pilha e fila) são salvas em arquivos de texto locais, garantindo que os dados não sejam perdidos ao fechar a aplicação.
*   **Interface Moderna**: A interface foi estilizada com QSS (Qt Style Sheets) para um visual agradável e responsivo, incluindo uma área de rolagem para telas menores.
*   **Validação de ISBN**: O sistema valida o código ISBN-13 para garantir a integridade dos dados.

## 🛠️ Tecnologias e Conceitos Aplicados

Este projeto é um excelente exemplo prático da aplicação de conceitos fundamentais da ciência da computação.

*   **Linguagem**: **C++20**
*   **Framework**: **Qt 6** para a construção da interface gráfica.

### Estruturas de Dados

*   `std::vector`: Utilizado como a estrutura principal para armazenar e gerenciar a coleção de `Livro` no acervo.
*   `std::stack`: Empregado para manter um histórico dos últimos livros adicionados (LIFO - *Last-In, First-Out*). O último livro a entrar é o primeiro a ser exibido no topo da lista.
*   `std::queue`: Utilizada para gerenciar a fila de empréstimos (FIFO - *First-In, First-Out*), garantindo uma ordem justa de registro.

### Algoritmos

*   **Busca Binária**: Implementada na função `buscaBinariaPorISBN` para pesquisar livros de forma altamente eficiente. Para que funcione, a lista é previamente ordenada por ISBN.
*   **Algoritmos de Ordenação**:
    *   **Insertion Sort**: Utilizado em `ordenarLivrosPorISBN` para organizar os livros por ISBN. É eficiente para listas pequenas ou quase ordenadas.
    *   **Selection Sort**: Utilizado em `ordenarLivrosPorAnoCresc` para encontrar o livro mais antigo e movê-lo para o início da lista.
    *   **Bubble Sort**: Utilizado em `ordenarLivrosPorAnoDecresc` para ordenar os livros do mais novo para o mais antigo.

## 📂 Estrutura do Projeto

```
/
├── CMakeLists.txt             # Arquivo de build do CMake
├── main.cpp                   # Código-fonte principal com toda a lógica e UI
├── style.qss                  # Folha de estilos da aplicação
├── recursos.qrc               # Arquivo de recursos do Qt
├── readme.md                  # Este arquivo
├── arrow-down.svg             # Ícones e outros recursos visuais
└── ...
```