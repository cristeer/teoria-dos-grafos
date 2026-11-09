#include "algoritmos.hpp"
#include <algorithm>


int UNVISITED = -1;


void registrarTempo(int verticeInicial, long long tempo){
    // ios::app = abre o arquivo para adicionar no final
    ofstream arquivo("Agrupamento-Tempo-Total.txt", ios::app);

    if (!arquivo.is_open()){
        cerr << "Erro ao abrir o arquivo: " << endl;
        return;
    }

    //"Vertice inicial: X | Tempo: X microssegundos
    arquivo << verticeInicial << ";"<< tempo << endl;

    arquivo.close();
}


bool salvar_arvore_busca(const string& caminho_saida, int vertices, const vector<int>& pai, const vector<int>& nivel) {
   

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

//================ LISTA DE ADJACENCIA =================
void bfs_lista(int s, int vertices, const lista_adj& adj, vector<int>& pai, vector<int>& nivel, vector<int>& componente_atual, int vertice_destino) {
    queue<int> fila;
    bool encontrado = false;
    
    nivel[s] = 0;  // vertice inicial dado pela biblioteca
    pai[s] = 0;    // a raiz
    fila.push(s);

    while (!fila.empty()) {
        int u = fila.front(); 
        fila.pop();
        componente_atual.push_back(u);

        // Verifica se encontrou o destino
        if (vertice_destino != -1 && u == vertice_destino && !encontrado) {
            cout << "\nVertice encontrado: " << u << endl;
            cout << "Distancia: " << nivel[u] << endl;
            encontrado = true;
        }

        for (int v : adj[u]) {
            if (nivel[v] != UNVISITED) continue;
            nivel[v] = nivel[u] + 1;
            pai[v] = u;
            fila.push(v);
        }
    }

    // Caso a BFS termine sem encontrar o destino
    if (!encontrado && vertice_destino != -1) {
        cout << "\nVertice " << vertice_destino << " nao foi encontrado." << endl;
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

//======== MATRIZ DE ADJACENCIA ========
void bfs_matriz(int s, int vertices, const matriz_adj& mat, vector<int>& pai, vector<int>& nivel, vector<int>& componente_atual,int vertice_destino) {
    queue<int> fila;

    nivel[s] = 0;
    pai[s] = 0;
    fila.push(s);
    bool encontrado = false;

    while (!fila.empty()) {
        int u = fila.front(); 
        fila.pop();
        componente_atual.push_back(u);

        // Verifica se encontrou o destino
        if (vertice_destino != -1 && u == vertice_destino && !encontrado) {
            cout << "\nVertice encontrado: " << u << endl;
            cout << "Distancia: " << nivel[u] << endl;

            encontrado = true;
        }

        for (int v = 1; v <= vertices; ++v) {
            if (mat[u][v] && nivel[v] == UNVISITED) {
                nivel[v] = nivel[u] + 1;
                pai[v] = u;
                fila.push(v);
            }
        }
    }
    // Caso o destino não tenha sido encontrado
    if (!encontrado && vertice_destino != -1) {
        cout << "\nVertice " << vertice_destino << " nao foi encontrado." << endl;
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

//====COMPARAÇÃO DE COMPONENTES CONEXAS====
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
            bfs_lista(i, vertices, adj, pai, nivel, componente_atual, -1); 
            // Passando -1 como vertice_destino para não buscar um destino específico;
            // VS Code Quem disse pra adicionar esse Parâmetro o maluco é bravo. ele até escreveu isso do nada;
            //"? Eu não pedi isso, mas tudo bem, vamos deixar assim." kkkkkkkkkkkkkkkkkkkkkkkk;
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
            bfs_matriz(i, vertices, mat, pai, nivel, componente_atual, -1);
            sort(componente_atual.begin(), componente_atual.end());
            componentes.push_back(componente_atual);
        }
    }

    sort(componentes.begin(), componentes.end(), comparar_componentes);
    return componentes;
}


bool salvar_componentes_conexas(const string& caminho_saida, const vector<vector<int>>& componentes) {
   

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

//=====GERAR DIÂMETRO=====================
int diametro_aproximado_lista(int vertices,const lista_adj& adj) {
    int inicio = 1;

    // Vetores da primeira BFS
    vector<int> pai(vertices + 1, UNVISITED);
    vector<int> nivel(vertices + 1, UNVISITED);
    vector<int> componente_atual;

    // Primeira BFS
    bfs_lista(inicio,vertices,adj,pai,nivel,componente_atual,-1);

    // Procura o vértice mais distante
    int mais_distante = inicio;
    int maior_nivel = 0;

    for (int v = 1; v <= vertices; v++) {//procura o vertice mais distante da primeira BFS
        if (nivel[v] != UNVISITED &&
            nivel[v] > maior_nivel) {

            maior_nivel = nivel[v];
            mais_distante = v;
        }
    }

    // Limpa os vetores para a segunda BFS
    fill(pai.begin(), pai.end(), UNVISITED);
    fill(nivel.begin(), nivel.end(), UNVISITED);
    componente_atual.clear();

    // Segunda BFS, começando pelo vértice mais distante
    bfs_lista(mais_distante,vertices,adj,pai,nivel,componente_atual,-1);

    int diametro = 0;

    for (int v = 1; v <= vertices; v++) {//procura o vertice mais distante da segunda BFS
        if (nivel[v] != UNVISITED &&
            nivel[v] > diametro) {

            diametro = nivel[v];
        }
    }

    return diametro;
}


int diametro_aproximado_matriz(int vertices,const matriz_adj& mat) {
    
    int inicio = 1;

    // Vetores da primeira BFS
    vector<int> pai(vertices + 1, UNVISITED);
    vector<int> nivel(vertices + 1, UNVISITED);
    vector<int> componente_atual;

    // Primeira BFS
    bfs_matriz(inicio,vertices,mat,pai,nivel,componente_atual,-1);

    // Procura o vértice mais distante
    int mais_distante = inicio;
    int maior_nivel = 0;

    for (int v = 1; v <= vertices; v++) {
        if (nivel[v] != UNVISITED &&
            nivel[v] > maior_nivel) {

            maior_nivel = nivel[v];
            mais_distante = v;
        }
    }

    // Limpa os vetores
    fill(pai.begin(), pai.end(), UNVISITED);
    fill(nivel.begin(), nivel.end(), UNVISITED);
    componente_atual.clear();

    // Segunda BFS
    bfs_matriz(mais_distante,vertices,mat,pai,nivel,componente_atual,-1);

    // Procura a maior distância
    int diametro = 0;

    for (int v = 1; v <= vertices; v++) {
        if (nivel[v] != UNVISITED &&
            nivel[v] > diametro) {

            diametro = nivel[v];
        }
    }

    return diametro;
}


//=====================================================================
//                    DIJKSTRA (grafos com peso)
//=====================================================================

//---------- Estrutura 1: VETOR de estimativas ----------
// atualizar = O(1); extrair_min = varredura O(V)  =>  Dijkstra O(V^2 + E)
static void vetor_atualizar(fila_distancias& f, int v, double d) {
    f.chave[v] = d;
    f.na_fila[v] = 1;
}

static bool fila_limite_tempo_atingido(fila_distancias& f) {
    if (f.limite_tempo_ativo && chrono::steady_clock::now() >= f.prazo) {
        f.limite_tempo_atingido = true;
    }
    return f.limite_tempo_atingido;
}

static bool vetor_extrair_min(fila_distancias& f, int& v, double& d) {
    int melhor = -1;
    int n = (int)f.chave.size() - 1;
    for (int i = 1; i <= n; ++i) {
        if ((i & 4095) == 0 && fila_limite_tempo_atingido(f)) return false;
        if (f.na_fila[i] && (melhor == -1 || f.chave[i] < f.chave[melhor]))
            melhor = i;
    }
    if (melhor == -1) return false;
    f.na_fila[melhor] = 0;   // sai da fila; a chave (distância final) permanece
    v = melhor;
    d = f.chave[melhor];
    return true;
}

void fila_vetor_iniciar(fila_distancias& f, int vertices) {
    f.chave.assign(vertices + 1, INF_DIST);
    f.na_fila.assign(vertices + 1, 0);
    f.heap_v.clear();
    f.pos.clear();
    f.limite_tempo_ativo = false;
    f.limite_tempo_atingido = false;
    f.atualizar = vetor_atualizar;
    f.extrair_min = vetor_extrair_min;
}

//---------- Estrutura 2: HEAP BINÁRIO com índice de posição ----------
// pos[v] permite achar v dentro do heap em O(1) e diminuir sua chave em
// O(log V) ("decrease-key"), sem inserir duplicatas.
//  =>  Dijkstra O((V + E) log V)
static void heap_troca(fila_distancias& f, int i, int j) {
    swap(f.heap_v[i], f.heap_v[j]);
    f.pos[f.heap_v[i]] = i;
    f.pos[f.heap_v[j]] = j;
}

static void heap_subir(fila_distancias& f, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (f.chave[f.heap_v[p]] <= f.chave[f.heap_v[i]]) break;
        heap_troca(f, i, p);
        i = p;
    }
}

static void heap_descer(fila_distancias& f, int i) {
    int n = (int)f.heap_v.size();
    while (true) {
        int esq = 2 * i + 1, dir = esq + 1, menor = i;
        if (esq < n && f.chave[f.heap_v[esq]] < f.chave[f.heap_v[menor]]) menor = esq;
        if (dir < n && f.chave[f.heap_v[dir]] < f.chave[f.heap_v[menor]]) menor = dir;
        if (menor == i) break;
        heap_troca(f, i, menor);
        i = menor;
    }
}

static void heap_atualizar(fila_distancias& f, int v, double d) {
    f.chave[v] = d;
    if (f.pos[v] == -1) {                       // ainda não está no heap: insere
        f.heap_v.push_back(v);
        f.pos[v] = (int)f.heap_v.size() - 1;
    }
    heap_subir(f, f.pos[v]);                    // só diminui chave => só sobe
}

static bool heap_extrair_min(fila_distancias& f, int& v, double& d) {
    if (f.heap_v.empty()) return false;
    v = f.heap_v[0];
    d = f.chave[v];
    heap_troca(f, 0, (int)f.heap_v.size() - 1);
    f.heap_v.pop_back();
    f.pos[v] = -1;                              // a chave permanece em f.chave[v]
    if (!f.heap_v.empty()) heap_descer(f, 0);
    return true;
}

void fila_heap_iniciar(fila_distancias& f, int vertices) {
    f.chave.assign(vertices + 1, INF_DIST);
    f.pos.assign(vertices + 1, -1);
    f.heap_v.clear();
    f.na_fila.clear();
    f.limite_tempo_ativo = false;
    f.limite_tempo_atingido = false;
    f.atualizar = heap_atualizar;
    f.extrair_min = heap_extrair_min;
}

//---------- Algoritmo (UMA única implementação) ----------
void dijkstra_generico(int s, int vertices, const lista_adj_ponderada& adj,
                       fila_distancias& fila, vector<int>& pai) {
    pai.assign(vertices + 1, -1);
    pai[s] = 0;                       // raiz (mesma convenção da BFS/DFS)
    fila.atualizar(fila, s, 0.0);

    int u;
    double d;
    size_t arcos_processados = 0;
    while (true) {
        if (fila_limite_tempo_atingido(fila)) break;
        if (!fila.extrair_min(fila, u, d)) break;
        for (const aresta_ponderada& a : adj[u]) {
            if ((arcos_processados++ & 4095) == 0 &&
                fila_limite_tempo_atingido(fila)) return;
            double nova = d + a.peso;
            if (nova < fila.chave[a.destino]) {      // relaxamento
                fila.atualizar(fila, a.destino, nova);
                pai[a.destino] = u;
            }
        }
    }
}

bool grafo_tem_peso_negativo(int vertices, const lista_adj_ponderada& adj) {
    for (int u = 1; u <= vertices; ++u)
        for (const aresta_ponderada& a : adj[u])
            if (a.peso < 0) return true;
    return false;
}

bool dijkstra_lista(int s, int vertices, const lista_adj_ponderada& adj, int tipo_fila,
                    vector<double>& dist, vector<int>& pai) {
    if (s < 1 || s > vertices) {
        cout << "Vertice inicial invalido.\n";
        return false;
    }
    if (grafo_tem_peso_negativo(vertices, adj)) {
        cout << "\n[AVISO] O grafo possui arestas com peso negativo.\n"
             << "A biblioteca ainda nao implementa caminhos minimos com pesos negativos.\n";
        return false;
    }

    fila_distancias fila;
    if (tipo_fila == FILA_HEAP) fila_heap_iniciar(fila, vertices);
    else                        fila_vetor_iniciar(fila, vertices);

    dijkstra_generico(s, vertices, adj, fila, pai);
    dist = move(fila.chave);          // as chaves finais são as distâncias
    return true;
}

vector<int> caminho_minimo(const vector<int>& pai, int t) {
    vector<int> caminho;
    if (t < 1 || t >= (int)pai.size() || pai[t] == -1) return caminho;   // inalcançável
    for (int v = t; v != 0; v = pai[v]) caminho.push_back(v);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

bool salvar_dijkstra(const string& caminho_saida, int vertices, int s,
                     const vector<double>& dist, const vector<int>& pai) {
    ofstream arq(caminho_saida, ios::out | ios::trunc);
    if (!arq.is_open()) {
        cout << "Erro ao criar o arquivo de caminhos minimos!\n";
        return false;
    }

    arq << "ARVORE DE CAMINHOS MINIMOS (origem = " << s << ")\n";
    arq << "Vertice\tPai\tDistancia\n";
    arq << "-----------------------------\n";
    arq << setprecision(10);
    for (int i = 1; i <= vertices; ++i) {
        if (pai[i] != -1) arq << i << "\t" << pai[i] << "\t" << dist[i] << "\n";
        else              arq << i << "\t-\tInalcançável\n";
    }
    arq.close();
    return true;
}
