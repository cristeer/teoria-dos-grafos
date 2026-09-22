#include <iostream>
#include "grafo.hpp"

using namespace std;

int main() {
    string caminho = "instancias/grafo_5.txt"; // Nome do seu arquivo de texto
    int vertices = 0;

    // 1. Teste com Lista de Adjacência
    cout << "=== Lendo com Lista de Adjacencia ===" << endl;
    lista_adj lista;
    
    if (ler_grafo_lista(caminho, vertices, lista)) {
        cout << "Sucesso! Numero de vertices: " << vertices << "\n\n";

        // Imprime a lista de cada vértice
        for (int i = 1; i <= vertices; ++i) {
            cout << "Vertice " << i << ":";
            for (int vizinho : lista[i]) {
                cout << " -> " << vizinho;
            }
            cout << "\n";
        }
    }

    cout << "\n=====================================\n\n";

    // 2. Teste com Matriz de Adjacência
    cout << "=== Lendo com Matriz de Adjacencia ===" << endl;
    matriz_adj matriz;

    if (ler_grafo_matriz(caminho, vertices, matriz)) {
        cout << "Sucesso! Numero de vertices: " << vertices << "\n\n";

        // Imprime a matriz de adjacência (1 para existe aresta, 0 para não existe)
        cout << "   ";
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

    return 0;
}