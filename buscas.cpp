#include "buscas.hpp"

 
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

    arq << "{{=====}} [ ARVORE DE BUSCA ] {{=====}}\n";
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

void bfs_lista(int s, int vertices, const lista_adj& adj, vector<int>& pai, vector<int>& nivel) {
    //vetores  começando em 1, preenchidos por -1
    pai.assign(vertices + 1, -1);
    nivel.assign(vertices + 1, UNVISITED);

    queue<int> fila;
    
    nivel[s] = 0;  // vertice inicial dado pela biblioteca
    pai[s] = 0;    // a raiz
    fila.push(s);

    while (!fila.empty()) {
        int u = fila.front(); 
        fila.pop();

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

void bfs_matriz(int s, int vertices, const matriz_adj& mat, vector<int>& pai, vector<int>& nivel) {
    pai.assign(vertices + 1, -1);
    nivel.assign(vertices + 1, UNVISITED);

    queue<int> fila;
    nivel[s] = 0;
    pai[s] = 0;
    fila.push(s);

    while (!fila.empty()) {
        int u = fila.front(); 
        fila.pop();

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


