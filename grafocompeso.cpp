#include "grafocompeso.hpp"
#include "arquivos.hpp"
#include "algoritmos.hpp"

//criei para ajustar o menu
void DijkstraVetor(string caminho, string saida, string saida_vetor, int imprimir){


}


//só criei para ajustar o menu
void DijkstraHeap(string caminho, string saida, string saida_vetor, int imprimir){


}





void gcpMenu(){
    string caminho = "", saida = "", saida_vetor = "", saida_heap = "";
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

    
    // caminhos e arquivos de saída
    caminho = "instancias/grafo_W_" + to_string(select) + ".txt";
    string pasta_saida = (tipo_entrada == 1) ? "Saidas/Dijkstra_Vetor/" : "Saidas/Dijkstra_Heap/";
    string prefixo = "grafo_W_" + to_string(select);

    saida = pasta_saida + prefixo + "_info.txt";
    saida_vetor = pasta_saida + prefixo + "_vetor.txt";
    saida_heap = pasta_saida + prefixo + "_heap.txt";
        
    system("cls");

    switch (tipo_entrada){  //AJUSTAR AS PASTAS, Criar somente as nessesárias
        case 1:{
            DijkstraVetor(caminho, saida, saida_vetor, imprimir);
        }break;

        case 2:{
            DijkstraHeap(caminho, saida, saida_heap, imprimir);
        }break;

    }


}

