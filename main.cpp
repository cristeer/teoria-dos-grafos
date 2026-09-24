#include "arquivos.hpp"
#include "buscas.hpp"

int main() {
    string caminho = "", saida = "", saida_bfs = "", saida_dfs = "";
    // Variavel 'verticeInicial' declarada corretamente aqui:
    int vertices = 0, TipoEntrada = 0, select = 0, imprimir = 0, verticeInicial = 1;

    do {
        // =========================================================
        // MENU DE SELEÇÃO DA ESTRUTURA DE DADOS
        // =========================================================
        do {
            system("cls"); // Se estiver no Linux/Mac, troque por system("clear");
            cout << "\n";
            cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << endl;
            cout << "{|                 M E N U                   |}" << endl;
            cout << "{|-------------------------------------------|}" << endl;
            cout << "{| 1 - Matriz de Adjacencia                  |}" << endl;
            cout << "{| 2 - Lista de Adjacencia                   |}" << endl;
            cout << "{|-------------------------------------------|}" << endl;
            cout << "{| 4 - Opcoes                                |}" << endl;
            cout << "{| 0 - Sair                                  |}" << endl;
            cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << endl;
            cout << "Escolha uma opcao: ";
            cin >> TipoEntrada;

            if (TipoEntrada == 4) {
                cout << "\nQuer imprimir o grafo gerado ao final?\n";
                cout << "  1 - Sim.\n";
                cout << "  0 - Nao.\n";
                cin >> imprimir;
                TipoEntrada = -1;
            }
        } while (TipoEntrada < 0 || TipoEntrada > 2);

        if (TipoEntrada == 0) {
            cout << "Saindo...\n";
            return 0;
        }

        // =========================================================
        // MENU DE SELEÇÃO DO GRAFO
        // =========================================================
        do {
            system("cls");
            cout << "\n";
            cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << endl;
            cout << "{|               G R A F O S                 |}" << endl;
            cout << "{|-------------------------------------------|}" << endl;
            cout << "{| 1 - Grafo 01  (10000   / 10 Mil)          |}" << endl;
            cout << "{| 2 - Grafo 02  (49948   / ~50 Mil)         |}" << endl;
            cout << "{| 3 - Grafo 03  (375000  / ~375 Mil)        |}" << endl;
            cout << "{| 4 - Grafo 04  (375000  / ~375 Mil)        |}" << endl;
            cout << "{| 5 - Grafo 05  (4843750 / ~5 Milhoes)      |}" << endl;
            cout << "{| 6 - Grafo 06  (4843750 / ~5 Milhoes)      |}" << endl;
            cout << "{|-------------------------------------------|}" << endl;
            cout << "{| 0 - Sair                                  |}" << endl;
            cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << endl;
            cout << "Escolha o grafo: ";
            cin >> select;
        } while (select < 0 || select > 6);

        if (select == 0) {
            cout << "Saindo...\n";
            return 0;
        }

        // Caminhos e arquivos de saída
        caminho = "instancias/grafo_" + to_string(select) + ".txt";
        string pastaSaida = (TipoEntrada == 1) ? "Saidas/Matriz/" : "Saidas/Listas/";
        string prefixo = "grafo_" + to_string(select);

        saida     = pastaSaida + prefixo + "_info.txt";
        saida_bfs = pastaSaida + prefixo + "_bfs.txt";
        saida_dfs = pastaSaida + prefixo + "_dfs.txt";

        system("cls");

        // =========================================================
        // EXECUÇÃO DA MATRIZ DE ADJACÊNCIA
        // =========================================================
        if (TipoEntrada == 1) {
            cout << "=== Lendo com Matriz de Adjacencia ===" << endl;
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
                    cin >> verticeInicial;
                } while (verticeInicial < 1 || verticeInicial > vertices);

                vector<int> pai_bfs, nivel_bfs;
                cout << "\nExecutando BFS...";
                bfs_matriz(verticeInicial, vertices, matriz, pai_bfs, nivel_bfs);
                if (salvar_arvore_busca(saida_bfs, vertices, pai_bfs, nivel_bfs)) {
                    cout << "\n[OK] Arvore BFS salva em: " << saida_bfs << "\n";
                }

                vector<int> pai_dfs, nivel_dfs;
                cout << "\nExecutando DFS...";
                dfs_matriz(verticeInicial, vertices, matriz, pai_dfs, nivel_dfs);
                if (salvar_arvore_busca(saida_dfs, vertices, pai_dfs, nivel_dfs)) {
                    cout << "\n[OK] Arvore DFS salva em: " << saida_dfs << "\n";
                }
            }
        } 
        // =========================================================
        // EXECUÇÃO DA LISTA DE ADJACÊNCIA
        // =========================================================
        else if (TipoEntrada == 2) {
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
                    cin >> verticeInicial;
                } while (verticeInicial < 1 || verticeInicial > vertices);

                vector<int> pai_bfs, nivel_bfs;
                cout << "\nExecutando BFS...";
                bfs_lista(verticeInicial, vertices, lista, pai_bfs, nivel_bfs);
                if (salvar_arvore_busca(saida_bfs, vertices, pai_bfs, nivel_bfs)) {
                    cout << "\n[OK] Arvore BFS salva em: " << saida_bfs << "\n";
                }

                vector<int> pai_dfs, nivel_dfs;
                cout << "\nExecutando DFS...";
                dfs_lista(verticeInicial, vertices, lista, pai_dfs, nivel_dfs);
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