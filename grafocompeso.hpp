#ifndef GRAFOCOMPESO_HPP
#define GRAFOCOMPESO_HPP

#include "arquivos.hpp"
#include <string>

// Executa o Dijkstra a partir de um vértice lido do usuário, usando a estrutura
// indicada (FILA_VETOR / FILA_HEAP, ver algoritmos.hpp). Retorna false em caso de erro.
bool executarDijkstra(string caminho, string saida, int tipo_fila, int imprimir);

void DijkstraVetor(string caminho, string saida, int imprimir);
void DijkstraHeap(string caminho, string saida, int imprimir);

void gcpMenu();

#endif
