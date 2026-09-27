#include "algoritmos.hpp"
#include <algorithm>

int UNVISITED = -1;

bool salvar_arvore_busca(const string& caminho_saida, int vertices, const vector<int>& pai, const vector<int>& nivel) {
    filesystem::path pasta = filesystem::path(caminho_saida).parent_path();
    if (!pasta.empty()) {
        verificadorDePastaSimples(pasta);
    }

    ofstream arq(caminho_saida, ios::out | ios::trunc);
    if (!arq.is_open()) {
        cout << "Erro ao criar o arquivo de arvore de busca!\n";
        return false;
    }

    arq << "ARVORE DE BUSCA ]\n";
    arq << "Vertice\tPai\tNivel\n";
    arq << "-----------------------------\n";

    for (int i = 1; i <= vertices; ++i) {
        if (nivel[i] != UNVISITED) {
            arq << i << "\t" << pai[i] << "\t" << nivel[i] << "\n";
        } else {
            arq << i << "\t-\tInalcançável\n";
        }
    }

    arq.close();
    return true;
}

void bfs_lista(int s, int vertices, const lista_adj& adj, vector<int>& pai, vector<int>& nivel, vector<int>& componente_atual) {
    queue<int> fila;
    
    nivel[s] = 0;  // vertice inicial dado pela biblioteca
    pai[s] = 0;    // a raiz
    fila.push(s);

    while (!fila.empty()) {
        int u = fila.front(); 
        fila.pop();
        componente_atual.push_back(u);

        for (int v : adj[u]) {
            if (nivel[v] != UNVISITED)
                continue;

            nivel[v] = nivel[u] + 1; //  nível do filho
            pai[v] = u;             // registrar o pai
            fila.push(v);
            
        }
    }
}

void dfs_lista(int s, int vertices, const lista_adj& adj, vector<int>& pai, vector<int>& nivel) {
    pai.assign(vertices + 1, -1);
    nivel.assign(vertices + 1, UNVISITED);

    stack<int> st;
    st.push(s); 
    pai[s] = 0;

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        if (nivel[u] != UNVISITED)
            continue;

        int p = pai[u];
        nivel[u] = (p == 0) ? 0 : nivel[p] + 1;

        for (int v : adj[u]) {
            if (nivel[v] == UNVISITED) {
                pai[v] = u;
                st.push(v);
            }
        }
    }
}

void bfs_matriz(int s, int vertices, const matriz_adj& mat, vector<int>& pai, vector<int>& nivel, vector<int>& componente_atual) {
    queue<int> fila;

    nivel[s] = 0;
    pai[s] = 0;
    fila.push(s);

    while (!fila.empty()) {
        int u = fila.front(); 
        fila.pop();
        componente_atual.push_back(u);

        for (int v = 1; v <= vertices; ++v) {
            if (mat[u][v] && nivel[v] == UNVISITED) {
                nivel[v] = nivel[u] + 1;
                pai[v] = u;
                fila.push(v);
            }
        }
    }
}


void dfs_matriz(int s, int vertices, const matriz_adj& mat, vector<int>& pai, vector<int>& nivel) {
    pai.assign(vertices + 1, -1);
    nivel.assign(vertices + 1, UNVISITED);

    stack<int> st;
    st.push(s);
    pai[s] = 0;

    while (!st.empty()) {
        int u = st.top();
        st.pop();

        if (nivel[u] != UNVISITED)
            continue;

        int p = pai[u];
        nivel[u] = (p == 0) ? 0 : nivel[p] + 1;

        for (int v = 1; v <= vertices; ++v) {
            if (mat[u][v] && nivel[v] == UNVISITED) {
                pai[v] = u;
                st.push(v);
            }
        }
    }
}

static bool comparar_componentes(const vector<int>& a, const vector<int>& b) {
    if (a.size() != b.size())
        return a.size() > b.size();
    return a.front() < b.front();
}

vector<vector<int>> cc_lista(int vertices, const lista_adj& adj) {
   vector<int> pai(vertices + 1, -1);
   vector<int> nivel(vertices + 1, UNVISITED);
   vector<vector<int>> componentes;

    for (int i = 1; i <= vertices; ++i) {
        if (nivel[i] == UNVISITED) {
            vector<int> componente_atual;
            bfs_lista(i, vertices, adj, pai, nivel, componente_atual);
            sort(componente_atual.begin(), componente_atual.end());
            componentes.push_back(componente_atual);
        }
    }

    sort(componentes.begin(), componentes.end(), comparar_componentes);
    return componentes;
}

vector<vector<int>> cc_matriz(int vertices, const matriz_adj& mat) {
    vector<int> pai(vertices + 1, -1);
    vector<int> nivel(vertices + 1, UNVISITED);
    vector<vector<int>> componentes;

    for (int i = 1; i <= vertices; ++i) {
        if (nivel[i] == UNVISITED) {
            vector<int> componente_atual;
            bfs_matriz(i, vertices, mat, pai, nivel, componente_atual);
            sort(componente_atual.begin(), componente_atual.end());
            componentes.push_back(componente_atual);
        }
    }

    sort(componentes.begin(), componentes.end(), comparar_componentes);
    return componentes;
}

bool salvar_componentes_conexas(const string& caminho_saida, const vector<vector<int>>& componentes) {
    filesystem::path pasta = filesystem::path(caminho_saida).parent_path();
    if (!pasta.empty()) {
        verificadorDePastaSimples(pasta);
    }

    ofstream arq(caminho_saida, ios::out | ios::trunc);
    if (!arq.is_open()) {
        cout << "Erro ao criar arquivo de componentes conexas!\n";
        return false;
    }

    arq << "COMPONENTES CONEXAS\n";
    arq << "Total de componentes: " << componentes.size() << "\n\n";

    for (size_t i = 0; i < componentes.size(); ++i) {
        arq << "Componente " << (i + 1) << "\tTamanho: " << componentes[i].size() << "\n";
        arq << "Vertices: ";
        for (size_t j = 0; j < componentes[i].size(); ++j) {
            arq << componentes[i][j] << (j + 1 == componentes[i].size() ? "" : " ");
        }
        arq << "\n\n";
    }

    arq.close();
    return true;
}