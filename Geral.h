#ifndef GERAL_H
#define GERAL_H

#include<bits/stdc++.h>
using namespace std;

using lista_adj = vector<vector<int>>; //ex: 1 -> 2, 3, 4 
using matriz_adj = vector<vector<bool>>;//matrix básica...

bool ler_grafo_lista(const string& caminho, int& vertices, lista_adj& lista_adj);


bool ler_grafo_matriz(const string& caminho, int& vertices, matriz_adj& matriz_adj);




#endif // GERAL_H