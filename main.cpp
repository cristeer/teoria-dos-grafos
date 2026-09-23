#include "arquivos.hpp"



int main(){
//g++ main.cpp Geral.cpp Largura.cpp Profundidade.cpp -o main.exe
//.\main.exe

    string caminho="",saida="";
    int vertices = 0, TipoEntrada = 0, select = 0, imprimir=0;

    do{
    // Menu de seleção
    system("cls");
    do{
        cout << "\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}"<<endl;
        cout << "{|                 M E N U                   |}"<<endl;
        cout << "{|-------------------------------------------|}"<<endl;
        cout << "{| 1 - Matriz de Adjacência                  |}"<<endl;
        cout << "{| 2 - Lista de Adjacência                   |}"<<endl;
        cout << "{| 3 - Lista de Arestas                      |}"<<endl;
        cout << "{|-------------------------------------------|}"<<endl;
        cout << "{| 4 - Opções                                |}"<<endl;
        cout << "{| 0 - Sair                                  |}"<<endl;
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}"<<endl;
        cin >> TipoEntrada;
        system("cls");

        if(TipoEntrada == 4){
            cout << "\n Quer imprimir o grafo gerado ao final?\n";
            cout << "  1 - Sim.\n";
            cout << "  0 - Não.\n";
            cin>>imprimir;
            TipoEntrada = -1;
        }
        system("cls");
    } while (TipoEntrada < 0 || TipoEntrada > 3);

    if(TipoEntrada == 0){
        cout << "Saindo...\n";
        return 0;
    }

    // Grafos
    do{
        cout << "\n";
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
        system("cls");
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
        switch (select){
    case 1:{saida = "Saidas/Matriz/grafo_1.txt";}break;
    case 2:{saida = "Saidas/Matriz/grafo_2.txt";}break;
    case 3:{saida = "Saidas/Matriz/grafo_3.txt";}break;
    case 4:{saida = "Saidas/Matriz/grafo_4.txt";}break;
    case 5:{saida = "Saidas/Matriz/grafo_5.txt";}break;
    case 6:{saida = "Saidas/Matriz/grafo_6.txt";}break;}
    };break;
    case 2:{
        switch (select){
    case 1:{saida = "Saidas/Listas/grafo_1.txt";}break;
    case 2:{saida = "Saidas/Listas/grafo_2.txt";}break;
    case 3:{saida = "Saidas/Listas/grafo_3.txt";}break;
    case 4:{saida = "Saidas/Listas/grafo_4.txt";}break;
    case 5:{saida = "Saidas/Listas/grafo_5.txt";}break;
    case 6:{saida = "Saidas/Listas/grafo_6.txt";}break;}
    };break;

    }//fim da organização de saida;


    switch (TipoEntrada){
    case 1:{
        cout << "=== Lendo com Matriz de Adjacencia ===" << endl;
        matriz_adj matriz;

        if (ler_grafo_matriz(caminho, vertices, matriz)){
            cout << "Numero de vertices: " << vertices << "\n\n";

            if(imprimir == 1){
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
            // Gera o arquivo de informções;
            if (gerar_informacoes_matriz(saida,vertices,matriz)) {
                cout << "\nArquivo de informações gerado em nome de:\n   " << saida;
            }else{
                cout << "Erro ao gerar o Arquivo\n";
            }
        }
    }break;
    case 2:{
        cout << "=== Lendo com Lista de Adjacencia ===" << endl;
        lista_adj lista;

        if (ler_grafo_lista(caminho, vertices, lista)){
            cout << "Numero de vertices: " << vertices << "\n\n";

            if(imprimir == 1){
                // Imprime a lista de cada vértice
                for (int i = 1; i <= vertices; ++i){
                    cout << "Vertice " << i << ":";
                    for (int vizinho : lista[i]){
                        cout << " -> " << vizinho;
                    }
                    cout << "\n";
                }
            }
            
            // Gera o arquivo de informções;
            if (gerar_informacoes_lista(saida, vertices, lista)) {
                cout << "\nArquivo de informações gerado em nome de:\n   " << saida;
            }else{
                cout << "Erro ao gerar o Arquivo\n";
            }
        }
    }break;
    case 0:{cout << "Saindo...\n";}break;
    }// Finaliza o Tipo Entrada;

    cout<<"\n\nPressione ENTER para continuar...";
    getchar(); getchar();

    }while(select != 0);//volta pro MENU;

return 0;
}