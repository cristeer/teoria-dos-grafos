#include "arquivos.hpp"
#include "algoritmos.hpp"

int main() {
    string caminho = "", saida = "", saida_bfs = "", saida_dfs = "";
    int vertices = 0, tipo_entrada = 0, select = 0, imprimir = 0, vertice_inicial = 1;

    do {

        do {
            system("cls"); // se for no linux/mac, trocar por "clear"
            cout << "\n";
            cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
            cout << "{|                 M E N U                   |}" << "\n";
            cout << "{|-------------------------------------------|}" << "\n";
            cout << "{| 1 - Matriz de Adjacencia                  |}" << "\n";
            cout << "{| 2 - Lista de Adjacencia                   |}" << "\n";
            cout << "{|-------------------------------------------|}" << "\n";
            cout << "{| 4 - Opcoes                                |}" << "\n";
            cout << "{| 0 - Sair                                  |}" << "\n";
            cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
            cout << "Escolha uma opcao: ";
            cin >> tipo_entrada;

            if (tipo_entrada == 4) {
                cout << "\nQuer imprimir o grafo gerado ao final?\n";
                cout << "  1 - Sim.\n";
                cout << "  0 - Nao.\n";
                cin >> imprimir;
                tipo_entrada = -1;
            }
        } while (tipo_entrada < 0 || tipo_entrada > 2);

        if (tipo_entrada == 0) {
            cout << "Saindo...\n";
            return 0;
        }

        do {
            system("cls");
            cout << "\n";
            cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
            cout << "{|               G R A F O S                 |}" << "\n";
            cout << "{|-------------------------------------------|}" << "\n";
            cout << "{| 1 - Grafo 01  (10000   / 10 Mil)          |}" << "\n";
            cout << "{| 2 - Grafo 02  (49948   / ~50 Mil)         |}" << "\n";
            cout << "{| 3 - Grafo 03  (375000  / ~375 Mil)        |}" << "\n";
            cout << "{| 4 - Grafo 04  (375000  / ~375 Mil)        |}" << "\n";
            cout << "{| 5 - Grafo 05  (4843750 / ~5 Milhoes)      |}" << "\n";
            cout << "{| 6 - Grafo 06  (4843750 / ~5 Milhoes)      |}" << "\n";
            cout << "{|-------------------------------------------|}" << "\n";
            cout << "{| 0 - Sair                                  |}" << "\n";
            cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
            cout << "Escolha o grafo: ";
            cin >> select;
        } while (select < 0 || select > 6);

        if (select == 0) {
            cout << "Saindo...\n";
            return 0;
        }

        // caminhos e arquivos de saída
        caminho = "instancias/grafo_" + to_string(select) + ".txt";
        string pasta_saida = (tipo_entrada == 1) ? "Saidas/Matriz/" : "Saidas/Listas/";
        string prefixo = "grafo_" + to_string(select);

        saida = pasta_saida + prefixo + "_info.txt";
        saida_bfs = pasta_saida + prefixo + "_bfs.txt";
        saida_dfs = pasta_saida + prefixo + "_dfs.txt";

        system("cls");

        // execucao da matriz de adj
        if (tipo_entrada == 1) {
            cout << "=== Lendo com Matriz de Adjacencia ===" << "\n";
            matriz_adj matriz;

            if (ler_grafo_matriz(caminho, vertices, matriz)) {
                cout << "Numero de vertices: " << vertices << "\n";

                if (imprimir == 1) {
                    cout << "\n   ";
                    for (int j = 1; j <= vertices; ++j) cout << j << " ";
                    cout << "\n";

                    for (int i = 1; i <= vertices; ++i) {
                        cout << i << "  ";
                        for (int j = 1; j <= vertices; ++j) {
                            cout << (matriz[i][j] ? "1 " : "0 ");
                        }
                        cout << "\n";
                    }
                }

                if (gerar_informacoes_matriz(saida, vertices, matriz)) {
                    cout << "\n[OK] Arquivo de informacoes gerado em: " << saida << "\n";
                }

                do {
                    cout << "\nInforme o vertice inicial para as buscas BFS e DFS (1 a " << vertices << "): ";
                    cin >> vertice_inicial;
                } while (vertice_inicial < 1 || vertice_inicial > vertices);

                vector<int> pai_bfs, nivel_bfs;
                cout << "\nExecutando BFS...";
                bfs_matriz(vertice_inicial, vertices, matriz, pai_bfs, nivel_bfs);
                if (salvar_arvore_busca(saida_bfs, vertices, pai_bfs, nivel_bfs)) {
                    cout << "\n[OK] Arvore BFS salva em: " << saida_bfs << "\n";
                }

                vector<int> pai_dfs, nivel_dfs;
                cout << "\nExecutando DFS...";
                dfs_matriz(vertice_inicial, vertices, matriz, pai_dfs, nivel_dfs);
                if (salvar_arvore_busca(saida_dfs, vertices, pai_dfs, nivel_dfs)) {
                    cout << "\n[OK] Arvore DFS salva em: " << saida_dfs << "\n";
                }
            }
        } 

        // execucao da lista de adj
        else if (tipo_entrada == 2) {
            cout << "=== Lendo com Lista de Adjacencia ===" << endl;
            lista_adj lista;

            if (ler_grafo_lista(caminho, vertices, lista)) {
                cout << "Numero de vertices: " << vertices << "\n";

                if (imprimir == 1) {
                    cout << "\n";
                    for (int i = 1; i <= vertices; ++i) {
                        cout << "Vertice " << i << ":";
                        for (int vizinho : lista[i]) {
                            cout << " -> " << vizinho;
                        }
                        cout << "\n";
                    }
                }

                if (gerar_informacoes_lista(saida, vertices, lista)) {
                    cout << "\n[OK] Arquivo de informacoes gerado em: " << saida << "\n";
                }

                do {
                    cout << "\nInforme o vertice inicial para as buscas BFS e DFS (1 a " << vertices << "): ";
                    cin >> vertice_inicial;
                } while (vertice_inicial < 1 || vertice_inicial > vertices);

                vector<int> pai_bfs, nivel_bfs;
                cout << "\nExecutando BFS...";
                bfs_lista(vertice_inicial, vertices, lista, pai_bfs, nivel_bfs);
                if (salvar_arvore_busca(saida_bfs, vertices, pai_bfs, nivel_bfs)) {
                    cout << "\n[OK] Arvore BFS salva em: " << saida_bfs << "\n";
                }

                vector<int> pai_dfs, nivel_dfs;
                cout << "\nExecutando DFS...";
                dfs_lista(vertice_inicial, vertices, lista, pai_dfs, nivel_dfs);
                if (salvar_arvore_busca(saida_dfs, vertices, pai_dfs, nivel_dfs)) {
                    cout << "\n[OK] Arvore DFS salva em: " << saida_dfs << "\n";
                }
            }
        }

        cout << "\n=============================================";
        cout << "\nPressione ENTER para voltar ao menu...";
        cin.ignore();
        cin.get();

    } while (select != 0);

    return 0;
}