#include "grafocompeso.hpp"
#include "arquivos.hpp"
#include "algoritmos.hpp"

// Lê o grafo, pergunta origem/destino, roda o Dijkstra com a estrutura escolhida
// e grava a árvore de caminhos mínimos. O algoritmo é o MESMO para as duas
// estruturas; só muda o tipo_fila (veja algoritmos.cpp).
bool executarDijkstra(string caminho, string saida, int tipo_fila, int imprimir){
    int vertices = 0, vertice_inicial = 1, vertice_destino = 1;
    lista_adj_ponderada lista;

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
        for (size_t i = 0; i < caminho_min.size(); ++i){
            cout << caminho_min[i] << (i + 1 == caminho_min.size() ? "" : " -> ");
        }
        cout << "\n";
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

    do{
    grafo:
        system("cls"); // se for no linux/mac, trocar por "clear"
        cout << "\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
        cout << "{|       G R A F O S   C O M   P E S O       |}" << "\n";
        cout << "{|-------------------------------------------|}" << "\n";
        cout << "{| 1 - Dijkstra por Vetor                    |}" << "\n";
        cout << "{| 2 - Dijkstra por Heap                     |}" << "\n";
        cout << "{|-------------------------------------------|}" << "\n";
        cout << "{| 3 - Opcoes                                |}" << "\n";
        cout << "{| 0 - Sair                                  |}" << "\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
        cout << "Escolha uma opcao: ";
        cin >> tipo_entrada;

        if (tipo_entrada == 3){
            cout << "\nQuer imprimir o grafo gerado ao final?\n";
            cout << "  1 - Sim.\n";
            cout << "  0 - Nao.\n";
            cin >> imprimir;
            tipo_entrada = -1;
        }
    } while (tipo_entrada < 0 || tipo_entrada > 2);

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
        cout << "{| 5 - Grafo 05  (1000000 / 1 Milhão)        |}" << "\n";
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
