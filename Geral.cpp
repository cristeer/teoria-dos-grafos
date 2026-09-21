#include"Geral.h"

 


bool ler_grafo_lista(const string& caminho, int& vertices, lista_adj& lista_adj) {
    int u, v;   
    ifstream arq(caminho);
    if (!arq.is_open()) {
        cout << "Não encontramos o arquivo!\n";
        return false;
    }

    arq >> vertices;
    lista_adj.assign (vertices+1, {});

    while (arq >> u >> v) {
        lista_adj[u].push_back(v);
        lista_adj[v].push_back(u);
    }
    return true;
}

bool ler_grafo_matriz(const string& caminho, int& vertices, matriz_adj& matriz_adj) {
   int u, v;   
    ifstream arq(caminho);
    if (!arq.is_open())
        cout << "Não encontramos o arquivo!\n";

    arq >> vertices;
    matriz_adj.assign(vertices+1, vector<bool>(vertices+1, false));


    while (arq >> u >> v) {
        matriz_adj[u][v] = true;
        matriz_adj[v][u] = true;
    }
    return true;
}   