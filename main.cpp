#include "Geral.h"
#include "Largura.h"
#include "Profundidade.h"

#include <bits/stdc++.h>
using namespace std;

int main(){
//g++ main.cpp Geral.cpp Largura.cpp Profundidade.cpp -o main.exe
//.\main.exe

    string caminho="";
    int vertices = 0, TipoEntrada = 0, select = 0;

    // Menu de seleção
    do{
        cout << "\n\n\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}"<<endl;
        cout << "{|                 M E N U                   |}"<<endl;
        cout << "{|-------------------------------------------|}"<<endl;
        cout << "{| 1 - Matriz de Adjacência                  |}"<<endl;
        cout << "{| 2 - Lista de Adjacência                   |}"<<endl;
        cout << "{|-------------------------------------------|}"<<endl;
        cout << "{| 0 - Sair                                  |}"<<endl;
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}"<<endl;
        cin >> TipoEntrada;
        if (TipoEntrada < 0 || TipoEntrada > 2){
            cout << "Opção inválida. Tente novamente.\n";
        }
    } while (TipoEntrada < 0 || TipoEntrada > 2);

    // Grafos
    do{
        cout << "\n\n\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}"<<endl;
        cout << "{|               G R A F O S                 |}"<<endl;
        cout << "{|-------------------------------------------|}"<<endl;
        cout << "{| 1 - Grafo 01  (10000   / 10 Mil)          |}"<<endl;
        cout << "{| 2 - Grafo 02  (49948   / ~50 Mil)         |}"<<endl;
        cout << "{| 3 - Grafo 03  (375000  / ~375 Mil)        |}"<<endl;
        cout << "{| 4 - Grafo 04  (375000  / ~375 Mil)        |}"<<endl;
        cout << "{| 5 - Grafo 05  (4843750 / ~5 Milhões)      |}"<<endl;
        cout << "{| 6 - Grafo 06  (4843750 / ~5 Milhões)      |}"<<endl;
        cout << "{|-------------------------------------------|}"<<endl;
        cout << "{| 0 - Sair                                  |}"<<endl;
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}"<<endl;
        cin >> select;
        if (select < 0 || select > 6){
            cout << "Opção inválida. Tente novamente.\n";
        }
    } while (select < 0 || select > 6);

    switch (select){
    case 1:{caminho = "Grafos/grafo_1/grafo_1.txt";}break;
    case 2:{caminho = "Grafos/grafo_2/grafo_2.txt";}break;
    case 3:{caminho = "Grafos/grafo_3/grafo_3.txt";}break;
    case 4:{caminho = "Grafos/grafo_4/grafo_4.txt";}break;
    case 5:{caminho = "Grafos/grafo_5/grafo_5.txt";}break;
    case 6:{caminho = "Grafos/grafo_6/grafo_6.txt";}break;
    case 0:{cout << "Saindo...\n";}break;
    }

    switch (TipoEntrada){
    case 1:{
        cout << "=== Lendo com Matriz de Adjacencia ===" << endl;
        matriz_adj matriz;

        if (ler_grafo_matriz(caminho, vertices, matriz)){
            cout << "Sucesso! Numero de vertices: " << vertices << "\n\n";

            // Imprime a matriz de adjacência (1 para existe aresta, 0 para não existe)
            cout << "   ";
            for (int j = 1; j <= vertices; ++j)
                cout << j << " ";
            cout << "\n";

            for (int i = 1; i <= vertices; ++i){
                cout << i << "  ";
                for (int j = 1; j <= vertices; ++j){
                    cout << (matriz[i][j] ? "1 " : "0 ");
                }
                cout << "\n";
            }
        }
    }break;
    case 2:{
        cout << "=== Lendo com Lista de Adjacencia ===" << endl;
        lista_adj lista;

        if (ler_grafo_lista(caminho, vertices, lista)){
            cout << "Sucesso! Numero de vertices: " << vertices << "\n\n";

            // Imprime a lista de cada vértice
            for (int i = 1; i <= vertices; ++i){
                cout << "Vertice " << i << ":";
                for (int vizinho : lista[i]){
                    cout << " -> " << vizinho;
                }
                cout << "\n";
            }
        }
    }break;
    case 0:{cout << "Saindo...\n";}break;
    }// Finaliza o Tipo Entrada;


return 0;
}