#include "arquivos.hpp"

void verificadorDePastaSimples(filesystem::path pastinha){
    // Tenta criar a pasta
    if (filesystem::create_directories(pastinha)) {
        cout << "Pasta criada com sucesso!" << endl;
    } else {
        cout << "A pasta ja existe" << endl;
    }
}

// Gera o arquivo com as informações nessesárias do grafo;
bool gerar_informacoes_lista(const string &caminhoSaida, int vertices, const lista_adj &lista){

    filesystem::path pasta = "Saidas/Listas";verificadorDePastaSimples(pasta);
    
    ofstream arq(caminhoSaida, ios::out | ios::trunc);
    if (!arq.is_open()){
        cout << "Não foi possível criar o arquivo de saída\n";
        return false;
    }

    vector<int> graus; // Vetor para armazenar o grau de cada vértice

    int numeroArestas = 0;

    // Percorre os vértices e contar
    for (int i = 1; i <= vertices; i++){
        int aux = lista[i].size();
        graus.push_back(aux);
        numeroArestas += aux;
    }
    numeroArestas /= 2; // n(n-1) / 2

    sort(graus.begin(), graus.end());
    // ordena os graus para calcular
    // a mediana e o grau mínimo/máximo

    int grauMinimo = graus.front();
    int grauMaximo = graus.back();

    double grauMedio = 0;
    for (int aux : graus){
        grauMedio += aux;
    }
    grauMedio /= vertices;

    // Mediana
    double mediana;
    if (vertices % 2 == 1){
        // Quantidade ímpar de vértices
        mediana = graus[vertices / 2];
    }
    else{
        // Quantidade par de vértices
        mediana = (graus[(vertices / 2) - 1] + graus[vertices / 2]) / 2.0;
    }

    // =================================================================================
    // ESCRITA DO ARQUIVO
    // =================================================================================

    arq << "{{=====}} [  INFORMAÇÕES DO GRAFO  ] {{=====}}\n";
    arq << "Numero de vértices: " << vertices << "\n";
    arq << "Numero de arestas: " << numeroArestas << "\n\n";
    arq << "Grau mínimo: " << grauMinimo << "\n";
    arq << "Grau máximo: " << grauMaximo << "\n";
    arq << fixed << setprecision(2);
    arq << "Grau médio: " << grauMedio << "\n";
    arq << "Mediana dos graus: " << mediana << "\n";
    arq.close();

    return true;
}

bool ler_grafo_lista(const string &caminho, int &vertices, lista_adj &lista_adj){
    int u, v;
    ifstream arq(caminho);
    if (!arq.is_open()){
        cout << "Erro de leitura/ Lista\n";
        return false;
    }

    arq >> vertices;
    lista_adj.assign(vertices + 1, {});

    while (arq >> u >> v){
        lista_adj[u].push_back(v);
        lista_adj[v].push_back(u);
    }
    return true;
}

//  ======  MATRIZ  ===================================================================

bool gerar_informacoes_matriz(const string& caminhoSaida,int vertices,const matriz_adj& matriz) {

    filesystem::path pasta = "Saidas/Matriz";verificadorDePastaSimples(pasta);
    
    ofstream arq(caminhoSaida, ios::out | ios::trunc);
    if (!arq.is_open()){
        cout << "Não foi possível criar o arquivo de saída\n";
        return false;
    }

    vector<int> graus;

    int somaGraus = 0;

    for (int i = 1; i <= vertices; i++) {
        int grau = 0;

        // Percorre a linha do vértice
        for (int j = 1; j <= vertices; j++) {
            if (matriz[i][j]) {
                grau++;
            }
        }
        graus.push_back(grau);
        somaGraus += grau;
    }
    // Como o grafo é não direcionado,
    // cada aresta aparece duas vezes na matriz:
    // matriz[u][v] = 1
    // matriz[v][u] = 1
    int numeroArestas = somaGraus / 2;

    sort(graus.begin(), graus.end());// ORDENA OS GRAUS
    int grauMinimo = graus.front();
    int grauMaximo = graus.back();

    double grauMedio = static_cast<double>(somaGraus) / vertices;

    double mediana;

    if (vertices % 2 == 1) {
        // Número ímpar de vértices
        mediana = graus[vertices / 2];
    } else {
        // Número par de vértices
        mediana =(graus[(vertices / 2) - 1] +graus[vertices / 2]) / 2.0;
    }

    // =================================================================================
    // ESCRITA DO ARQUIVO
    // =================================================================================

    arq << "{{=====}} [  INFORMAÇÕES DO GRAFO  ] {{=====}}\n";
    arq << "Numero de vértices: "<< vertices << "\n";
    arq << "Numero de arestas: "<< numeroArestas << "\n\n";
    arq << "Grau mínimo: "<< grauMinimo << "\n";
    arq << "Grau máximo: "<< grauMaximo << "\n";
    arq << fixed << setprecision(2);
    arq << "Grau médio: "<< grauMedio << "\n";
    arq << "Mediana dos graus: "<< mediana << "\n";
    arq.close();

    return true;
}

bool ler_grafo_matriz(const string &caminho, int &vertices, matriz_adj &matriz_adj){
    int u, v;

    ifstream arq(caminho);
    if (!arq.is_open()){
        cout << "Erro de leitura/ Matriz\n";
        return false;
    }

    arq >> vertices;
    matriz_adj.assign(vertices + 1, vector<bool>(vertices + 1, false));

    while (arq >> u >> v){
        matriz_adj[u][v] = true;
        matriz_adj[v][u] = true;
    }
    return true;
}