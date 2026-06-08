# Gerenciador de Biblioteca

Um sistema de desktop para gerenciamento de acervo de biblioteca, desenvolvido em C++ com o framework Qt. A aplicação permite cadastrar livros, controlar o estoque, gerenciar uma fila de empréstimos e visualizar o histórico de livros adicionados.

---

## ✨ Funcionalidades Principais

*   **📚 Cadastro de Livros:**
    *   Adicione novos livros com Título, Autor, ISBN-13, Quantidade e Ano.
    *   **Validação de ISBN-13:** O sistema verifica se o código ISBN é matematicamente válido, fornecendo feedback visual instantâneo.
    *   **Controle de Duplicatas:** Ao adicionar um livro com um ISBN já existente, o sistema apenas incrementa a quantidade em estoque.
    *   **Feedback Visual:** Campos de formulário vazios "tremem" para alertar o usuário sobre a necessidade de preenchimento.

*   **💾 Persistência de Dados:**
    *   Todo o acervo, a fila de empréstimos e o histórico de adições são salvos em arquivos locais (`.txt`), garantindo que os dados não sejam perdidos ao fechar a aplicação.

*   **📊 Gestão de Acervo:**
    *   Visualize todos os livros em uma tabela clara e organizada.
    *   Ordene o acervo por **ano de lançamento** (crescente ou decrescente).

*   **➡️ Fila de Empréstimos (FIFO):**
    *   Gerencie os empréstimos de livros em uma estrutura de **Fila** (*First-In, First-Out*).
    *   Ao realizar um empréstimo, a quantidade do livro é automaticamente debitada do estoque.

*   **⬅️ Histórico de Adições (LIFO):**
    *   Acompanhe os últimos livros cadastrados no sistema através de uma estrutura de **Pilha** (*Last-In, First-Out*).

*   **🎨 Interface Gráfica:**
    *   Interface moderna e intuitiva construída com Qt 6.
    *   Estilização customizada através de folhas de estilo (QSS) para uma aparência única.

---

## 🛠️ Tecnologias Utilizadas

*   **Linguagem:** C++20
*   **Framework:** Qt 6 (Widgets, Core, Gui)
*   **Build System:** CMake
*   **Estruturas de Dados:** `std::vector`, `std::stack`, `std::queue`

---

## 📝 Futuras Melhorias (To-Do)

- [ ] Implementar ordenação da lista por **Título**.
- [ ] Implementar **Busca Binária** para encontrar livros por ISBN de forma mais eficiente.
- [ ] Adicionar funcionalidade para editar e remover livros.
- [ ] Criar um sistema de devolução para os livros emprestados.

---
*Desenvolvido como um projeto para demonstrar a aplicação de estruturas de dados em C++ com uma interface gráfica.*