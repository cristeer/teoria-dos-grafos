#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <string>
using namespace std;

using lista_adj = vector<vector<int>>; //ex: 1 -> 2, 3, 4 
using matriz_adj = vector<vector<bool>>;

// Lê o grafo do ficheiro e preenche a lista de adjacência (Requisito 1 e 3)
bool ler_grafo_lista(const string& caminho, int& vertices, lista_adj& lista_adj);

// Lê o grafo do ficheiro e preenche a matriz de adjacência (Requisito 1 e 3)
bool ler_grafo_matriz(const string& caminho, int& vertices, matriz_adj& matriz_adj);


#endif
