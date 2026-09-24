#ifndef BUSCAS_HPP
#define BUSCAS_HPP

#include "arquivos.hpp"
#include <vector>
#include <queue>
#include <stack>
#include <string>


bool salvar_arvore_busca(const string& caminho_saida, int vertices, const vector<int>& pai, const vector<int>& nivel);


void bfs_lista(int s, int vertices, const lista_adj& adj, vector<int>& pai, vector<int>& nivel);
void dfs_lista(int s, int vertices, const lista_adj& adj, vector<int>& pai, vector<int>& nivel);

void bfs_matriz(int s, int vertices, const matriz_adj& mat, vector<int>& pai, vector<int>& nivel);
void dfs_matriz(int s, int vertices, const matriz_adj& mat, vector<int>& pai, vector<int>& nivel);

#endif
