#ifndef ARQUIVOS_HPP
#define ARQUIVOS_HPP

#include<iostream>
#include<fstream>
#include<vector>
#include<filesystem>
#include<iomanip>
#include<algorithm>

using namespace std;

using lista_adj = vector<vector<int>>; //ex: 1 -> 2, 3, 4 
using matriz_adj = vector<vector<bool>>;//matrix básica...

void verificadorDePastaSimples(filesystem::path pastinha);

bool gerar_informacoes_lista(const string& caminhoSaida,int vertices,const lista_adj& lista);

bool ler_grafo_lista(const string&  caminho, int& vertices, lista_adj& lista_adj);

bool gerar_informacoes_matriz(const string& caminhoSaida,int vertices,const matriz_adj& matriz) ;

bool ler_grafo_matriz(const string& caminho, int& vertices, matriz_adj& matriz_adj);




#endif // GERAL_H