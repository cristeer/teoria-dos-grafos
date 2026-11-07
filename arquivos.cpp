#include "arquivos.hpp"
#include <cmath>
#include <sstream>

namespace {
struct aresta_entrada {
    int origem;
    int destino;
    double peso;
};

bool ler_arestas_arquivo(const string& caminho, int& vertices, vector<aresta_entrada>& arestas) {
    ifstream arq(caminho);
    if (!arq.is_open()) {
        cerr << "Erro ao abrir o arquivo do grafo: " << caminho << "\n";
        return false;
    }

    int quantidade_vertices;
    if (!(arq >> quantidade_vertices) || quantidade_vertices <= 0) {
        cerr << "Quantidade de vertices invalida no arquivo: " << caminho << "\n";
        return false;
    }

    string linha;
    getline(arq, linha);
    vector<aresta_entrada> arestas_lidas;
    while (getline(arq, linha)) {
        if (linha.find_first_not_of(" \t\r\n") == string::npos) {
            continue;
        }

        istringstream entrada(linha);
        aresta_entrada aresta{0, 0, 1.0};
        if (!(entrada >> aresta.origem >> aresta.destino)) {
            cerr << "Linha de aresta invalida no arquivo: " << linha << "\n";
            return false;
        }

        entrada >> ws;
        if (!entrada.eof()) {
            if (!(entrada >> aresta.peso)) {
                cerr << "Peso invalido na linha: " << linha << "\n";
                return false;
            }
            entrada >> ws;
            if (!entrada.eof()) {
                cerr << "Colunas extras na linha de aresta: " << linha << "\n";
                return false;
            }
        }

        if (aresta.origem < 1 || aresta.origem > quantidade_vertices ||
            aresta.destino < 1 || aresta.destino > quantidade_vertices ||
            !std::isfinite(aresta.peso)) {
            cerr << "Aresta ou peso fora dos limites: " << linha << "\n";
            return false;
        }
        arestas_lidas.push_back(aresta);
    }

    vertices = quantidade_vertices;
    arestas = move(arestas_lidas);
    return true;
}
}




void gera_pastas(){
    
    filesystem::create_directories("Saidas/Listas");
    filesystem::create_directories("Saidas/Matriz");

    filesystem::create_directories("Saidas/Dijkstra/Vetor");
    filesystem::create_directories("Saidas/Dijkstra/Heap");

}

//=================== LISTA DE ADJACENCIA =================
// gera o arquivo com as informações nessesárias do grafo;
bool gerar_informacoes_lista(const string &caminhoSaida, int vertices, const lista_adj &lista, const vector<vector<int>>& componentes){
    

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
    vector<aresta_entrada> arestas;
    if (!ler_arestas_arquivo(caminho, vertices, arestas)) {
        return false;
    }

    lista_adj.assign(vertices + 1, {});
    for (const aresta_entrada& aresta : arestas) {
        lista_adj[aresta.origem].push_back(aresta.destino);
        lista_adj[aresta.destino].push_back(aresta.origem);
    }
    return true;
}

bool ler_grafo_lista(const string& caminho, int& vertices, lista_adj_ponderada& lista) {
    vector<aresta_entrada> arestas;
    if (!ler_arestas_arquivo(caminho, vertices, arestas)) {
        return false;
    }

    lista.assign(vertices + 1, {});
    for (const aresta_entrada& aresta : arestas) {
        lista[aresta.origem].push_back({aresta.destino, aresta.peso});
        lista[aresta.destino].push_back({aresta.origem, aresta.peso});
    }
    return true;
}

//================ MATRIZ DE ADJACENCIA =================
bool gerar_informacoes_matriz(const string& caminhoSaida,int vertices,const matriz_adj& matriz, const vector<vector<int>>& componentes) {

    
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
    vector<aresta_entrada> arestas;
    if (!ler_arestas_arquivo(caminho, vertices, arestas)) {
        return false;
    }

    const int limite = 50000;
    if (vertices > limite) {
        cout << "\n[ERRO] Memoria Insuficiente\n";
        cout << "Utilize Lista de Adjacencia para esta instancia.\n";
        return false;
    }

    matriz_adj.assign(vertices + 1, vector<bool>(vertices + 1, false));
    for (const aresta_entrada& aresta : arestas) {
        matriz_adj[aresta.origem][aresta.destino] = true;
        matriz_adj[aresta.destino][aresta.origem] = true;
    }
    return true;
}

bool ler_grafo_matriz(const string& caminho, int& vertices, matriz_adj_ponderada& matriz) {
    vector<aresta_entrada> arestas;
    if (!ler_arestas_arquivo(caminho, vertices, arestas)) {
        return false;
    }

    const int limite = 50000;
    if (vertices > limite) {
        cerr << "\n[ERRO] Memoria Insuficiente\n";
        cerr << "Utilize Lista de Adjacencia para esta instancia.\n";
        return false;
    }

    matriz.assign(vertices + 1, vector<celula_matriz_ponderada>(
        vertices + 1, celula_matriz_ponderada{false, 0.0}));
    for (const aresta_entrada& aresta : arestas) {
        matriz[aresta.origem][aresta.destino] = {true, aresta.peso};
        matriz[aresta.destino][aresta.origem] = {true, aresta.peso};
    }
    return true;
}


