#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <QListWidget> // Incluído para a lista visual
#include <QRegularExpressionValidator>
#include <vector>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <iostream>
#include <algorithm> // Incluído para std::swap

using namespace std;

class Livro {
public:
    string titulo;
    string autor;
    int ano;
    int qtd;
    string isbn; // Mudamos para string porque ISBNs podem ter até 13 dígitos e começar com zero

    Livro(string t, string a, int y, int q, string isbn) : titulo(t), autor(a), ano(y), qtd(q), isbn(isbn) {}
};

void salvarLivrosNoArquivo(const vector<Livro>& livros) {
    // Define o nome/caminho do arquivo
    QFile arquivo("meus_livros.txt");

    // Tenta abrir o arquivo em modo de escrita (WriteOnly) e texto (Text)
    if (arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream saida(&arquivo);

        // Percorre o vetor e escreve cada livro no arquivo
        for (const Livro& l : livros) {
            // Escreve no formato: Titulo;Autor;Ano
            saida << QString::fromStdString(l.titulo) << ";"
                  << QString::fromStdString(l.autor) << ";"
                  << l.ano << ";"
                  << l.qtd << ";"
                  << QString::fromStdString(l.isbn) << "\n";
        }

        // Fecha o arquivo após terminar
        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo para salvar!" << endl;
    }
}
void carregarLivrosDoArquivo(vector<Livro>& livros) {
    // Define o mesmo nome de arquivo usado para salvar
    QFile arquivo("meus_livros.txt");

    // Tenta abrir o arquivo em modo de leitura (ReadOnly) e texto (Text)
    if (arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream entrada(&arquivo);

        // Lê o arquivo até o final
        while (!entrada.atEnd()) {
            // Pega uma linha inteira do arquivo (Ex: "O Hobbit;J.R.R. Tolkien;1937")
            QString linha = entrada.readLine();

            // Pula linhas vazias (caso o arquivo tenha espaços em branco no final)
            if (linha.trimmed().isEmpty()) {
                continue;
            }

            // Divide a linha usando o ponto e vírgula como separador
            // O resultado é uma lista de pedaços: [0]="O Hobbit", [1]="J.R.R. Tolkien", [2]="1937"
            QStringList partes = linha.split(";");

            // Verifica se a linha realmente tinha 3 partes antes de tentar ler
            if (partes.size() == 5) {
                QString tituloStr = partes[0];
                QString autorStr = partes[1];
                int anoVal = partes[2].toInt(); // Converte o texto do ano para int
                int qtdVa = partes[3].toInt();
                string isbnVa = partes[4].toStdString(); // ISBN agora é lido como string

                // Adiciona o livro recuperado de volta ao vetor
                livros.push_back(Livro(tituloStr.toStdString(), autorStr.toStdString(), anoVal, qtdVa, isbnVa));
            }
        }
        arquivo.close();
    } else {
        // Se o arquivo não existir (primeira vez abrindo o app), tudo bem.
        cout << "Nenhum arquivo de livros encontrado ou erro ao abrir." << endl;
    }
}

// Funcoes de ordenacao de lista baseada no ano
void ordenarLivrosPorAnoCresc(vector<Livro>& livros) {
    int n = livros.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Compara os anos. Se quiser por título, seria: livros[j].titulo > livros[j+1].titulo
            if (livros[j].ano > livros[j + 1].ano) {
                // Troca os elements usando std::swap da biblioteca <algorithm>
                std::swap(livros[j], livros[j + 1]);
            }
        }
    }
}
void ordenarLivrosPorAnoDecresc(vector<Livro>& livros) {
    int n = livros.size();
    for (int i=0;i < n; i++) {
        for (int j=0; j < n-i-1; j++) {
            if (livros[j].ano < livros[j+1].ano) {
                std::swap(livros[j], livros[j+1]);
            }
        }
    }
}

// Lógica original de dados (intocada, não conectada aos botões ainda)
vector<Livro> vetorDeLivros;

int main(int argc, char *argv[]) {
    // Inicializa a aplicação Qt
    QApplication app(argc, argv);
    carregarLivrosDoArquivo(vetorDeLivros);

    // --- CRIAÇÃO DA PARTE VISUAL ---

    // Cria a janela principal
    QWidget janela;
    janela.setWindowTitle("Gerenciador de Biblioteca");
    janela.setMinimumSize(1000, 400); // Aumentado o tamanho para caber a lista e o formulário

    // --- LADO ESQUERDO: LISTA DE LIVROS ---
    QGroupBox *grupoLista = new QGroupBox("Acervo Atual", &janela);
    QVBoxLayout *layoutLista = new QVBoxLayout();

    QListWidget *listaVisual = new QListWidget();

    // Preenchendo a lista visual com os dados do vetor (apenas visualização inicial)
      for (const Livro& l : vetorDeLivros) {
          QString texto = QString::fromStdString(l.titulo) + " - " +
                          QString::fromStdString(l.autor) + " (" +
                          QString::number(l.ano) + ") - " +
                          QString::number(l.qtd) + " unid. - ISBN: " +
                          QString::fromStdString(l.isbn);

          listaVisual->addItem(texto);
      }

    layoutLista->addWidget(listaVisual);

    // Cria um layout horizontal para os botões de ordenação
    QHBoxLayout *layoutBotoesOrdenacao = new QHBoxLayout();

    // Botão para acionar a ordenação
    QPushButton *btnOrdenarAnoCresc = new QPushButton("Ordenar por Ano Crescente");
    QPushButton *btnOrdenarAnoDecresc = new QPushButton("Ordenar por Ano Decrescente");

    // Adiciona os botões no layout horizontal
    layoutBotoesOrdenacao->addWidget(btnOrdenarAnoCresc);
    layoutBotoesOrdenacao->addWidget(btnOrdenarAnoDecresc);

    // Adiciona o layout horizontal ao layout vertical principal da lista
    layoutLista->addLayout(layoutBotoesOrdenacao);

    grupoLista->setLayout(layoutLista);


    // --- LADO DIREITO: FORMULÁRIO ---
    QGroupBox *grupoFormulario = new QGroupBox("Adicionar Novo Livro", &janela);
    QFormLayout *layoutFormulario = new QFormLayout();

    QLineEdit *campoTitulo = new QLineEdit();
    campoTitulo->setPlaceholderText("Ex: O Senhor dos Anéis");
    layoutFormulario->addRow("Título:", campoTitulo);

    QLineEdit *campoAutor = new QLineEdit();
    campoAutor->setPlaceholderText("Ex: J.R.R. Tolkien");
    layoutFormulario->addRow("Autor:", campoAutor);

    QSpinBox *campoQtd = new QSpinBox();
    campoQtd-> setRange(1,100);
    campoQtd->setValue(1);
    layoutFormulario->addRow("Quantidade de Cópias:", campoQtd);

    QSpinBox *campoAno = new QSpinBox();
    campoAno->setRange(0, 2026);
    campoAno->setValue(2026);
    layoutFormulario->addRow("Ano de Lançamento:", campoAno);

    // Usaremos um QLineEdit para o ISBN porque um QSpinBox/int não suporta números tão grandes
    // Além disso, ISBNs podem começar com zero.
    QLineEdit *campoISBN = new QLineEdit();
    campoISBN->setPlaceholderText("Ex: 9781234567890");
    // ISBN costuma ter 13 dígitos
    campoISBN->setMaxLength(13);

    QRegularExpression rx("^[0-9]{0,13}$");
    QValidator *validator = new QRegularExpressionValidator(rx, campoISBN);
    campoISBN->setValidator(validator);

    layoutFormulario->addRow("ISBN:", campoISBN);

    QPushButton *btnAdicionar = new QPushButton("Salvar Livro");

    // Botões do formulário
    QHBoxLayout *layoutBotoes = new QHBoxLayout();
    layoutBotoes->addStretch();
    layoutBotoes->addWidget(btnAdicionar);

    // Junta os campos e os botões dentro do GroupBox do formulário
    QVBoxLayout *layoutFormularioPrincipal = new QVBoxLayout();
    layoutFormularioPrincipal->addLayout(layoutFormulario);
    layoutFormularioPrincipal->addStretch(); // Empurra os campos pra cima e os botões pra baixo
    layoutFormularioPrincipal->addLayout(layoutBotoes);
    grupoFormulario->setLayout(layoutFormularioPrincipal);


    // --- ORGANIZAÇÃO FINAL (LISTA NA ESQUERDA, FORMULÁRIO NA DIREITA) ---
    QHBoxLayout *layoutPrincipal = new QHBoxLayout(&janela);

    // Define que a lista ocupa mais espaço na tela (ratio 2) e o formulário menos (ratio 1)
    layoutPrincipal->addWidget(grupoLista, 1);
    layoutPrincipal->addWidget(grupoFormulario, 1);

    // Exibe a janela na tela
    janela.show();

    // Ação do botão de adicionar
    QObject::connect(btnAdicionar, &QPushButton::clicked, [&] {
        QString titulo = campoTitulo->text().trimmed();
        QString autor = campoAutor->text().trimmed();
        // Como removemos o InputMask, o texto do ISBN já vem limpo.
        QString isbn = campoISBN->text().trimmed();

        if (!titulo.isEmpty() && !autor.isEmpty() && !isbn.isEmpty()) {

            bool livroJaExiste = false;
            // 1. Verifica se o livro já existe pelo ISBN
            for (int i=0; i < vetorDeLivros.size(); i++) {
                if (vetorDeLivros[i].isbn == isbn.toStdString()) {
                    vetorDeLivros[i].qtd += campoQtd->value();
                    livroJaExiste = true;
                    break;
                }
            }

            // 2. Se não existe, adiciona no vetor de dados
            if (!livroJaExiste) {
                vetorDeLivros.push_back(Livro(
                    titulo.toStdString(),
                    autor.toStdString(),
                    campoAno->value(),
                    campoQtd->value(),
                    isbn.toStdString()
                ));
            }

            // 3. Atualiza a lista visual
            listaVisual->clear();
            for (const Livro& l : vetorDeLivros) {
                QString texto = QString::fromStdString(l.titulo) + " - " +
                                QString::fromStdString(l.autor) + " (" +
                                QString::number(l.ano) + ") - " +
                                QString::number(l.qtd) + " unid. - ISBN: " +
                                QString::fromStdString(l.isbn);
                listaVisual->addItem(texto);
            }

            // 4. Salva no arquivo
            salvarLivrosNoArquivo(vetorDeLivros);

            // 5. Limpa os campos para o próximo cadastro
            campoTitulo->clear();
            campoAutor->clear();
            campoISBN->clear();
            campoAno->setValue(2026);
            campoQtd->setValue(1);
            campoTitulo->setFocus();

        } else {
            QMessageBox::warning(&janela, "Erro", "Preencha todos os campos corretamente para adicionar um livro.");
        }
    });

    // Ação do botão de ordenar
    QObject::connect(btnOrdenarAnoCresc, &QPushButton::clicked, [&] {
        // 1. Aplica o bubble sort no vetor
        ordenarLivrosPorAnoCresc(vetorDeLivros);
        listaVisual->clear();
        for (const Livro& l : vetorDeLivros) {
            QString texto = QString::fromStdString(l.titulo) + " - " +
                            QString::fromStdString(l.autor) + " (" +
                            QString::number(l.ano) + ") - " +
                            QString::number(l.qtd) + " unid. - ISBN: " +
                            QString::fromStdString(l.isbn);
            listaVisual->addItem(texto);
        }
    });
    QObject::connect(btnOrdenarAnoDecresc, &QPushButton::clicked, [&] {
        ordenarLivrosPorAnoDecresc(vetorDeLivros);
        listaVisual->clear();
        for (const Livro& l : vetorDeLivros) {
            QString texto = QString::fromStdString(l.titulo) + " - " +
                            QString::fromStdString(l.autor) + " (" +
                            QString::number(l.ano) + ") - " +
                            QString::number(l.qtd) + " unid. - ISBN: " +
                            QString::fromStdString(l.isbn);
            listaVisual->addItem(texto);
        }


    });

    // Inicia o loop da interface gráfica
    return app.exec();
}