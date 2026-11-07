#ifndef ARQUIVOS_HPP
#define ARQUIVOS_HPP

#include<iostream>
#include<fstream>
#include<vector>
#include<filesystem>
#include<iomanip>
#include<algorithm>

using namespace std;

using lista_adj = vector<vector<int>>; //ex: 1 -> 2, 3, 4 
using matriz_adj = vector<vector<bool>>;//matrix básica...

struct aresta_ponderada {
    int destino;
    double peso;
};

struct celula_matriz_ponderada {
    bool adjacente;
    double peso;
};

using lista_adj_ponderada = vector<vector<aresta_ponderada>>;
using matriz_adj_ponderada = vector<vector<celula_matriz_ponderada>>;

void salvadora();

void gera_pastas();

bool gerar_informacoes_lista(const string& caminhoSaida, int vertices, const lista_adj& lista, const vector<vector<int>>& componentes);
bool ler_grafo_lista(const string&  caminho, int& vertices, lista_adj& lista_adj);
bool ler_grafo_lista(const string& caminho, int& vertices, lista_adj_ponderada& lista);

bool gerar_informacoes_matriz(const string& caminhoSaida, int vertices, const matriz_adj& matriz, const vector<vector<int>>& componentes);
bool ler_grafo_matriz(const string& caminho, int& vertices, matriz_adj& matriz_adj);
bool ler_grafo_matriz(const string& caminho, int& vertices, matriz_adj_ponderada& matriz);




#endif