#include "grafocompeso.hpp"
#include "arquivos.hpp"
#include "algoritmos.hpp"

#include <chrono>
#include <iomanip>
#include <unordered_map>

namespace {
bool executar_estudo_rede_colaboracao() {
    struct destino_pesquisador {
        string nome_enunciado;
        string nome_cadastro;
    };

    const string arquivo_grafo = "instancias/rede_colaboracao.txt";
    const string arquivo_vertices = "instancias/rede_colaboracao_vertices.txt";
    const string arquivo_resultado = "resultados_rede_colaboracao.txt";
    const string origem_nome = "Edsger W. Dijkstra";
    const vector<destino_pesquisador> destinos = {
        {"Alan M. Turing", "Alan M. Turing"},
        {"J. B. Kruskal", "J. B. Kruskal"},
        {"Jon M. Kleinberg", "Jon M. Kleinberg"},
        {"Eva Tardos", "Éva Tardos"},
        {"Daniel R. Figueiredo", "Daniel R. Figueiredo"}
    };

    int vertices = 0;
    lista_adj_ponderada adj;
    cout << "Carregando a rede de colaboracao...\n";
    if (!ler_grafo_lista(arquivo_grafo, vertices, adj)) return false;

    vector<string> nomes(vertices + 1);
    unordered_map<string, int> ids_pesquisados;
    ids_pesquisados.emplace(origem_nome, 0);
    for (const destino_pesquisador& destino : destinos) {
        ids_pesquisados.emplace(destino.nome_cadastro, 0);
    }

    ifstream arquivo_nomes(arquivo_vertices);
    if (!arquivo_nomes.is_open()) {
        cerr << "Erro ao abrir o arquivo de vertices: "
             << arquivo_vertices << ".\n";
        return false;
    }

    string linha;
    int registros = 0;
    while (getline(arquivo_nomes, linha)) {
        if (!linha.empty() && linha.back() == '\r') linha.pop_back();
        if (linha.empty()) continue;

        const size_t separador = linha.find(',');
        if (separador == string::npos) {
            cerr << "Registro invalido no arquivo de vertices: "
                 << linha << ".\n";
            return false;
        }

        int id = 0;
        try {
            size_t lidos = 0;
            id = stoi(linha.substr(0, separador), &lidos);
            if (lidos != separador || id < 1 || id > vertices) {
                cerr << "ID invalido no arquivo de vertices: "
                     << linha << ".\n";
                return false;
            }
        } catch (const exception&) {
            cerr << "ID invalido no arquivo de vertices: "
                 << linha << ".\n";
            return false;
        }

        const string nome = linha.substr(separador + 1);
        if (nomes[id].empty()) {
            ++registros;
        } else {
            cerr << "ID duplicado no arquivo de vertices: " << id << ".\n";
            return false;
        }
        nomes[id] = nome;

        const auto pesquisado = ids_pesquisados.find(nome);
        if (pesquisado != ids_pesquisados.end()) {
            if (pesquisado->second != 0) {
                cerr << "Nome exato duplicado no cadastro: " << nome << ".\n";
                return false;
            }
            pesquisado->second = id;
        }
    }

    if (registros != vertices) {
        cerr << "Quantidade de registros no arquivo de vertices (" << registros
             << ") difere da quantidade de vertices do grafo (" << vertices
             << ").\n";
        return false;
    }
    for (const auto& pesquisado : ids_pesquisados) {
        if (pesquisado.second == 0) {
            cerr << "Pesquisador nao encontrado com a grafia exata: "
                 << pesquisado.first << ".\n";
            return false;
        }
    }

    const int origem = ids_pesquisados.at(origem_nome);
    vector<double> distancias;
    vector<int> pai;
    const auto inicio = chrono::steady_clock::now();
    if (!dijkstra_lista(origem, vertices, adj, FILA_HEAP, distancias, pai)) {
        return false;
    }
    const auto fim = chrono::steady_clock::now();
    const auto tempo_ms =
        chrono::duration<double, milli>(fim - inicio).count();

    ofstream resultado(arquivo_resultado);
    if (!resultado.is_open()) {
        cerr << "Erro ao criar o arquivo de resultados: "
             << arquivo_resultado << ".\n";
        return false;
    }

    resultado << "ESTUDO DE CASO - REDE DE COLABORACAO\n"
              << "Origem: " << origem_nome << " vertice " << origem << "\n"
              << "Grafo: " << arquivo_grafo << '\n'
              << "Vertices: " << vertices << '\n'
              << "Algoritmo: Dijkstra com heap\n"
              << "Tempo de execucao: " << fixed << setprecision(4)
              << tempo_ms << " ms\n"
              << "Distancia e soma dos pesos das arestas do caminho minimo.\n\n";

    cout << "\nOrigem: " << origem_nome << " (vertice " << origem << ")\n";
    for (const destino_pesquisador& pesquisador : destinos) {
        const int destino = ids_pesquisados.at(pesquisador.nome_cadastro);
        const vector<int> caminho = caminho_minimo(pai, destino);
        resultado << "Destino: " << pesquisador.nome_enunciado
                  << " (vertice " << destino << ")\n";
        cout << "\nDestino: " << pesquisador.nome_enunciado
             << " (vertice " << destino << ")\n";
        if (pesquisador.nome_cadastro != pesquisador.nome_enunciado) {
            resultado << "Grafia no cadastro: "
                      << pesquisador.nome_cadastro << '\n';
            cout << "Grafia no cadastro: "
                 << pesquisador.nome_cadastro << '\n';
        }

        if (caminho.empty()) {
            resultado << "Nao existe caminho entre os pesquisadores.\n\n";
            cout << "Nao existe caminho entre os pesquisadores.\n";
            continue;
        }

        resultado << "Distancia minima: " << setprecision(15)
                  << distancias[destino] << "\nCaminho minimo: ";
        cout << "Distancia minima: " << setprecision(15)
             << distancias[destino] << "\nCaminho minimo: ";
        for (size_t i = 0; i < caminho.size(); ++i) {
            if (i > 0) {
                resultado << " -> ";
                cout << " -> ";
            }
            resultado << nomes[caminho[i]] << " [" << caminho[i] << "]";
            cout << nomes[caminho[i]] << " [" << caminho[i] << "]";
        }
        resultado << "\n\n";
        cout << '\n';
    }

    cout << "\nResultados salvos em " << arquivo_resultado << ".\n";
    return true;
}
}

// Lê o grafo, pergunta origem/destino, roda o Dijkstra com a estrutura escolhida
// e grava a árvore de caminhos mínimos. O algoritmo é o MESMO para as duas
// estruturas; só muda o tipo_fila (veja algoritmos.cpp).
bool executarDijkstra(string caminho, string saida, int tipo_fila, int imprimir){
    int vertices = 0, vertice_inicial = 1, vertice_destino = 1;
    lista_adj_ponderada lista;

    ofstream arquivo("Agrupamento-Tempo-Dikstra.txt", ios::app);
    if (!arquivo.is_open()){
        cerr << "Erro ao abrir o arquivo: " << endl;
        return false;
    }

    cout << "=== Dijkstra com " << (tipo_fila == FILA_HEAP ? "Heap" : "Vetor") << " ===\n";
    if (!ler_grafo_lista(caminho, vertices, lista)){
        return false;
    }
    cout << "Numero de vertices: " << vertices << "\n";

    if (imprimir == 1){
        cout << "\n";
        for (int i = 1; i <= vertices; ++i){
            cout << "Vertice " << i << ":";
            for (const aresta_ponderada& a : lista[i]){
                cout << " -> " << a.destino << " (" << a.peso << ")";
            }
            cout << "\n";
        }
    }

    do{
        cout << "\nInforme o vertice inicial (1 a " << vertices << "): ";
        cin >> vertice_inicial;
    } while (vertice_inicial < 1 || vertice_inicial > vertices);

    do{
        cout << "\nInforme o vertice destino para exibir o caminho (1 a " << vertices << "): ";
        cin >> vertice_destino;
    } while (vertice_destino < 1 || vertice_destino > vertices);

    vector<double> dist;
    vector<int> pai;

    auto inicio = chrono::high_resolution_clock::now();
    bool ok = dijkstra_lista(vertice_inicial, vertices, lista, tipo_fila, dist, pai);
    auto fim = chrono::high_resolution_clock::now();
    if (!ok){
        return false;   // peso negativo: a mensagem já foi exibida
    }
    long long tempo = chrono::duration_cast<chrono::microseconds>(fim - inicio).count();
    cout << "\nDijkstra executado em " << tempo << " microssegundos.\n";

    if (salvar_dijkstra(saida, vertices, vertice_inicial, dist, pai)){
        cout << "[OK] Arvore de caminhos minimos salva em: " << saida << "\n";
    }

    vector<int> caminho_min = caminho_minimo(pai, vertice_destino);

    if (caminho_min.empty()){
        cout << "\nVertice " << vertice_destino << " nao e alcancavel a partir de " << vertice_inicial << ".\n";
    } else {
        cout << "\nDistancia " << vertice_inicial << " -> " << vertice_destino << ": " << dist[vertice_destino] << "\n";
        cout << "Caminho: ";
        arquivo << caminho << "\n";
        arquivo << "Tipo: " << tipo_fila << "\nInicio " << vertice_inicial << " / Destino " << vertice_destino << " / Distancia " << dist[vertice_destino] << " / Tempo " << tempo << endl << "Caminho: ";
        for (size_t i = 0; i < caminho_min.size(); ++i){
            cout << caminho_min[i] << (i + 1 == caminho_min.size() ? "" : " -> ");
            arquivo << caminho_min[i] << (i + 1 == caminho_min.size() ? "" : " -> ");
        }
        arquivo << "\n=============================================\n";
        cout << "\n";

        arquivo << endl;
        arquivo.close();
    }
    return true;
}

void DijkstraVetor(string caminho, string saida, int imprimir){
    executarDijkstra(caminho, saida, FILA_VETOR, imprimir);
}

void DijkstraHeap(string caminho, string saida, int imprimir){
    executarDijkstra(caminho, saida, FILA_HEAP, imprimir);
}

void gcpMenu(){
    string caminho = "", saida = "";
    int tipo_entrada = 0, select = 0, imprimir = 0;

    ofstream arquivo("Agrupamento-Tempo-Dikstra.txt", ios::app);
    if (!arquivo.is_open()){
        cerr << "Erro ao abrir o arquivo: " << endl;
        return;
    }

    do{
    grafo:
        system("cls"); // se for no linux/mac, trocar por "clear"
        cout << "\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
        cout << "{|       G R A F O S   C O M   P E S O       |}" << "\n";
        cout << "{|-------------------------------------------|}" << "\n";
        cout << "{| 1 - Dijkstra por Vetor                    |}" << "\n";
        cout << "{| 2 - Dijkstra por Heap                     |}" << "\n";
        cout << "{| 3 - Estudo de caso: rede de colaboracao   |}" << "\n";
        cout << "{|-------------------------------------------|}" << "\n";
        cout << "{| 4 - Opcoes                                |}" << "\n";
        cout << "{| 0 - Sair                                  |}" << "\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
        cout << "Escolha uma opcao: ";
        cin >> tipo_entrada;

        if (tipo_entrada == 4){
            cout << "\nQuer imprimir o grafo gerado ao final?\n";
            cout << "  1 - Sim.\n";
            cout << "  0 - Nao.\n";
            cin >> imprimir;
            tipo_entrada = -1;
        }
    } while (tipo_entrada < 0 || tipo_entrada > 4);

    if (tipo_entrada == 3) {
        executar_estudo_rede_colaboracao();
        return;
    }

    if (tipo_entrada == 0){
        cout << "Saindo...\n";
        return;
    }

    do{
        system("cls");
        cout << "\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
        cout << "{|               G R A F O S                 |}" << "\n";
        cout << "{|-------------------------------------------|}" << "\n";
        cout << "{| 1 - Grafo 01  (10000   / 10 Mil)          |}" << "\n";
        cout << "{| 2 - Grafo 02  (25000   / 25 Mil)          |}" << "\n";
        cout << "{| 3 - Grafo 03  (100000  / 100 Mil)         |}" << "\n";
        cout << "{| 4 - Grafo 04  (1000000 / 1 Milhão)        |}" << "\n";
        cout << "{| 5 - Grafo 05  (10000000 / 10 Milhões)     |}" << "\n";
        cout << "{|-------------------------------------------|}" << "\n";
        cout << "{| 0 - Sair                                  |}" << "\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
        cout << "Escolha o grafo: ";
        cin >> select;
    } while (select < 0 || select > 5);

    if (select == 0){
        cout << "Saindo...\n";
        goto grafo;
    }

    
    // caminhos e arquivos de saída (pastas criadas em gera_pastas())
    caminho = "instancias/grafo_W_" + to_string(select) + ".txt";
    string pasta_saida = (tipo_entrada == 1) ? "Saidas/Dijkstra/Vetor/" : "Saidas/Dijkstra/Heap/";
    saida = pasta_saida + "grafo_W_" + to_string(select) + "_dijkstra.txt";

    system("cls");

    switch (tipo_entrada){
        case 1:{
            DijkstraVetor(caminho, saida, imprimir);
        }break;

        case 2:{
            DijkstraHeap(caminho, saida, imprimir);
        }break;
    }
}
