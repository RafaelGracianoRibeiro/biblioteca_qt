#include <QApplication>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QRegularExpressionValidator>
#include <QHeaderView>
#include <vector>
#include <string>
#include <QTextStream>
#include <QDir>
#include <QPropertyAnimation>
#include <iostream>
#include <algorithm>
#include <stack>
#include <queue>
#include <QScrollBar>
#include <QMessageBox>
#include <QScrollArea>

using namespace std;

class Livro {
public:
    string titulo;
    string autor;
    int ano;
    int qtd;
    string isbn;

    Livro(string t, string a, int y, int q, string isbn) : titulo(std::move(t)), autor(std::move(a)), ano(y), qtd(q), isbn(std::move(isbn)) {}
};

struct Emprestimo {
    string nomePessoa;
    string tiuloLivro;
    string isbn;
};

//Funções para salvar e carregar os dados salvos, manipulando dados em csv no arquivo txt e no vetor e pilha de livros
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
                const QString& tituloStr = partes[0];
                const QString& autorStr = partes[1];
                int anoVal = partes[2].toInt(); // Converte o texto do ano para int
                int qtdVal = partes[3].toInt();
                string isbnVal = partes[4].toStdString(); // ISBN agora é lido como string

                // Adiciona o livro recuperado de volta ao vetor
                livros.emplace_back(tituloStr.toStdString(), autorStr.toStdString(), anoVal, qtdVal, isbnVal);
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

void salvarFila(queue<Emprestimo> fila) {
    QFile arquivo("fila_emprestimos.txt");
    if (arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream saida(&arquivo);
        while (!fila.empty()) {
            Emprestimo emprestimo = fila.front();
            saida << QString::fromStdString(emprestimo.nomePessoa) << ";"
                  << QString::fromStdString(emprestimo.tiuloLivro) << ";"
                  << QString::fromStdString(emprestimo.isbn) << "\n";
            fila.pop();
        }
        arquivo.close();
    }
}
void carregarFila(queue<Emprestimo>& fila) {
    QFile arquivo("fila_emprestimos.txt");
    if (arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream entrada(&arquivo);
        while (!entrada.atEnd()) {
            QString linha = entrada.readLine();
            if (!linha.trimmed().isEmpty()) {
                QStringList partes = linha.split(";");
                if (partes.size() == 3){
                    fila.push({partes[0].toStdString(), partes[1].toStdString(), partes[2].toStdString()});
                }
            }
        }
        arquivo.close();
    }
}

void ordenarLivrosPorAnoCresc(vector<Livro>& livros) {
    int n = livros.size();
    if (n == 0) {
        return; // Não há o que ordenar
    }

    // 1. Crie um vetor auxiliar para o resultado e uma cópia para trabalhar
    vector<Livro> aux;
    vector<Livro> copia = livros;

    //Repete o processo n vezes
    for (int i = 0; i < n; i++) {

        //Encontrando o menor ano na cópia
        int indiceDoMenor = 0;
        for (int j = 1; j < copia.size(); j++) {
            if (copia[j].ano < copia[indiceDoMenor].ano) {
                indiceDoMenor = j; //
            }
        }

        //Adicionando esse livro ao seu novo vetor ordenado
        aux.push_back(copia[indiceDoMenor]);

        //Remova o livro da cópia para não pegá-lo de novo
        copia.erase(copia.begin() + indiceDoMenor);
    }

    //No final, o vetor 'aux' está ordenado. Copie-o de volta.
    livros = aux;
} // --> Selection Sort

void ordenarLivrosPorAnoDecresc(vector<Livro>& livros) {
    int n = livros.size();
    for (int i=0;i < n; i++) {
        for (int j=0; j < n-i-1; j++) {
            if (livros[j].ano < livros[j+1].ano) {
                Livro aux = livros[j];
                livros[j] = livros[j+1];
                livros[j+1] = aux;
            }
        }
    }
}// --> Bubble Sort

void ordenarLivrosPorISBN(vector<Livro>& livros) {
    for (size_t i = 1; i < livros.size(); i++) {
        Livro chave = livros[i];
        int j = i - 1;
        while (j >= 0 && livros[j].isbn > chave.isbn) {
            livros[j + 1] = livros[j];
            j = j - 1;
        }
        livros[j + 1] = chave;
    }
} // --> Insertion Sort

int buscaBinariaPorISBN(const vector<Livro>& livros, const string& isbn) {
    int inicio = 0;
    int fim = static_cast<int>(livros.size()) - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        if (livros[meio].isbn == isbn) {
            return meio; // Encontrou
        }
        if (livros[meio].isbn < isbn) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1; // Não encontrou
}

//Função que preenche a lista visual com os elementos do vetor
void preencherTabelaVisual(QTableWidget *tabela, const vector<Livro> &livros){
    tabela->clearContents();
    tabela->setRowCount(0);
    for (const auto& l: livros){
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
void preencherFilaVisual(QListWidget *lista, queue<Emprestimo> fila) {
    lista->clear();
    while (!fila.empty()) {
        Emprestimo emp = fila.front();
        lista->addItem(QString::fromStdString(emp.nomePessoa) +
            " - " + QString::fromStdString(emp.tiuloLivro) +
            " - " + QString::fromStdString(emp.isbn));
        fila.pop();
    }
}

//Funcao de feedback visual
void shakeWidget(QWidget *widget){
    if (!widget || widget->property("shaking_animation_running").toBool()){
        return;
    }
    widget->setProperty("shaking_animation_running", true);
    auto *animation = new QPropertyAnimation(widget,"pos");
    QPoint originalPos = widget->pos();
    animation->setDuration(500);
    animation->setKeyValueAt(0.0, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.1, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(0.2, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.3, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(0.4, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.5, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(0.6, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.7, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(0.8, originalPos + QPoint(-10, 0));
    animation->setKeyValueAt(0.9, originalPos + QPoint(10, 0));
    animation->setKeyValueAt(1.0, originalPos);
    QObject::connect(animation, &QPropertyAnimation::finished, widget, [widget](){
        widget->setProperty("shaking_animation_running", QVariant(false));
    });
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

//Funcao de validar isbn-13
bool validarIsbn(const QString& isbnQstring) {
    string isbn = isbnQstring.toStdString();

    // 1. Verificação de tamanho: o ISBN-13 deve ter exatos 13 caracteres numéricos.
    if (isbn.length() != 13) {
        return false;
    }

    int soma = 0;

    // 2. Loop para calcular a soma ponderada dos 12 primeiros dígitos.
    for (int i = 0; i < 12; ++i) {
        // Converte o caractere atual (que está em ASCII) para o valor inteiro correspondente.
        // A subtração por '0' funciona pois em ASCII os números são sequenciais ('0' é 48, '1' é 49, etc).
        int digito = isbn[i] - '0';

        // O algoritmo do ISBN-13 aplica pesos alternados:
        // Dígitos em posições ímpares (índices pares 0, 2, 4...) recebem peso 1.
        if (i % 2 == 0) {
            soma += digito * 1;
        }
        // Dígitos em posições pares (índices ímpares 1, 3, 5...) recebem peso 3.
        else {
            soma += digito * 3;
        }
    }

    // 3. O dígito verificador é calculado subtraindo o resto da divisão da soma por 10 de 10.
    // Exemplo: se soma = 104 -> 104 % 10 = 4 -> 10 - 4 = 6. O verificador deve ser 6.
    int digitoVerificadorCalculado = 10 - (soma % 10);

    // Há um caso especial na regra do ISBN-13: se a conta acima resultar em 10, o dígito verificador é 0.
    if (digitoVerificadorCalculado == 10) {
        digitoVerificadorCalculado = 0;
    }

    // 4. Capturamos o dígito verificador real que veio na string (o último caractere, índice 12).
    int digitoVerificadorOriginal = isbn[12] - '0';

    // O ISBN é válido apenas se o dígito que calculamos for igual ao último dígito fornecido.
    return digitoVerificadorCalculado == digitoVerificadorOriginal;
}

bool deletarLivro(vector<Livro>& livros, const string& isbn) {
    int indexParaDeletar = -1;
    for (int i = 0; i < livros.size(); ++i) {
        if (livros[i].isbn == isbn) {
            indexParaDeletar = i;
            break;
        }
    }

    if (indexParaDeletar != -1) {
        // Mover todos os elementos após o índice para uma posição para trás
        for (int i = indexParaDeletar; i < livros.size() - 1; ++i) {
            livros[i] = livros[i + 1];
        }
        // Remover o último elemento, que agora é um duplicado
        livros.pop_back();
        return true;
    }

    return false; // Retorna false se não encontrou
}


// Definição de vetores, pilhas e filas
vector<Livro> vetorDeLivros;
stack<string> livrosAdicionados;
queue<Emprestimo> FilaEmprestimos;

int main(int argc, char *argv[]) {
    // Inicializa a aplicação Qt
    QApplication app(argc, argv);


    QFile file(":/style.qss");
    if(file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
        file.close();
    }

    carregarLivrosDoArquivo(vetorDeLivros);
    carregarPilha(livrosAdicionados);
    carregarFila(FilaEmprestimos);


    // Cria a janela principal
    QWidget janela;
    janela.setObjectName("janelaPrincipal");
    janela.setWindowTitle("Gerenciador de Biblioteca");
    janela.setMinimumSize(1200, 700);
    janela.setWindowIcon(QIcon(":/ico.png"));

    auto *layoutMain = new QVBoxLayout(&janela);
    layoutMain->setContentsMargins(20, 20, 20, 20);
    layoutMain->setSpacing(20);

    auto *layoutPrincipal = new QHBoxLayout();
    layoutMain->addLayout(layoutPrincipal);

    // --- LADO ESQUERDO: LISTA DE LIVROS ---
    auto *grupoLista = new QGroupBox("Acervo de Dados");
    auto *layoutLista = new QVBoxLayout();
    layoutLista->setContentsMargins(15, 25, 15, 15);
    layoutLista->setSpacing(15);

    auto *tabelaVisual = new QTableWidget();
    tabelaVisual->setColumnCount(5);
    tabelaVisual->setHorizontalHeaderLabels({"Título", "Autor", "Ano", "Qtd", "ISBN"});
    tabelaVisual->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabelaVisual->setSelectionMode(QAbstractItemView::SingleSelection);
    tabelaVisual->setSelectionBehavior(QAbstractItemView::SelectItems);
    tabelaVisual->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabelaVisual->setAlternatingRowColors(false);
    tabelaVisual->verticalHeader()->setVisible(false);


    preencherTabelaVisual(tabelaVisual, vetorDeLivros);

    layoutLista->addWidget(tabelaVisual);

    // Cria um layout horizontal para os botões de ordenação
    auto *layoutBotoesOrdenacao = new QHBoxLayout();
    layoutBotoesOrdenacao->setSpacing(15);

    // Botão para acionar a ordenação
    auto *btnOrdenarAnoCresc = new QPushButton("Sort(ANO) : Crescente");
    auto *btnOrdenarAnoDecresc = new QPushButton("Sort(ANO) : Decrescente");

    // Adiciona os botões no layout horizontal
    layoutBotoesOrdenacao->addWidget(btnOrdenarAnoCresc);
    layoutBotoesOrdenacao->addWidget(btnOrdenarAnoDecresc);

    // Adiciona o layout horizontal ao layout vertical principal da lista
    layoutLista->addLayout(layoutBotoesOrdenacao);

    grupoLista->setLayout(layoutLista);


    // --- LADO DIREITO: ÁREA DE ROLAGEM ---
    auto *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    auto *containerDireito = new QWidget();
    auto *layoutDireito = new QVBoxLayout(containerDireito);
    layoutDireito->setSpacing(20);
    scrollArea->setWidget(containerDireito);


    // Grupo do Formulário
    auto *grupoFormulario = new QGroupBox("Adicionar Livro");
    auto *layoutFormulario = new QFormLayout();
    layoutFormulario->setContentsMargins(15, 30, 15, 15);
    layoutFormulario->setSpacing(15);
    layoutFormulario->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    auto *campoTitulo = new QLineEdit();
    campoTitulo->setPlaceholderText(">> TÍTULO DO LIVRO");
    layoutFormulario->addRow("Título:", campoTitulo);

    auto *campoAutor = new QLineEdit();
    campoAutor->setPlaceholderText(">> IDENTIFICAÇÃO DO AUTOR");
    layoutFormulario->addRow("Autor:", campoAutor);

    auto *campoISBN = new QLineEdit();
    campoISBN->setPlaceholderText(">> CÓDIGO 13-DIGITOS");
    campoISBN->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,13}$"), campoISBN));
    layoutFormulario->addRow("ISBN-13:", campoISBN);

    auto *campoQtd = new QSpinBox();
    campoQtd-> setRange(1,100);
    campoQtd->setValue(1);
    layoutFormulario->addRow("Quantidade:", campoQtd);

    auto *campoAno = new QSpinBox();
    campoAno->setRange(0, 2026);
    campoAno->setValue(2026);
    layoutFormulario->addRow("Ano de lançamento:", campoAno);

    auto *btnAdicionar = new QPushButton("Adicionar");
    btnAdicionar->setObjectName("btnAdicionar"); // Identificador para o QSS
    btnAdicionar->setMinimumHeight(42);

    // Botões do formulário
    auto *layoutBotoes = new QHBoxLayout();
    layoutBotoes->addStretch();
    layoutBotoes->addWidget(btnAdicionar);

    // Junta os campos e os botões dentro do GroupBox do formulário
    auto *layoutFormularioPrincipal = new QVBoxLayout();
    layoutFormularioPrincipal->addLayout(layoutFormulario);
    layoutFormularioPrincipal->addStretch();
    layoutFormularioPrincipal->addLayout(layoutBotoes);
    grupoFormulario->setLayout(layoutFormularioPrincipal);

    // --- NOVO GRUPO: BUSCAR LIVRO ---
    auto *grupoBuscar = new QGroupBox("Buscar Livro");
    auto *layoutBuscar = new QVBoxLayout();
    layoutBuscar->setContentsMargins(15, 25, 15, 15);

    auto *isbnBuscar = new QLineEdit();
    isbnBuscar->setPlaceholderText(">>> ISBN para Buscar");
    isbnBuscar->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,13}$"), isbnBuscar));
    layoutBuscar->addWidget(isbnBuscar);

    auto *btnBuscar = new QPushButton("Buscar (Binária)");
    btnBuscar->setMinimumHeight(42);
    layoutBuscar->addWidget(btnBuscar);

    grupoBuscar->setLayout(layoutBuscar);

    // --- NOVO GRUPO: DELETAR LIVRO ---
    auto *grupoDeletar = new QGroupBox("Deletar Livro");
    auto *layoutDeletar = new QVBoxLayout();
    layoutDeletar->setContentsMargins(15, 25, 15, 15);

    auto *isbnDeletar = new QLineEdit();
    isbnDeletar->setPlaceholderText(">>> ISBN para Deletar");
    isbnDeletar->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,13}$"), isbnDeletar));
    layoutDeletar->addWidget(isbnDeletar);

    auto *btnDeletar = new QPushButton("Deletar");
    btnDeletar->setMinimumHeight(42);
    // Pode aplicar um estilo específico para chamar atenção (ex: vermelho), mas vamos manter consistente por enquanto
    layoutDeletar->addWidget(btnDeletar);

    grupoDeletar->setLayout(layoutDeletar);

    auto *grupoEmprestimo = new QGroupBox("Novo Emprestimo");
    auto *layoutEmprestimo = new QVBoxLayout();

    auto *nomeEmprestimo = new QLineEdit();
    nomeEmprestimo->setPlaceholderText(">>> Cliente");
    layoutEmprestimo->addWidget(nomeEmprestimo);

    auto *isbnEmprestimo = new QLineEdit();
    isbnEmprestimo->setPlaceholderText(">>> ISBN");
    isbnEmprestimo->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,13}$"), isbnEmprestimo));
    layoutEmprestimo->addWidget(isbnEmprestimo);

    auto *btnEmprestimo = new QPushButton("Emprestar");
    btnEmprestimo->setMinimumHeight(42);
    layoutEmprestimo->addWidget(btnEmprestimo);

    auto *grupoFilaEmprestimos = new QGroupBox("Fila de Empréstimos");
    auto *layoutFilaEmprestimos = new QVBoxLayout();
    layoutFilaEmprestimos->setContentsMargins(15, 25, 15, 15);
    auto *listaEprestimos = new QListWidget();
    listaEprestimos->setMinimumHeight(200); // Define altura mínima de 200px
    layoutFilaEmprestimos->addWidget(listaEprestimos);

    grupoEmprestimo->setLayout(layoutEmprestimo);
    grupoFilaEmprestimos->setLayout(layoutFilaEmprestimos);

    // Grupo da Pilha (Últimos Vistos)
    auto *grupoUltimosVistos = new QGroupBox("Livros Adicionados ao Sistema");
    auto *layoutUltimosVistos = new QVBoxLayout();
    layoutUltimosVistos->setContentsMargins(15, 25, 15, 15);
    auto *pilhaUltimosAdicionados = new QListWidget();
    pilhaUltimosAdicionados->setMinimumHeight(200); // Define altura mínima de 200px
    layoutUltimosVistos->addWidget(pilhaUltimosAdicionados);
    grupoUltimosVistos->setLayout(layoutUltimosVistos);

    // Adiciona os grupos ao layout da direita
    layoutDireito->addWidget(grupoFormulario);
    layoutDireito->addWidget(grupoBuscar);
    layoutDireito->addWidget(grupoDeletar);
    layoutDireito->addWidget(grupoUltimosVistos, 5);
    layoutDireito->addWidget(grupoEmprestimo);
    layoutDireito->addWidget(grupoFilaEmprestimos, 5);
    layoutDireito->addStretch();

    // --- ORGANIZAÇÃO FINAL (LISTA NA ESQUERDA, LAYOUT DIREITO NA DIREITA) ---
    layoutPrincipal->addWidget(grupoLista, 7);
    layoutPrincipal->addWidget(scrollArea, 3);

    preencherPilhaVisual(pilhaUltimosAdicionados, livrosAdicionados);
    preencherFilaVisual(listaEprestimos, FilaEmprestimos);


    // Exibe a janela na tela
    janela.showMaximized();

    // Ação do botão de adicionar
    QObject::connect(btnAdicionar, &QPushButton::clicked, [=, &janela] {
        QString titulo = campoTitulo->text().trimmed();
        QString autor = campoAutor->text().trimmed();
        QString isbn = campoISBN->text().trimmed();

        if (!titulo.isEmpty() && !autor.isEmpty() && !isbn.isEmpty() && validarIsbn(isbn)) {
            bool livroJaExiste = false;
            // 1. Verifica se o livro já existe pelo ISBN
            for (auto & livro : vetorDeLivros) {
                if (livro.isbn == isbn.toStdString()) {
                    livro.qtd += campoQtd->value();
                    livroJaExiste = true;
                    QMessageBox::information(&janela, "Sucesso", "Quantidade atualizada para o livro " + titulo);
                    break;
                }
            }
            // 2. Se não existe, adiciona no vetor de dados
            if (!livroJaExiste) {
                vetorDeLivros.emplace_back(
                    titulo.toStdString(),
                    autor.toStdString(),
                    campoAno->value(),
                    campoQtd->value(),
                    isbn.toStdString()
                );
                livrosAdicionados.push(titulo.toStdString());
                preencherPilhaVisual(pilhaUltimosAdicionados, livrosAdicionados);
                QMessageBox::information(&janela, "Sucesso", "Novo livro " + titulo + " cadastrado com sucesso!");
            }

            // 3. Atualiza a lista visual
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

            campoISBN->setStyleSheet("");
        } else{
            if (titulo.isEmpty()){shakeWidget(campoTitulo);}
            if (autor.isEmpty()){shakeWidget(campoAutor);}
            if (isbn.isEmpty()){shakeWidget(campoISBN);}
            if (!validarIsbn(isbn) && !isbn.isEmpty()){
                shakeWidget(campoISBN);
                QMessageBox::warning(&janela, "Erro de Validação", "O ISBN informado é inválido. Verifique o código e tente novamente.");
            } else {
                QMessageBox::warning(&janela, "Campos Obrigatórios", "Por favor, preencha todos os campos obrigatórios.");
            }
        }

        salvarPilha(livrosAdicionados);
    });

    QObject::connect(btnBuscar, &QPushButton::clicked, [=, &janela] {
        QString isbn = isbnBuscar->text().trimmed();
        if (!isbn.isEmpty()) {
            // A busca binária requer que o vetor esteja ordenado
            ordenarLivrosPorISBN(vetorDeLivros);
            preencherTabelaVisual(tabelaVisual, vetorDeLivros);

            int index = buscaBinariaPorISBN(vetorDeLivros, isbn.toStdString());
            if (index != -1) {
                QString titulo = QString::fromStdString(vetorDeLivros[index].titulo);
                QMessageBox::information(&janela, "Livro Encontrado", "Título: " + titulo + "\nEstoque: " + QString::number(vetorDeLivros[index].qtd));
                tabelaVisual->selectRow(index); // Destaca na tabela
                isbnBuscar->clear();
                isbnBuscar->setStyleSheet("");
            } else {
                QMessageBox::warning(&janela, "Não Encontrado", "Nenhum livro encontrado com este ISBN.");
            }
        } else {
            shakeWidget(isbnBuscar);
            QMessageBox::warning(&janela, "Campo Obrigatório", "Por favor, informe o ISBN do livro a ser buscado.");
        }
    });

    QObject::connect(btnDeletar, &QPushButton::clicked, [=, &janela] {
        QString isbn = isbnDeletar->text().trimmed();
        if (!isbn.isEmpty()) {
            if (deletarLivro(vetorDeLivros, isbn.toStdString())) {
                preencherTabelaVisual(tabelaVisual, vetorDeLivros);
                salvarLivrosNoArquivo(vetorDeLivros);
                QMessageBox::information(&janela, "Sucesso", "Livro deletado com sucesso.");
                isbnDeletar->clear();
                isbnDeletar->setStyleSheet("");
            } else {
                QMessageBox::warning(&janela, "Erro", "Livro não encontrado com o ISBN informado.");
            }
        } else {
            shakeWidget(isbnDeletar);
            QMessageBox::warning(&janela, "Campo Obrigatório", "Por favor, informe o ISBN do livro a ser deletado.");
        }
    });

    QObject::connect(btnEmprestimo, &QPushButton::clicked, [=, &janela] {
        QString nome = nomeEmprestimo->text().trimmed();
        QString isbn = isbnEmprestimo->text().trimmed();
        if (!nome.isEmpty() && !isbn.isEmpty()) {
            bool livroEncontrado = false;
            for (int i=0; i < vetorDeLivros.size(); i++) {
                if (vetorDeLivros[i].isbn == isbn.toStdString()) {
                    livroEncontrado = true;
                    if (vetorDeLivros[i].qtd > 0) {
                        vetorDeLivros[i].qtd--;
                        FilaEmprestimos.push({nome.toStdString(), vetorDeLivros[i].titulo, isbn.toStdString()});
                        preencherFilaVisual(listaEprestimos, FilaEmprestimos);
                        salvarFila(FilaEmprestimos);
                        preencherTabelaVisual(tabelaVisual,vetorDeLivros);
                        salvarLivrosNoArquivo(vetorDeLivros);

                        QMessageBox::information(&janela, "Empréstimo Registrado", "Empréstimo de " + QString::fromStdString(vetorDeLivros[i].titulo) + " para " + nome + " registrado.");

                        nomeEmprestimo->clear();
                        isbnEmprestimo->clear();
                        nomeEmprestimo->setFocus();
                        break;
                    } else {
                        QMessageBox::warning(&janela, "Estoque Insuficiente", "O livro " + QString::fromStdString(vetorDeLivros[i].titulo) + " está sem estoque no momento.");
                        break;
                    }
                }
            }
            if (!livroEncontrado) {
                 QMessageBox::warning(&janela, "Livro Não Encontrado", "Não existe nenhum livro cadastrado com o ISBN informado.");
            }
        } else {
            if (nome.isEmpty()){shakeWidget(nomeEmprestimo);}
            if (isbn.isEmpty()){shakeWidget(isbnEmprestimo);}
            QMessageBox::warning(&janela, "Campos Obrigatórios", "Por favor, informe o nome do cliente e o ISBN do livro.");
        }
    });

    // Evento disparado toda vez que o usuário digita ou apaga algo no campo ISBN
    QObject::connect(campoISBN, &QLineEdit::textChanged, [=](const QString &texto) {
        if (texto.length() == 13) {
            if (validarIsbn(texto)) {
                // Validação Ciano
                campoISBN->setStyleSheet("border: 1px solid #00e5ff; color: #2EBF24;");
            } else {
                // Erro Vermelho
                campoISBN->setStyleSheet("border: 1px solid #ff4444; color: #ff4444;");
            }
        } else {
            campoISBN->setStyleSheet("");
        }
    });
    QObject::connect(isbnBuscar, &QLineEdit::textChanged, [=](const QString &texto) {
        if (texto.length() == 13) {
            if (validarIsbn(texto)) {
                isbnBuscar->setStyleSheet("border: 1px solid #00e5ff; color: #2EBF24;");
            } else {
                isbnBuscar->setStyleSheet("border: 1px solid #ff4444; color: #ff4444;");
            }
        } else {
            isbnBuscar->setStyleSheet("");
        }
    });
    QObject::connect(isbnEmprestimo, &QLineEdit::textChanged, [=](const QString &texto) {
        if (texto.length() == 13) {
          if (validarIsbn(texto)) {
              // Validação Ciano
              isbnEmprestimo->setStyleSheet("border: 1px solid #00e5ff; color: #2EBF24;");
          } else {
              // Erro Vermelho
              isbnEmprestimo->setStyleSheet("border: 1px solid #ff4444; color: #ff4444;");
          }
      } else {
          isbnEmprestimo->setStyleSheet("");
      }
    });
    QObject::connect(isbnDeletar, &QLineEdit::textChanged, [=](const QString &texto) {
        if (texto.length() == 13) {
            if (validarIsbn(texto)) {
                isbnDeletar->setStyleSheet("border: 1px solid #00e5ff; color: #2EBF24;");
            } else {
                isbnDeletar->setStyleSheet("border: 1px solid #ff4444; color: #ff4444;");
            }
        } else {
            isbnDeletar->setStyleSheet("");
        }
    });

    // Ação do botão de ordenar
    QObject::connect(btnOrdenarAnoCresc, &QPushButton::clicked, [=, &janela] {
        ordenarLivrosPorAnoCresc(vetorDeLivros);
        preencherTabelaVisual(tabelaVisual, vetorDeLivros);
        QMessageBox::information(&janela, "Ordenação", "A lista de livros foi ordenada por ano de lançamento em ordem crescente.");
    });
    QObject::connect(btnOrdenarAnoDecresc, &QPushButton::clicked, [=, &janela] {
        ordenarLivrosPorAnoDecresc(vetorDeLivros);
        preencherTabelaVisual(tabelaVisual, vetorDeLivros);
        QMessageBox::information(&janela, "Ordenação", "A lista de livros foi ordenada por ano de lançamento em ordem decrescente.");
    });

    btnAdicionar->setCursor(Qt::PointingHandCursor);
    btnBuscar->setCursor(Qt::PointingHandCursor);
    btnDeletar->setCursor(Qt::PointingHandCursor);
    btnOrdenarAnoCresc->setCursor(Qt::PointingHandCursor);
    btnOrdenarAnoDecresc->setCursor(Qt::PointingHandCursor);
    campoQtd->setCursor(Qt::PointingHandCursor);
    campoAno->setCursor(Qt::PointingHandCursor);

    // Inicia o loop da interface gráfica
    return QApplication::exec();
}