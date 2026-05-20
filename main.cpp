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
#include <QPropertyAnimation>
#include <iostream>
#include <algorithm> // Incluído para std::swap
using namespace std;

class Livro {
public:
    string titulo;
    string autor;
    int ano;
    int qtd;
    string isbn;

    Livro(string t, string a, int y, int q, string isbn) : titulo(t), autor(a), ano(y), qtd(q), isbn(isbn) {}
};

//Funções para salvar e carregar os dados salvos, manipulando dados em csv no arquivo txt e no vetro de livros
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
                int qtdVal = partes[3].toInt();
                string isbnVal = partes[4].toStdString(); // ISBN agora é lido como string

                // Adiciona o livro recuperado de volta ao vetor
                livros.push_back(Livro(tituloStr.toStdString(), autorStr.toStdString(), anoVal, qtdVal, isbnVal));
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
                swap(livros[j], livros[j + 1]);
            }
        }
    }
}
void ordenarLivrosPorAnoDecresc(vector<Livro>& livros) {
    int n = livros.size();
    for (int i=0;i < n; i++) {
        for (int j=0; j < n-i-1; j++) {
            if (livros[j].ano < livros[j+1].ano) {
                swap(livros[j], livros[j+1]);
            }
        }
    }
}

//Função que preenche a lista visual com os elementos do vetor
void preencheListaVisual(QListWidget *listaVisual, const vector<Livro>& vetorDeLivros){
    // Preenchendo a lista visual com os dados do vetor (apenas visualização inicial)
    for (const Livro& l : vetorDeLivros) {
        QString texto = QString::fromStdString(l.titulo) + " - " +
                        QString::fromStdString(l.autor) + " (" +
                        QString::number(l.ano) + ") - " +
                        QString::number(l.qtd) + " unid. - ISBN: " +
                        QString::fromStdString(l.isbn);

        listaVisual->addItem(texto);
    }
}
void shakeWidget(QWidget *widget){
    if (!widget){
        return;
    }
    auto *animation = new QPropertyAnimation(widget,"pos");
    QPoint originalPos = widget->pos();
    animation->setDuration(500);
    animation->setKeyValueAt(0.0, originalPos + QPoint(-10, 0));  // Move 10 pixels para a esquerda
    animation->setKeyValueAt(0.1, originalPos + QPoint(10, 0));   // Move 10 pixels para a direita
    animation->setKeyValueAt(0.2, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.3, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(0.4, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.5, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(0.6, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.7, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(0.8, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.9, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(1.0, originalPos);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

bool validarIsbn(const QString& isbnQstring) {
    string isbn = isbnQstring.toStdString();
    if (isbn.length() != 13) {
        return false;
    }

    int soma = 0;
    for (int i = 0; i < 12; ++i) {
        // Converte o caractere para inteiro
        int digito = isbn[i] - '0';

        // Aplica o peso (1 para posições pares, 3 para ímpares, no índice 0-based)
        if (i % 2 == 0) {
            soma += digito * 1;
        } else {
            soma += digito * 3;
        }
    }

    // Calcula o dígito verificador esperado
    int digitoVerificadorCalculado = 10 - (soma % 10);
    if (digitoVerificadorCalculado == 10) {
        digitoVerificadorCalculado = 0;
    }

    // Pega o dígito verificador original do ISBN
    int digitoVerificadorOriginal = isbn[12] - '0';

    // Compara se são iguais
    return digitoVerificadorCalculado == digitoVerificadorOriginal;
}

// Definição do vetor de livros
vector<Livro> vetorDeLivros;

int main(int argc, char *argv[]) {
    // Inicializa a aplicação Qt
    QApplication app(argc, argv);
    carregarLivrosDoArquivo(vetorDeLivros);

    // --- CRIAÇÃO DA PARTE VISUAL ---

    // Cria a janela principal
    QWidget janela;
    janela.setWindowTitle("Gerenciador de Biblioteca");
    janela.setMinimumSize(1600, 600); // Aumentado o tamanho para caber a lista e o formulário

    // --- LADO ESQUERDO: LISTA DE LIVROS ---
    QGroupBox *grupoLista = new QGroupBox("Acervo Atual", &janela);
    QVBoxLayout *layoutLista = new QVBoxLayout();

    QListWidget *listaVisual = new QListWidget();

    preencheListaVisual(listaVisual, vetorDeLivros);

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
    campoISBN->setPlaceholderText("Ex: 777777777777");
    campoISBN->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,13}$"), campoISBN));
    layoutFormulario->addRow("ISBN-13:", campoISBN);
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
        QString isbn = campoISBN->text().trimmed();

        if (!titulo.isEmpty() && !autor.isEmpty() && !isbn.isEmpty() && validarIsbn(isbn)) {

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
            preencheListaVisual(listaVisual, vetorDeLivros);

            // 4. Salva no arquivo
            salvarLivrosNoArquivo(vetorDeLivros);

            // 5. Limpa os campos para o próximo cadastro
            campoTitulo->clear();
            campoAutor->clear();
            campoISBN->clear();
            campoAno->setValue(2026);
            campoQtd->setValue(1);
            campoTitulo->setFocus();
        } else{
            if (titulo.isEmpty()){shakeWidget(campoTitulo);}
            if (autor.isEmpty()){shakeWidget(campoAutor);}
            if (isbn.isEmpty()){shakeWidget(campoISBN);}
        }
    });

    // Evento disparado toda vez que o usuário digita ou apaga algo no campo ISBN
    QObject::connect(campoISBN, &QLineEdit::textChanged, [&](const QString &texto) {
        if (texto.length() == 13) {
            if (validarIsbn(texto)) {
                // ISBN válido: Borda verde
                campoISBN->setStyleSheet("border: 2px solid green; border-radius: 4px; padding: 2px;");
            } else {
                // ISBN inválido: Borda vermelha
                campoISBN->setStyleSheet("border: 2px solid red; border-radius: 4px; padding: 2px;");
            }
        } else {
            // Menos de 10 dígitos: Tira as cores (volta ao padrão)
            campoISBN->setStyleSheet("");
        }
    });

    // Ação do botão de ordenar
    QObject::connect(btnOrdenarAnoCresc, &QPushButton::clicked, [&] {
        // 1. Aplica o bubble sort no vetor
        ordenarLivrosPorAnoCresc(vetorDeLivros);
        listaVisual->clear();
        preencheListaVisual(listaVisual, vetorDeLivros);

    });
    QObject::connect(btnOrdenarAnoDecresc, &QPushButton::clicked, [&] {
        ordenarLivrosPorAnoDecresc(vetorDeLivros);
        listaVisual->clear();
        preencheListaVisual(listaVisual, vetorDeLivros);


    });

    // Inicia o loop da interface gráfica
    return app.exec();
}