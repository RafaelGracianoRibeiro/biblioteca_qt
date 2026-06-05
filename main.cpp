#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <QListWidget> // Incluído para a lista visual
#include <QRegularExpressionValidator>
#include <QHeaderView>
#include <vector>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QPropertyAnimation>
#include <iostream>
#include <algorithm> // Incluído para std::swap
#include <stack>
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
    QFile logLivrosAdicionados("livros_adicionados");

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

void salvarPilha(const stack<string>& pilha) {
    QFile arquivo("pilha.txt");
    if (arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream saida(&arquivo);
        stack<string> temp = pilha;
        stack<string> reversa;
        while (!temp.empty()) {
            reversa.push(temp.top());
            temp.pop();
        }
        while (!reversa.empty()) {
            saida << QString::fromStdString(reversa.top()) << "\n";
            reversa.pop();
        }
    }
}
void carregarPilha(stack<string>& pilha) {
    QFile arquivo("pilha.txt");
    if (arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream entrada(&arquivo);
        while (!entrada.atEnd()) {
            QString linha = entrada.readLine();
            if (!linha.trimmed().isEmpty()) {
                pilha.push(linha.toStdString());
            }
        }
        arquivo.close();
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
void preencherTabelaVisual(QTableWidget *tabela, const vector<Livro> &livros){
    for (const Livro& l: livros){
        int linhaAtual = tabela->rowCount();
        tabela->insertRow(linhaAtual);

        tabela->setItem(linhaAtual, 0, new QTableWidgetItem(QString::fromStdString(l.titulo)));
        tabela->setItem(linhaAtual, 1, new QTableWidgetItem(QString::fromStdString(l.autor)));
        tabela->setItem(linhaAtual, 2, new QTableWidgetItem(QString::number(l.ano)));
        tabela->setItem(linhaAtual, 3, new QTableWidgetItem(QString::number(l.qtd)));
        tabela->setItem(linhaAtual, 4, new QTableWidgetItem(QString::fromStdString(l.isbn)));
    }
}
void preencherPilhaVisual(QListWidget *lista, stack<string> &pilha) {
    lista->clear();
    stack<string> copiaPilha = pilha;
    while (!copiaPilha.empty()) {
        lista->addItem(QString::fromStdString(copiaPilha.top()));
        copiaPilha.pop();
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
stack<string> livrosAdicionados;

int main(int argc, char *argv[]) {
    // Inicializa a aplicação Qt
    QApplication app(argc, argv);
    carregarLivrosDoArquivo(vetorDeLivros);
    carregarPilha(livrosAdicionados);


    // Cria a janela principal
    QWidget janela;
    janela.setWindowTitle("Gerenciador de Biblioteca");
    janela.setMinimumSize(1600, 600); // Aumentado o tamanho para caber a lista e o formulário

    // --- LADO ESQUERDO: LISTA DE LIVROS ---
    QGroupBox *grupoLista = new QGroupBox("Acervo Atual", &janela);
    QVBoxLayout *layoutLista = new QVBoxLayout();

    auto *tabelaVisual = new QTableWidget();
    tabelaVisual->setColumnCount(5);
    tabelaVisual->setHorizontalHeaderLabels({"Título", "Autor", "Ano", "Quantidade", "ISBN"});
    tabelaVisual->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabelaVisual->setSelectionMode(QAbstractItemView::SingleSelection);
    tabelaVisual->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabelaVisual->resizeRowsToContents();


    preencherTabelaVisual(tabelaVisual, vetorDeLivros);

    layoutLista->addWidget(tabelaVisual);

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


    // --- LADO DIREITO: FORMULÁRIO E ÚLTIMOS VISTOS ---
    QVBoxLayout *layoutDireito = new QVBoxLayout();

    // Grupo do Formulário
    QGroupBox *grupoFormulario = new QGroupBox("Adicionar Novo Livro", &janela);
    QFormLayout *layoutFormulario = new QFormLayout();

    QLineEdit *campoTitulo = new QLineEdit();
    campoTitulo->setPlaceholderText("Ex: O Senhor dos Anéis");
    layoutFormulario->addRow("Título:", campoTitulo);

    QLineEdit *campoAutor = new QLineEdit();
    campoAutor->setPlaceholderText("Ex: J.R.R. Tolkien");
    layoutFormulario->addRow("Autor:", campoAutor);

    // Usaremos um QLineEdit para o ISBN porque um QSpinBox/int não suporta números tão grandes
    // Além disso, ISBNs podem começar com zero.
    QLineEdit *campoISBN = new QLineEdit();
    campoISBN->setPlaceholderText("Ex: 777777777777");
    campoISBN->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,13}$"), campoISBN));
    layoutFormulario->addRow("ISBN-13:", campoISBN);

    QSpinBox *campoQtd = new QSpinBox();
    campoQtd-> setRange(1,100);
    campoQtd->setValue(1);
    layoutFormulario->addRow("Quantidade de Cópias:", campoQtd);

    QSpinBox *campoAno = new QSpinBox();
    campoAno->setRange(0, 2026);
    campoAno->setValue(2026);
    layoutFormulario->addRow("Ano de Lançamento:", campoAno);

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

    // Grupo da Pilha (Últimos Vistos)
    QGroupBox *grupoUltimosVistos = new QGroupBox("Últimos Livros Adicionados ao Sistema", &janela);
    QVBoxLayout *layoutUltimosVistos = new QVBoxLayout();
    QListWidget *pilhaUltimosAdicionados = new QListWidget();
    layoutUltimosVistos->addWidget(pilhaUltimosAdicionados);
    grupoUltimosVistos->setLayout(layoutUltimosVistos);

    // Adiciona os grupos ao layout da direita
    layoutDireito->addWidget(grupoFormulario);
    layoutDireito->addWidget(grupoUltimosVistos);

    // --- ORGANIZAÇÃO FINAL (LISTA NA ESQUERDA, LAYOUT DIREITO NA DIREITA) ---
    QHBoxLayout *layoutPrincipal = new QHBoxLayout(&janela);

    layoutPrincipal->addWidget(grupoLista, 3);
    layoutPrincipal->addLayout(layoutDireito, 1);

    preencherPilhaVisual(pilhaUltimosAdicionados, livrosAdicionados);

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
                livrosAdicionados.push(titulo.toStdString());
                preencherPilhaVisual(pilhaUltimosAdicionados, livrosAdicionados);
            }

            // 3. Atualiza a lista visual
            tabelaVisual->setRowCount(0);
            preencherTabelaVisual(tabelaVisual, vetorDeLivros);

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

        salvarPilha(livrosAdicionados);
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
        tabelaVisual->setRowCount(0);
        preencherTabelaVisual(tabelaVisual, vetorDeLivros);

    });
    QObject::connect(btnOrdenarAnoDecresc, &QPushButton::clicked, [&] {
        ordenarLivrosPorAnoDecresc(vetorDeLivros);
        tabelaVisual->setRowCount(0);
        preencherTabelaVisual(tabelaVisual, vetorDeLivros);


    });

    // Inicia o loop da interface gráfica
    return app.exec();
}