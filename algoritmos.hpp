#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include "arquivos.hpp"
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <chrono>
#include <limits>

void registrarTempo(int verticeInicial, long long tempo);

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

// DIJKSTRA (grafos com peso)
// Distância "infinita" = vértice ainda não alcançado
const double INF_DIST = numeric_limits<double>::infinity();

enum tipo_fila_dijkstra { FILA_VETOR = 1, FILA_HEAP = 2 };

// Abstração de COMO as estimativas de distância são guardadas/atualizadas
// e de COMO o mínimo é extraído. O Dijkstra só conhece estas 2 operações.

struct fila_distancias {
    vector<double> chave;     // estimativa de distância de cada vértice (1..n)
    chrono::steady_clock::time_point prazo;
    bool limite_tempo_ativo = false;
    bool limite_tempo_atingido = false;

    // usado só pela versão VETOR
    vector<char>   na_fila;   // na_fila[v] = v tem estimativa e ainda não foi extraído

    // usado só pela versão HEAP
    vector<int>    heap_v;    // heap binário de vértices (ordenado por chave)
    vector<int>    pos;       // pos[v] = posição de v em heap_v (-1 = fora do heap)

    // atualizar(v, d): insere v com chave d, ou DIMINUI a chave de v para d
    void (*atualizar)(fila_distancias& f, int v, double d);
    // extrair_min: remove o vértice de menor chave; false se a fila estiver vazia
    bool (*extrair_min)(fila_distancias& f, int& v, double& d);
};

void fila_vetor_iniciar(fila_distancias& f, int vertices);   // O(V) por extração
void fila_heap_iniciar(fila_distancias& f, int vertices);    // O(log V) por operação

// Algoritmo único (independe da estrutura usada)
void dijkstra_generico(int s, int vertices, const lista_adj_ponderada& adj,
                       fila_distancias& fila, vector<int>& pai);

// Ponto de entrada: valida pesos (negativo => avisa e retorna false),
// escolhe a estrutura (FILA_VETOR / FILA_HEAP) e devolve dist/pai.
// dist[v] = INF_DIST e pai[v] = -1 se v for inalcançável; pai[s] = 0.
bool dijkstra_lista(int s, int vertices, const lista_adj_ponderada& adj, int tipo_fila,
                    vector<double>& dist, vector<int>& pai);

bool grafo_tem_peso_negativo(int vertices, const lista_adj_ponderada& adj);

// Caminho s -> t seguindo a árvore de pais (vazio se t inalcançável)
vector<int> caminho_minimo(const vector<int>& pai, int t);

// Salva "Vertice  Pai  Distancia" (árvore de caminhos mínimos)
bool salvar_dijkstra(const string& caminho_saida, int vertices, int s,
                     const vector<double>& dist, const vector<int>& pai);

#endif
