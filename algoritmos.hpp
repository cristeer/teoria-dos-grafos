#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include "arquivos.hpp"
#include <vector>
#include <queue>
#include <stack>
#include <string>

bool salvar_arvore_busca(const string& caminho_saida, int vertices, const vector<int>& pai, const vector<int>& nivel);

void bfs_lista(int s, int vertices, const lista_adj& adj, vector<int>& pai, vector<int>& nivel, vector<int>& componente_atual, int vertice_destino);
void dfs_lista(int s, int vertices, const lista_adj& adj, vector<int>& pai, vector<int>& nivel);

void bfs_matriz(int s, int vertices, const matriz_adj& mat, vector<int>& pai, vector<int>& nivel, vector<int>& componente_atual,int vertice_destino);
void dfs_matriz(int s, int vertices, const matriz_adj& mat, vector<int>& pai, vector<int>& nivel);

vector<vector<int>> cc_lista(int vertices, const lista_adj& adj);
vector<vector<int>> cc_matriz(int vertices, const matriz_adj& mat);
bool salvar_componentes_conexas(const string& caminho_saida, const vector<vector<int>>& componentes);

int diametro_aproximado_lista(int vertices,const lista_adj& adj);
int diametro_aproximado_matriz(int vertices,const matriz_adj& mat);

#endif
